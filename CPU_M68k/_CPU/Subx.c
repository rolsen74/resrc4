
/*
 * Copyright (c) 2014-2025 Rene W. Olsen < renewolsen @ gmail . com >
 *
 * This software is released under the GNU General Public License, version 3.
 * For the full text of the license, please visit:
 * https://www.gnu.org/licenses/gpl-3.0.html
 *
 * You can also find a copy of the license in the LICENSE file included with this software.
 */

// --

#include "Resourcer/ReSrc4.h"

// --

enum RS4DecodeStat M68kCmd_SUBX( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
int size;
int rm;
int rx;
int ry;

	ry		= ( rt->rt_CPU.M68k.mt_Opcode & 0x0e000000 ) >> 25;
	size	= ( rt->rt_CPU.M68k.mt_Opcode & 0x00c00000 ) >> 22;
	rm		= ( rt->rt_CPU.M68k.mt_Opcode & 0x00080000 );
	rx		= ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;

	switch( size )
	{
		case 0: // Subx.b
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Subx.b";
			rt->rt_CPU.M68k.mt_ArgType  = M68KSIZE_Byte;
			break;
		}

		case 1: // Subx.w
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Subx.w";
			rt->rt_CPU.M68k.mt_ArgType  = M68KSIZE_Word;
			break;
		}

		case 2: // Subx.l
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Subx.l";
			rt->rt_CPU.M68k.mt_ArgType  = M68KSIZE_Long;
			break;
		}

		default:
		{
			printf( "Unsupported 'Subx' Opcode (Size: %d)\n", size );
			ds = RS4DecodeStat_Error;
			goto bailout;
		}
	}

	if ( rm )
	{
		sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "-(%s),-(%s)", Ax_RegNames[rx], Ax_RegNames[ry] );
	}
	else
	{
		sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "%s,%s", Dx_RegNames[rx], Dx_RegNames[ry] );
	}

	// --

	ds = RS4DecodeStat_Okay;
	ec = RS4ErrStat_Okay;

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	return( ds );
}

// --
