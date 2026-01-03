
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
// -- Mode 70 - move.l $0004.w

enum RS4DecodeStat MODE_70( enum RS4ErrorCode *errcode, RS4Trace *rt, STR outstr )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Label *rl;
MEM mem;
S16 val;
S32 pos;

	// --

	ec = RS4ErrStat_Error;
	ds = RS4DecodeStat_Error;

	// --

	mem = rt->rt_CurMemBuf;
	pos = rt->rt_CPU.M68k.mt_ArgSize;

	val = (( mem[pos] << 8 ) | ( mem[pos+1] << 0 ));

	if ( rt->rt_CPU.M68k.mt_DoExternal )
	{
		ERR_CHK( RS4AddExtLabel( & ec, & rl, rt->rt_File, val ))

		if ( rt->rt_Pass != RS4TracePass_Trace )
		{
			if (( rl ) && ( rl->rl_Name[0] ))
			{
				sprintf( outstr, "%s.w", rl->rl_Name );
			}
			else
			{
				sprintf( outstr, "($%04x).w", val );
			}
		}
		else
		{
			outstr[0] = 0;
		}
	}
	else
	{
		if ( rt->rt_Pass != RS4TracePass_Trace )
		{
			if (( rt->rt_CPU.M68k.mt_IsPea ) && ( val < 0 ))
			{
				sprintf( outstr, "(%d).w", val );
			}
			else
			{
				sprintf( outstr, "($%04x).w", val );
			}
		}
		else
		{
			outstr[0] = 0;
		}
	}

	if ( rt->rt_CPU.M68k.mt_CurRegister )
	{
		rt->rt_CPU.M68k.mt_CurRegister->mr_Type1 = RRT_Address;
		rt->rt_CPU.M68k.mt_CurRegister->mr_Address = val;
	}

	rt->rt_CPU.M68k.mt_ArgSize += 2;

	// --

	ec = RS4ErrStat_Okay;
	ds = RS4DecodeStat_Okay;

	// --

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	return( ds );
}

// --
