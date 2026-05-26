
/*
** Copyright (c) 2014-2026 Rene W. Olsen
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

enum RS4DecodeStat
M68kCmd_BFINS ( enum RS4ErrorCode * errcode, RS4Trace * rt )
{
	enum RS4DecodeStat ds;
	enum RS4ErrorCode  ec;

	U32 offset;
	U32 width;
	STR args;
	U32 reg;
	U32 len;

	rt->rt_Container.Hunk.ms_Str_Opcode = "BFIns";

	// --
	// Src Dx Reg
	reg = ( rt->rt_CPU.M68k.mt_Opcode & 0x00007000 ) >> 12;

	args = rt->rt_Container.Hunk.ms_Buf_Argument;
	sprintf ( args, "%s", Dx_RegNames[reg] );

	// --
	// EA arg

	rt->rt_CPU.M68k.mt_ArgSize	= 4;
	rt->rt_CPU.M68k.mt_ArgType	= M68KSIZE_Unsized;
	rt->rt_CPU.M68k.mt_ArgEMode = ( rt->rt_CPU.M68k.mt_Opcode & 0x00380000 ) >> 19;
	rt->rt_CPU.M68k.mt_ArgEReg	= ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;

	EA_CHK ( M68k_EffectiveAddress ( &ec, rt ) )

	len = strlen ( args );

	// --
	// Check 'Do' bit
	if ( rt->rt_CPU.M68k.mt_Opcode & 0x00000800 )
	{
		reg = ( rt->rt_CPU.M68k.mt_Opcode & 0x000001c0 ) >> 6;

		sprintf ( &args[len], "{%s:", Dx_RegNames[reg] );
	}
	else
	{
		offset = ( rt->rt_CPU.M68k.mt_Opcode & 0x000007c0 ) >> 6;

		sprintf ( &args[len], "{%" PRIu32 ":", offset );
	}

	len = strlen ( args );

	// --
	// Check 'Dw' bit
	if ( rt->rt_CPU.M68k.mt_Opcode & 0x00000020 )
	{
		reg = ( rt->rt_CPU.M68k.mt_Opcode & 0x00000007 );

		sprintf ( &args[len], "%s}", Dx_RegNames[reg] );
	}
	else
	{
		width = ( rt->rt_CPU.M68k.mt_Opcode & 0x0000001f );

		sprintf ( &args[len], "%" PRIu32 "}", width );
	}

	// --

	ds = RS4DecodeStat_Okay;
	ec = RS4ErrStat_Okay;

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	return ( ds );
}
