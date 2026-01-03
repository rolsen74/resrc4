
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
// -- Mode 50 - jsr (d16,Ax)

enum RS4DecodeStat MODE_50( enum RS4ErrorCode *errcode, RS4Trace *rt, STR outstr )
{
struct M68kRegister *mr;
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Label *rl;
STR lvostr;
S64 adr;
S32 pos;
MEM mem;
S16 val;

	// --

	ec = RS4ErrStat_Error;
	ds = RS4DecodeStat_Error;

	// --

	mem = rt->rt_CurMemBuf;
	pos = rt->rt_CPU.M68k.mt_ArgSize;

	val = (( mem[pos] << 8 ) | ( mem[pos+1] << 0 ));

	lvostr = NULL;

	// --
	// Check Reg Type

	mr = & rt->rt_CPU.M68k.mt_Registers[ M68KREGT_Ax + rt->rt_CPU.M68k.mt_ArgEReg ];

	/**/ if ( mr->mr_Type1 == RRT_Label )
	{
		rl = mr->mr_Label;

		if ( rl )
		{
			adr = rl->rl_Address + val;

			RS4FileSection *sec;
			
			ERR_CHK( RS4FindSection_File( & ec, & sec, rt->rt_File, adr ))

			if ( sec )
			{
				ERR_CHK( RS4AddLabel_Sec( NULL, & rl, sec, adr, RS4LabelType_Unset ))
			}
			else
			{
				rl = NULL;
			}

			if ( rl )
			{
				rt->rt_CPU.M68k.mt_SrcRegister.mr_Type1 = RRT_Label;
				rt->rt_CPU.M68k.mt_SrcRegister.mr_Label = rl;
			}
			else
			{
				rt->rt_CPU.M68k.mt_SrcRegister.mr_Type1 = RRT_Address;
				rt->rt_CPU.M68k.mt_SrcRegister.mr_Address = adr;
			}
		}
	}

	// --

	else if ( mr->mr_Type1 == RRT_Library )
	{
		if ( rt->rt_CPU.M68k.mt_LibCall )
		{
			lvostr = AmigaOS3_FindLibFunc( & ec, rt, val, __FILE__ );

			if (( ! lvostr ) && ( ec != RS4ErrStat_Okay ))
			{
				// ec allready set
				ds = RS4DecodeStat_Error;

				#ifdef DEBUG
				printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
				#endif

				goto bailout;
			}
		}
	}

	// --

	if ( rt->rt_Pass != RS4TracePass_Trace )
	{
		if ( lvostr )
		{
			sprintf( outstr, "(%s,%s)", lvostr, Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ] );
		}
		else
		{
			// Most proberly a Library call, so we use Decimal
			sprintf( outstr, "(%d,%s)", val, Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ] );
		}
	}
	else
	{
		outstr[0] = 0;
	}

	rt->rt_CPU.M68k.mt_ArgSize += 2;

	// --

	ec = RS4ErrStat_Okay;
	ds = RS4DecodeStat_Okay;

	// --

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( ds );
}

// --
