
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

enum RS4FuncStat AmigaOS3_FindLibFunc_Regs( enum RS4ErrorCode *errcode, RS4Trace *rt, struct _AOS3_RegStruct *rs )
{
struct _AOS3_RegStruct *qregs;
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Label *rl;
S32 reg;
S32 pos;

//	printf( "AmigaOS3_FindLibFunc_Regs : \n" );

	fs = RS4FuncStat_Error;
	ec = RS4ErrStat_Okay;

	// --

	if ( ! rs )
	{
		ec = RS4ErrStat_Internal;
		printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
		goto bailout;
	}

	// --

	for( pos=0 ; TRUE ; pos++ )
	{
		qregs = & rs[pos];

		reg = qregs->Reg3;

		// Validate Reg

		if (( reg < M68KREGT_D0 ) || ( M68KREGT_A7 < reg ))
		{
			break;
		}

		switch( qregs->Type3 )
		{
			case AOS3_RegType_Code:
			{
//				printf( "AOS3_LVOType_Regs : AOS3_RegType_Code\n" );

				if ( rt->rt_CPU.M68k.mt_Registers[ reg ].mr_Type1 != RRT_Label )
				{
					break;
				}

				rl = rt->rt_CPU.M68k.mt_Registers[ reg ].mr_Label;

				if ( ! rl )
				{
					break;
				}

				if ( rl->rl_Type1 == RS4LabelType_Pointer )
				{
					Mark_Code( rl );
				}
				else
				{
					if ( DoVerbose > 2 )
					{
						printf( "%s:%04d: hmm 1 : Type1 %d : Type2 %d : Type3 %d :\n", __FILE__, __LINE__, rl->rl_Type1, rl->rl_Type2, rl->rl_Type3 );
					}
				}
				break;
			}

			case AOS3_RegType_String:
			{
//				printf( "AOS3_LVOType_Regs : AOS3_RegType_String\n" );

				if ( rt->rt_CPU.M68k.mt_Registers[ reg ].mr_Type1 != RRT_Label )
				{
					break;
				}

				rl = rt->rt_CPU.M68k.mt_Registers[ reg ].mr_Label;

				if ( ! rl )
				{
					break;
				}

				if (( rl->rl_Type1 == RS4LabelType_Unset )
				||	( rl->rl_Type1 == RS4LabelType_String ))
				{
					Mark_NulString( rl );
				}
				else
				{
					if ( DoVerbose > 2 )
					{
						printf( "%s:%04d: hmm 2 : Type1 %d : Type2 %d : Type3 %d :\n", __FILE__, __LINE__, rl->rl_Type1, rl->rl_Type2, rl->rl_Type3 );
					}
				}
				break;
			}

			case AOS3_RegType_Struct:
			{
//				printf( "AOS3_LVOType_Regs : AOS3_RegType_Struct\n" );

				if ( rt->rt_CPU.M68k.mt_Registers[ reg ].mr_Type1 != RRT_Label )
				{
					break;
				}

				rl = rt->rt_CPU.M68k.mt_Registers[ reg ].mr_Label;

				if ( ! rl )
				{
					break;
				}

				if (( rl->rl_Type1 == RS4LabelType_Unset )
				||	( rl->rl_Type1 == RS4LabelType_Struct ))
				{
					fs = Mark_Struct( & ec, rl, qregs->Val3, __FILE__ );
				}
				else
				{
					if ( DoVerbose > 2 )
					{
						printf( "%s:%04d: hmm 3 : Type1 %d : Type2 %d : Type3 %d :\n", __FILE__, __LINE__, rl->rl_Type1, rl->rl_Type2, rl->rl_Type3 );
					}
				}
				break;
			}

			default:
			{
				printf( "%s:%04d: AOS3_LVOType_Regs : Internal Error : Type %d\n", __FILE__, __LINE__, qregs->Type3 );
				ec = RS4ErrStat_Internal;
				goto bailout;
			}
		}
	}

	// --

	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fs );
}

// --
