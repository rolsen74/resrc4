
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

enum RS4DecodeStat AOS3_Exec_FindPort_Func( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
RS4Label *rl;

	ec = RS4ErrStat_Okay;
	ds = RS4DecodeStat_Okay;

	// A1 = String
	if ( rt->rt_CPU.M68k.mt_Registers[ M68KREGT_A1 ].mr_Type1 != RRT_Label )
	{
		goto bailout;
	}

	rl = rt->rt_CPU.M68k.mt_Registers[ M68KREGT_A1 ].mr_Label;

	if ( ! rl )
	{
		goto bailout;
	}

	Mark_NulString( rl );

	// Todo return MsgPort pointer in D0
//	rt->rt_CPU.M68k.mt_ClearRegMask &= ~( 1 << M68KREGT_D0 ); 
//	rt->rt_CPU.M68k.mt_Registers[ M68KREGT_D0 ].mr_Type1 = RRT_Library,
//	rt->rt_CPU.M68k.mt_Registers[ M68KREGT_D0 ].mr_Type2 = myLibs[pos].Type;

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( ds );
}

// --
