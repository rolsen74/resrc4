
/*
** Copyright (c) 2014-2025 Rene W. Olsen
**
** SPDX-License-Identifier: GPL-3.0-or-later
**
** This software is released under the GNU General Public License, version 3.
** For the full text of the license, please visit:
** https://www.gnu.org/licenses/gpl-3.0.html
**
** You can also find a copy of the license in the LICENSE file included with this software.
*/

// --

#include "Resourcer/ReSrc4.h"

// --

#define DDEBUG(x)

// --

#include "M68k_EA_00.inc"			// Clr Dn
#include "M68k_EA_10.inc"			// Clr An
#include "M68k_EA_20.inc"			// Clr (An)
#include "M68k_EA_30.inc"			// Clr (An)+
#include "M68k_EA_40.inc"			// Clr -(An)
#include "M68k_EA_50.inc"			// Clr (xxxx.w,An)
#include "M68k_EA_60_Breif.inc"		// Clr (d8,An,Xn.SIZE*SCALE)
#include "M68k_EA_60_Full.inc"
#include "M68k_EA_70.inc"			// Clr $0004.w
#include "M68k_EA_71.inc"			// Clr $00000004.l
#include "M68k_EA_72.inc"			// Clr (xxxx.w,pc)
#include "M68k_EA_73_Breif.inc"		// Clr (d8,PC,Xn.SIZE*SCALE)
#include "M68k_EA_73_Full.inc"
#include "M68k_EA_74.inc"			// # <xxx>

// --

enum RS4DecodeStat M68k_EffectiveAddress( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
STR outstr;
STR buf;
S32 mode;
S32 pos;

	buf = rt->rt_Container.Hunk.ms_Buf_Argument;
	pos = strlen( buf );

	if ( pos )
	{
		buf[pos++] = ',';
		buf[pos] = 0;
	}

	outstr = & buf[pos];

	mode = (( rt->rt_CPU.M68k.mt_ArgEMode << 4 ) | ( rt->rt_CPU.M68k.mt_ArgEReg ));

	switch( mode )
	{
		case 0x00: // 000 REG
		case 0x01: // Clr Dn
		case 0x02:
		case 0x03:
		case 0x04:
		case 0x05:
		case 0x06:
		case 0x07:
		{
			DDEBUG( { printf( "%s:%04d: EA Mode 0x00 - Adr: $%08" PRIx64 "\n", __FILE__, __LINE__, rt->rt_CurMemAdr ); } )

			ds = MODE_00( & ec, rt, outstr );
			break;
		}

		case 0x10: // 001 REG
		case 0x11: // Clr An
		case 0x12:
		case 0x13:
		case 0x14:
		case 0x15:
		case 0x16:
		case 0x17:
		{
			DDEBUG( { printf( "%s:%04d: EA Mode 0x10 - Adr: $%08" PRIx64 "\n", __FILE__, __LINE__, rt->rt_CurMemAdr ); } )

			ds = MODE_10( & ec, rt, outstr );
			break;
		}

		case 0x20: // 010 REG
		case 0x21: // Clr (An)
		case 0x22:
		case 0x23:
		case 0x24:
		case 0x25:
		case 0x26:
		case 0x27:
		{
			DDEBUG( { printf( "%s:%04d: EA Mode 0x20 - Adr: $%08" PRIx64 "\n", __FILE__, __LINE__, rt->rt_CurMemAdr ); } )

			ds = MODE_20( & ec, rt, outstr );
			break;
		}

		case 0x30: // 011 REG
		case 0x31: // Clr (An)+
		case 0x32:
		case 0x33:
		case 0x34:
		case 0x35:
		case 0x36:
		case 0x37:
		{
			DDEBUG( { printf( "%s:%04d: EA Mode 0x30 - Adr: $%08" PRIx64 "\n", __FILE__, __LINE__, rt->rt_CurMemAdr ); } )

			ds = MODE_30( & ec, rt, outstr );
			break;
		}

		case 0x40: // 100 REG
		case 0x41: // Clr -(An)
		case 0x42:
		case 0x43:
		case 0x44:
		case 0x45:
		case 0x46:
		case 0x47:
		{
			DDEBUG( { printf( "%s:%04d: EA Mode 0x40 - Adr: $%08" PRIx64 "\n", __FILE__, __LINE__, rt->rt_CurMemAdr ); } )

			ds = MODE_40( & ec, rt, outstr );
			break;
		}

		case 0x50: // 101 REG
		case 0x51: // jsr (d16,An)
		case 0x52:
		case 0x53:
		case 0x54:
		case 0x55:
		case 0x56:
		case 0x57:
		{
			DDEBUG( { printf( "%s:%04d: EA Mode 0x50 - Adr: $%08" PRIx64 "\n", __FILE__, __LINE__, rt->rt_CurMemAdr ); } )

			ds = MODE_50( & ec, rt, outstr );
			break;
		}

		case 0x60: // 110 REG
		case 0x61: // ( d8 , An , Xn.SIZE * SCALE )
		case 0x62: // ( bd , An , Xn.SIZE * SCALE )
		case 0x63: // ( [ bd , An ] , Xn.SIZE * SCALE , od )
		case 0x64: // ( [ bd , An , Xn.SIZE * SCALE ] , od )
		case 0x65:
		case 0x66:
		case 0x67:
		{
			if ( rt->rt_CurMemBuf[ rt->rt_CPU.M68k.mt_ArgSize ] & 0x01 )
			{
				DDEBUG( { printf( "%s:%04d: EA Mode 0x60 Full - Adr: $%08" PRIx64 "\n", __FILE__, __LINE__, rt->rt_CurMemAdr ); } )

				// Full Extension Word Format
				if ( rt->rt_CurMemBuf[ rt->rt_CPU.M68k.mt_ArgSize + 1 ] & 0x08 )
				{
					printf( "%s:%04d: 60_1 : Unsupported EA mode %02x at $%08" PRIx64 "\n", __FILE__, __LINE__, mode, rt->rt_CurMemAdr );
					ds = RS4DecodeStat_UnknownCmd;
				}
				else
				{
					ds = MODE_60_Full_0( & ec, rt, outstr );
				}
			}
			else
			{
				DDEBUG( { printf( "%s:%04d: EA Mode 0x60 Breif - Adr: $%08" PRIx64 "\n", __FILE__, __LINE__, rt->rt_CurMemAdr ); } )

				// Brief Extension Word Format
				ds = MODE_60_Brief( & ec, rt, outstr );
			}
			break;
		}

		case 0x70: // 111 000
		{
			DDEBUG( { printf( "%s:%04d: EA Mode 0x70 - Adr: $%08" PRIx64 "\n", __FILE__, __LINE__, rt->rt_CurMemAdr ); } )

			// move.l $0004.w
			// ( xxx ).W
			ds = MODE_70( & ec, rt, outstr );
			break;
		}

		case 0x71: // 111 001
		{
			DDEBUG( { printf( "%s:%04d: EA Mode 0x71 - Adr: $%08" PRIx64 "\n", __FILE__, __LINE__, rt->rt_CurMemAdr ); } )

			// move.l $00000004
			// ( xxx ).L
			ds = MODE_71( & ec, rt, outstr );
			break;
		}

		case 0x72: // 111 010
		{
			DDEBUG( { printf( "%s:%04d: EA Mode 0x72 - Adr: $%08" PRIx64 "\n", __FILE__, __LINE__, rt->rt_CurMemAdr ); } )

			// Clr (xxxx.w,pc)
			// ( d16, PC )
			ds = MODE_72( & ec, rt, outstr );
			break;
		}

		case 0x73: // 110 011
		{
			// ( d8 , PC , Xn.SIZE * SCALE )
			// ( bd , PC , Xn.SIZE * SCALE )
			// ( [ bd , PC ] , Xn.SIZE * SCALE , od )
			// ( [ bd , PC , Xn.SIZE * SCALE ] , od )
			if ( rt->rt_CurMemBuf[ rt->rt_CPU.M68k.mt_ArgSize ] & 0x01 )
			{
				DDEBUG( { printf( "%s:%04d: EA Mode 0x73 Full - Adr: $%08" PRIx64 "\n", __FILE__, __LINE__, rt->rt_CurMemAdr ); } )

				// Full Extension Word Format
				if ( rt->rt_CurMemBuf[ rt->rt_CPU.M68k.mt_ArgSize + 1 ] & 0x08 )
				{
					printf( "%s:%04d: 73_1 : Unsupported EA mode $%02x at $%08" PRIx64 "\n", __FILE__, __LINE__, mode, rt->rt_CurMemAdr );
					ds = RS4DecodeStat_UnknownCmd;
				}
				else
				{
					ds = MODE_73_Full_0( & ec, rt, outstr );
				}
			}
			else
			{
				DDEBUG( { printf( "%s:%04d: EA Mode 0x73 Breif - Adr: $%08" PRIx64 "\n", __FILE__, __LINE__, rt->rt_CurMemAdr ); } )

				// Brief Extension Word Format
				ds = MODE_73_Brief( & ec, rt, outstr );
			}
			break;
		}

		case 0x74: // 111
		{
			DDEBUG( { printf( "%s:%04d: EA Mode 0x74 - Adr: $%08" PRIx64 "\n", __FILE__, __LINE__, rt->rt_CurMemAdr ); } )

			// # <xxx>
			ds = MODE_74( & ec, rt, outstr );
			break;
		}

		default:
		{
			printf( "%s:%04d: Unsupported EA mode $%02x at $%08" PRIx64 "\n", __FILE__, __LINE__, mode, rt->rt_CurMemAdr );
			ds = RS4DecodeStat_UnknownCmd;
			break;
		}
	}

	if ( errcode )
	{
		*errcode = ec;
	}

	return( ds );
}
