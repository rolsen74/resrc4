
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

enum RS4DecodeStat M68kCmd_JSR( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;

	rt->rt_Container.Hunk.ms_Str_Opcode = "Jsr";

	// --

	rt->rt_CPU.M68k.mt_DoLabelSize	= FALSE;
	rt->rt_CPU.M68k.mt_ArgType 		= M68KSIZE_Long;
	rt->rt_CPU.M68k.mt_ArgEMode		= ( rt->rt_CPU.M68k.mt_Opcode & 0x00380000 ) >> 19;
	rt->rt_CPU.M68k.mt_ArgEReg		= ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;
	rt->rt_CPU.M68k.mt_CurRegister	= & rt->rt_CPU.M68k.mt_JmpRegister;
	rt->rt_CPU.M68k.mt_LibCall		= TRUE;

	if (( rt->rt_CPU.M68k.mt_ArgEMode == 5 ) && ( rt->rt_CPU.M68k.mt_ArgEReg == 6 ))
	{
		// (d16,A6)
		// Clear Only D0-D1 and A0-A1
		rt->rt_CPU.M68k.mt_ClearRegMask = ( 1 << M68KREGT_D0 ) | ( 1 << M68KREGT_D1 ) | ( 1 << M68KREGT_A0 ) | ( 1 << M68KREGT_A1 );
	}
	else
	{
		// Clear all Regs
		rt->rt_CPU.M68k.mt_ClearRegMask = -1;
	}

	EA_CHK( M68k_EffectiveAddress( & ec, rt ))

	// --

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
