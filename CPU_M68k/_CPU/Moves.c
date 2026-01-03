
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

enum RS4DecodeStat M68kCmd_MOVES( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
CSTR rname;
S32 size;
S32 reg;
S32 pos;
S32 ad;
S32 dr;

	size	= ( rt->rt_CPU.M68k.mt_Opcode & 0x00c00000 ) >> 22;
	ad		= ( rt->rt_CPU.M68k.mt_Opcode & 0x00008000 ) >> 15;
	reg		= ( rt->rt_CPU.M68k.mt_Opcode & 0x00007000 ) >> 12;
	dr		= ( rt->rt_CPU.M68k.mt_Opcode & 0x00000800 ) >> 11;

	switch( size )
	{
		case 0:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Moves.b";
			rt->rt_CPU.M68k.mt_ArgType = M68KSIZE_Byte;
			break;
		}

		case 1:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Moves.w";
			rt->rt_CPU.M68k.mt_ArgType = M68KSIZE_Word;
			break;
		}

		case 2:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Moves.l";
			rt->rt_CPU.M68k.mt_ArgType = M68KSIZE_Long;
			break;
		}

		default:
		{
			printf( "Unsupported 'Moves' Opcode (Mode: %d)\n", size );
			ds = RS4DecodeStat_Error;
			goto bailout;
		}
	}

	rname = ( ad ) ? Ax_RegNames[reg] : Dx_RegNames[reg] ;

	rt->rt_CPU.M68k.mt_ArgEMode	= ( rt->rt_CPU.M68k.mt_Opcode & 0x00380000 ) >> 19;
	rt->rt_CPU.M68k.mt_ArgEReg	= ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;

	rt->rt_CPU.M68k.mt_CurRegister = & rt->rt_CPU.M68k.mt_DstRegister;
	rt->rt_CPU.M68k.mt_ArgSize = 4;

	if ( dr )
	{
		sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "%s", rname );

		EA_CHK( M68k_EffectiveAddress( & ec, rt ))
	}
	else
	{
		EA_CHK( M68k_EffectiveAddress( & ec, rt ))

		pos = strlen( rt->rt_Container.Hunk.ms_Buf_Argument );

		sprintf( & rt->rt_Container.Hunk.ms_Buf_Argument[pos], ",%s", rname );
	}

	rt->rt_CPU.M68k.mt_OpcodeSize = rt->rt_CPU.M68k.mt_ArgSize;

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
