
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

#define DDEBUG(x)

// --

enum RS4ErrorCode Misc_Set__Src_is_Reg( 
	struct AmigaOS3_Misc_Move_GetSetStruct *gss,
	struct M68kRegister *dst_reg, 
	RS4Trace *rt,
	MEM mem )
{
struct M68kRegister *src_reg;
struct M68kRegister *mr;
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Label *src_rl;
RS4Label *dst_rl;

	ec = RS4ErrStat_Error;

	src_reg = & gss->Reg;

	dst_rl = NULL;

	// Dst is Reg : Dx
	/**/ if ( rt->rt_CPU.M68k.mt_ArgEMode == 0 )
	{
		// Reg -> Reg

		DDEBUG( printf( "write : 35.1.1 : Reg -> Reg\n" ); )

		*dst_reg = gss->Reg;
	}

	// Dst is Reg : Ax
	else if ( rt->rt_CPU.M68k.mt_ArgEMode == 1 )
	{
		// Reg -> Reg

		DDEBUG( printf( "write : 35.2.1 : Reg -> Reg\n" ); )

		*dst_reg = gss->Reg;
	}

	// Dst is (xxx,Ax)
	else if ( rt->rt_CPU.M68k.mt_ArgEMode == 5 )
	{
		// Reg -> (xxx,Ax)

		mr = & rt->rt_CPU.M68k.mt_Registers[ M68KREGT_Ax + rt->rt_CPU.M68k.mt_ArgEReg ];

		if (( mr->mr_Type1 == RRT_Label ) && ( mr->mr_Label ))
		{
			S16 off = (( mem[0] << 8 ) | ( mem[1] << 0 ));
			S64 adr = mr->mr_Label->rl_Address + off;

			ERR_CHK( RS4FindLabel_File( & ec, rt->rt_File, & dst_rl, adr, __FILE__ ))
		}

		DDEBUG( printf( "write : 35.3.1 : Reg -> Label : dst_rl %p\n", dst_rl ); )
	}

	// Dst is Addr : $00000004.l
	else if (( rt->rt_CPU.M68k.mt_ArgEMode == 7 ) && ( rt->rt_CPU.M68k.mt_ArgEReg == 1 ))	
	{
		// Reg -> xxx.l

		U32 val = (( mem[0] << 24 ) | ( mem[1] << 16 ) | ( mem[2] << 8 ) | ( mem[3] << 0 ));

		ERR_CHK( RS4FindLabel_File( & ec, rt->rt_File, & dst_rl, val, __FILE__ ))

		DDEBUG( printf( "write : 35.4.1 : Reg -> Label : dst_rl %p\n", dst_rl ); )
	}

	// Dst is Unknown : Unsupported
	else
	{
		DDEBUG( printf( "write : 35.5.1 : Reg -> ?? :\n" ); )

		// Do Nothing
	}

	// -- Update Label

	if (( dst_rl ) && ( ! dst_rl->rl_UserLocked ))
	{
		switch( src_reg->mr_Type1 )
		{
			case RRT_Unknown:
			{
				DDEBUG( printf( "write : 36.1.1 : Reg (Unknown)\n" ); )

				// Check if Label Type have been set
				// if yes, then its used for more than one type
				// and we have to set Unknown
				if ( dst_rl->rl_Type1 != RS4LabelType_Unset )
				{
					dst_rl->rl_Type1 = RS4LabelType_Unknown;
					dst_rl->rl_Type2 = 0;
					dst_rl->rl_Type3 = 0;
				}
				break;
			}

			case RRT_Label:
			{
				// Label -> Label

				src_rl = src_reg->mr_Label;

				if (( src_rl->rl_Type1 == RS4LabelType_Unset )
				&&	( dst_rl->rl_Type1 == RS4LabelType_Unset ))
				{
					// Both Labels have unset types, all okay
					DDEBUG( printf( "write : 36.2.1 : Reg (Label) -> Label : Both unset\n" ); )
					break;
				}

				if (( src_rl->rl_Type1 == dst_rl->rl_Type1 )
				&&	( src_rl->rl_Type2 == dst_rl->rl_Type2 ))
				{
					// Same type, all okay
					DDEBUG( printf( "write : 36.2.2 : Reg (Label) -> Label : Same type\n" ); )
					break;
				}

				if ( dst_rl->rl_Type1 == RS4LabelType_Unset )
				{
					dst_rl->rl_Type1 = src_rl->rl_Type1;
					dst_rl->rl_Type2 = src_rl->rl_Type2;
					dst_rl->rl_Type3 = src_rl->rl_Type3;
					DDEBUG( printf( "write : 36.2.3 : Reg (Label) -> Label : Copy\n" ); )
				}
				else
				{
					// We have two diffrent types, set it to unknown
					dst_rl->rl_Type1 = RS4LabelType_Unknown;
					dst_rl->rl_Type2 = 0;
					dst_rl->rl_Type3 = 0;
					DDEBUG( printf( "write : 36.2.4 : Reg (Label) -> Label : Unknown\n" ); )
				}
				break;
			}

			case RRT_Library:
			{
				// src Reg is a Library
				// Reg -> Label

				if ( dst_rl->rl_Type1 == RS4LabelType_Unset )
				{
					// Set Label Type
					dst_rl->rl_Type1 = RS4LabelType_Pointer;
					dst_rl->rl_Type2 = RS4LabelPtrType_Library;
					dst_rl->rl_Type3 = src_reg->mr_Type2;
					DDEBUG( printf( "write : 36.3.1 : Reg (Library) -> Label : Copy\n" ); )
					break;
				}

				if (( dst_rl->rl_Type1 == RS4LabelType_Pointer )
				&&	( dst_rl->rl_Type2 == RS4LabelPtrType_Library )
				&&	( dst_rl->rl_Type3 == src_reg->mr_Type2 ))
				{
					// Same type, all okay
					DDEBUG( printf( "write : 36.3.2 : Reg (Library) -> Label : Same\n" ); )
					break;
				}

				// We have two diffrent types, set it to unknown
				dst_rl->rl_Type1 = RS4LabelType_Unknown;
				dst_rl->rl_Type2 = 0;
				dst_rl->rl_Type3 = 0;
				DDEBUG( printf( "write : 36.3.3 : Reg (Library) -> Label : Unknown\n" ); )
				break;
			}

			default:
			{
				DDEBUG( printf( "write : 36.4.1 : Reg (Unknown)\n" ); )
				break;
			}
		}
	}

	ec = RS4ErrStat_Okay;

bailout:

	return( ec );
}

// --
