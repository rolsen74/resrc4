
/*
 * Copyright (c) 2014-2024 Rene W. Olsen < renewolsen @ gmail . com >
 *
 * This software is released under the GNU General Public License, version 3.
 * For the full text of the license, please visit:
 * https://www.gnu.org/licenses/gpl-3.0.html
 *
 * You can also find a copy of the license in the LICENSE file included with this software.
 */

// --

#include "ReSrc4.h"

// --

#include "M68k__EA_00.c"		// Clr Dn
#include "M68k__EA_10.c"		// Clr An
#include "M68k__EA_20.c"		// Clr (An)
#include "M68k__EA_30.c"		// Clr (An)+
#include "M68k__EA_40.c"		// Clr -(An)
#include "M68k__EA_50.c"		// Clr (xxxx.w,An)
#include "M68k__EA_60_Breif.c"	// Clr (d8,An,Xn.SIZE*SCALE)
#include "M68k__EA_60_Full.c"
#include "M68k__EA_70.c"		// Clr $0004.w
#include "M68k__EA_71.c"		// Clr $00000004.l
#include "M68k__EA_72.c"		// Clr (xxxx.w,pc)
#include "M68k__EA_73_Breif.c"	// Clr (d8,PC,Xn.SIZE*SCALE)
#include "M68k__EA_73_Full.c"
#include "M68k__EA_74.c"		// # <xxx>

// --

void M68k_EffectiveAddress( struct M68kStruct *ms )
{
char *outstr;
char *buf;
int mode;
int pos;

	buf = ms->ms_Buf_Argument;
	pos = strlen( buf );

	if ( pos )
	{
		buf[pos++] = ',';
		buf[pos] = 0;
	}

	outstr = & buf[pos];

	mode = (( ms->ms_ArgEMode << 4 ) | ( ms->ms_ArgEReg ));

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
			MODE_00( ms, outstr );
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
			MODE_10( ms, outstr );
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
			MODE_20( ms, outstr );
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
			MODE_30( ms, outstr );
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
			MODE_40( ms, outstr );
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
			MODE_50( ms, outstr );
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
			if ( ms->ms_MemoryBuf[ ms->ms_ArgSize ] & 0x01 )
			{
				// Full Extension Word Format
				if ( ms->ms_MemoryBuf[ ms->ms_ArgSize + 1 ] & 0x08 )
				{
					printf( "%s:%04d: 60_1 : Unsupported EA mode %02x at %08x\n", __FILE__, __LINE__, mode, ms->ms_MemoryAdr );
					ms->ms_DecodeStatus = MSTAT_UnknownCmd;
				}
				else
				{
					MODE_60_Full_0( ms, outstr );
				}
			}
			else
			{
				// Brief Extension Word Format
				MODE_60_Brief( ms, outstr );
			}
			break;
		}

		case 0x70: // 111 000
		{
			// move.l $0004.w
			// ( xxx ).W
			MODE_70( ms, outstr );
			break;
		}

		case 0x71: // 111 001
		{
			// move.l $00000004
			// ( xxx ).L
			MODE_71( ms, outstr );
			break;
		}

		case 0x72: // 111 010
		{
			// Clr (xxxx.w,pc)
			// ( d16, PC )
			MODE_72( ms, outstr );
			break;
		}

		case 0x73: // 110 011
		{
			// ( d8 , PC , Xn.SIZE * SCALE )
			// ( bd , PC , Xn.SIZE * SCALE )
			// ( [ bd , PC ] , Xn.SIZE * SCALE , od )
			// ( [ bd , PC , Xn.SIZE * SCALE ] , od )
			if ( ms->ms_MemoryBuf[ ms->ms_ArgSize ] & 0x01 )
			{
				// Full Extension Word Format
				if ( ms->ms_MemoryBuf[ ms->ms_ArgSize + 1 ] & 0x08 )
				{
					printf( "%s:%04d: 73_1 : Unsupported EA mode $%02x at %08x\n", __FILE__, __LINE__, mode, ms->ms_MemoryAdr );
					ms->ms_DecodeStatus = MSTAT_UnknownCmd;
				}
				else
				{
					MODE_73_Full_0( ms, outstr );
				}
			}
			else
			{
				// Brief Extension Word Format
				MODE_73_Brief( ms, outstr );
			}
			break;
		}

		case 0x74: // 111
		{
			// # <xxx>
			MODE_74( ms, outstr );
			break;
		}

		default:
		{
			printf( "%s:%04d: Unsupported EA mode $%02x at %08x\n", __FILE__, __LINE__, mode, ms->ms_MemoryAdr );
			ms->ms_DecodeStatus = MSTAT_UnknownCmd;
			break;
		}
	}
}
