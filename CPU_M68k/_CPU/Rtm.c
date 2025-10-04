
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

enum RS4DecodeStat M68kCmd_RTM( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
S32 reg;
S32 da;

	// --

	rt->rt_Container.Hunk.ms_Str_Opcode = "Rtm";
	rt->rt_CPU.M68k.mt_LastOpcode = TRUE;

	reg = ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;
	da  = ( rt->rt_CPU.M68k.mt_Opcode & 0x00080000 );

	if ( da )
	{
		sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "%s", Ax_RegNames[ reg ] );
	}
	else
	{
		sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "%s", Dx_RegNames[ reg ] );
	}

	// --

	ds = RS4DecodeStat_Okay;
	ec = RS4ErrStat_Okay;

	// --

//bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( ds );
}
