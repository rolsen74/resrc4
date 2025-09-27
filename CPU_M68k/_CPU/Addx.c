
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

enum RS4DecodeStat M68kCmd_ADDX( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
S32 size;
S32 rm;
S32 rx;
S32 ry;

	rx		= ( rt->rt_CPU.M68k.mt_Opcode & 0x0e000000 ) >> 25;
	size	= ( rt->rt_CPU.M68k.mt_Opcode & 0x00c00000 ) >> 22;
	rm		= ( rt->rt_CPU.M68k.mt_Opcode & 0x00080000 );
	ry		= ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;

	switch( size )
	{
		case 0: // Addx.b
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Addx.b";
			rt->rt_CPU.M68k.mt_ArgType  = M68KSIZE_Byte;
			break;
		}

		case 1: // Addx.w
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Addx.w";
			rt->rt_CPU.M68k.mt_ArgType  = M68KSIZE_Word;
			break;
		}

		case 2: // Addx.l
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Addx.l";
			rt->rt_CPU.M68k.mt_ArgType  = M68KSIZE_Long;
			break;
		}

		default:
		{
			printf( "Unsupported 'Addx' Opcode (Size: %d)\n", size );
			ds = RS4DecodeStat_Error;
			goto bailout;
		}
	}

	if ( rm )
	{
		sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "-(%s),-(%s)", Ax_RegNames[ry], Ax_RegNames[rx] );
	}
	else
	{
		sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "%s,%s", Dx_RegNames[ry], Dx_RegNames[rx] );
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
