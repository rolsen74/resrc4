
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
// -- Mode 00 - Clr Dx

enum RS4DecodeStat MODE_00( enum RS4ErrorCode *errcode, RS4Trace *rt, STR outstr )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;

	// --

	ec = RS4ErrStat_Okay;
	ds = RS4DecodeStat_Okay;

	// --

	if ( rt->rt_Pass != RS4TracePass_Trace )
	{
		sprintf( outstr, "%s", Dx_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ] );
	}
	else
	{
		outstr[0] = 0;
	}

	rt->rt_CPU.M68k.mt_CurRegister = & rt->rt_CPU.M68k.mt_Registers[ M68KREGT_Dx + rt->rt_CPU.M68k.mt_ArgEReg ];

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( ds );
}

// --
