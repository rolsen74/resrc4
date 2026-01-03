
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

enum RS4DecodeStat M68kCmd_LEA( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
struct M68kRegister *src;
// struct M68kRegister *mr;
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
// RS4FileSection *sec;
// RS4Label *rl;
S32 src_emode;
S32 src_ereg;

	rt->rt_Container.Hunk.ms_Str_Opcode = "Lea";

	// --

	src_emode = ( rt->rt_CPU.M68k.mt_Opcode & 0x00380000 ) >> 19;
	src_ereg  = ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;

	rt->rt_CPU.M68k.mt_ArgType		= M68KSIZE_Long;
	rt->rt_CPU.M68k.mt_ArgEMode		= src_emode;
	rt->rt_CPU.M68k.mt_ArgEReg		= src_ereg;
	rt->rt_CPU.M68k.mt_CurRegister	= & rt->rt_CPU.M68k.mt_SrcRegister;
	rt->rt_CPU.M68k.mt_DoLabelSize	= FALSE;

	EA_CHK( M68k_EffectiveAddress( & ec, rt ))

	src = rt->rt_CPU.M68k.mt_CurRegister;

	// --

	rt->rt_CPU.M68k.mt_ArgEMode		= 0x01; // Ax Reg
	rt->rt_CPU.M68k.mt_ArgEReg		= ( rt->rt_CPU.M68k.mt_Opcode & 0x0e000000 ) >> 25;
	rt->rt_CPU.M68k.mt_CurRegister	= & rt->rt_CPU.M68k.mt_DstRegister;

	EA_CHK( M68k_EffectiveAddress( & ec, rt ))

	memcpy( rt->rt_CPU.M68k.mt_CurRegister, src, sizeof( struct M68kRegister ));
	rt->rt_CPU.M68k.mt_Registers[ M68KREGT_Ax + rt->rt_CPU.M68k.mt_ArgEReg ] = rt->rt_CPU.M68k.mt_SrcRegister;
	rt->rt_CPU.M68k.mt_OpcodeSize = rt->rt_CPU.M68k.mt_ArgSize;

	// --

	ds = RS4DecodeStat_Okay;
	ec = RS4ErrStat_Okay;

	// --

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

//	leatst = 0;

	return( ds );
}
