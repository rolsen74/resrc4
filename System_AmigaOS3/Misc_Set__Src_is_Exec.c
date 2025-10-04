
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

enum RS4ErrorCode Misc_Set__Src_is_Exec( 
	struct M68kRegister *reg, 
	RS4Trace *rt,
	MEM mem )
{
struct M68kRegister *dst_reg;
struct M68kRegister *mr;
enum RS4ErrorCode ec;
RS4Label *dst_rl;

	ec = RS4ErrStat_Error;

	dst_reg = NULL;
	dst_rl = NULL;

	// Dst is Reg : Dx
	/**/ if ( rt->rt_CPU.M68k.mt_ArgEMode == 0 )
	{
		DDEBUG( printf( "write : 11.1.1 : Exec -> Dx Reg\n" ); )

		dst_reg = reg;
	}

	// Dst is Reg : Ax
	else if ( rt->rt_CPU.M68k.mt_ArgEMode == 1 )
	{
		DDEBUG( printf( "write : 11.2.1 : Exec -> Ax Reg\n" ); )

		dst_reg = reg;
	}

	// Dst is Addr : $00000004.l
	else if (( rt->rt_CPU.M68k.mt_ArgEMode == 7 ) && ( rt->rt_CPU.M68k.mt_ArgEReg == 1 ))	
	{
		DDEBUG( printf( "write : 11.3.1 :\n" ); )

		U32 val = (( mem[0] << 24 ) | ( mem[1] << 16 ) | ( mem[2] << 8 ) | ( mem[3] << 0 ));

		dst_rl = RS4FindLabel_File( rt->rt_File, val, __FILE__ );
	}

	// Dst is (xxx,Ax)
	else if ( rt->rt_CPU.M68k.mt_ArgEMode == 5 )
	{
		DDEBUG( printf( "write : 11.4.1 :\n" ); )

		mr = & rt->rt_CPU.M68k.mt_Registers[ M68KREGT_Ax + rt->rt_CPU.M68k.mt_ArgEReg ];

		if (( mr->mr_Type1 == RRT_Label ) && ( mr->mr_Label ))
		{
			S16 off = (( mem[0] << 8 ) | ( mem[1] << 0 ));
			S64 adr = mr->mr_Label->rl_Address + off;

			dst_rl = RS4FindLabel_File( rt->rt_File, adr, __FILE__ );
		}
	}

	// Dst is Unknown : Unsupported
	else
	{
		DDEBUG( printf( "write : 11.5.1 : Exec -> ??\n" ); )

		// Do Nothing
	}



	// -- Update Label

	/**/ if ( dst_rl )
	{
		DDEBUG( printf( "write : 16.1.1 : Exec -> Label : dst_rl %p\n", dst_rl ); )

		while( TRUE )
		{
			// Label -> Label

			if ( dst_rl->rl_UserLocked )
			{
				DDEBUG( printf( "write : 16.2.1 : Exec -> Label : User locked\n" ); )
				break;
			}

			if (( dst_rl->rl_Type1 == RS4LabelType_Pointer )
			&&	( dst_rl->rl_Type2 == RS4LabelPtrType_Library )
			&&	( dst_rl->rl_Type3 == AOS3_LibType_ExecBase ))
			{
				// Same type, all okay
				DDEBUG( printf( "write : 16.2.2 : Exec -> Label : Same type\n" ); )
				break;
			}

			if ( dst_rl->rl_Type1 == RS4LabelType_Unset )
			{
				// Set Exec -> Dst
				DDEBUG( printf( "write : 16.2.3 : Exec -> Label : Set\n" ); )
				dst_rl->rl_Type1 = RS4LabelType_Pointer;
				dst_rl->rl_Type2 = RS4LabelPtrType_Library;
				dst_rl->rl_Type3 = AOS3_LibType_ExecBase;
			}
			else
			{
				// We have two diffrent types, set it to unknown
				DDEBUG( printf( "write : 16.2.4 : Exec -> Label : Unknown\n" ); )
				dst_rl->rl_Type1 = RS4LabelType_Unknown;
				dst_rl->rl_Type2 = 0;
				dst_rl->rl_Type3 = 0;
			}
			break;
		}

	}

	// -- Update Reg

	else if ( dst_reg )
	{
		DDEBUG( printf( "write : 16.3.1 : Exec -> Reg : dst_reg %p\n", dst_reg ); )

		// Exec -> Reg
		dst_reg->mr_Type1	= RRT_Library;
		dst_reg->mr_Type2	= AOS3_LibType_ExecBase;
		dst_reg->mr_Label	= NULL;
		dst_reg->mr_Address	= 0;
	}

	// --

	else
	{
		DDEBUG( printf( "write : 16.4.1 : Exec -> ?? : Unknown\n" ); )

		// Do Nothing
	}

	// --

	ec = RS4ErrStat_Okay;

// bailout:

	return( ec );
}

// --
