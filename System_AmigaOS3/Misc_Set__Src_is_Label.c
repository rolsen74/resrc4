
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

enum RS4ErrorCode Misc_Set__Src_is_Label( 
	struct AmigaOS3_Misc_Move_GetSetStruct *gss,
	struct M68kRegister *reg, 
	RS4Trace *rt,
	MEM mem )
{
struct M68kRegister *dst_reg;
struct M68kRegister *mr;
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Label *src_rl;
RS4Label *dst_rl;

	ec		= RS4ErrStat_Error;
	src_rl	= gss->Label;
	dst_rl	= NULL;
	dst_reg	= NULL;

	#ifdef DEBUG
	if ( ! src_rl )
	{
		ec = RS4ErrStat_Internal;
		printf( "%s:%04d: Error - NULL Pointer\n", __FILE__, __LINE__ );
		goto bailout;
	}
	#endif

	// --

	// Dst is Reg : Dx
	/**/ if ( rt->rt_CPU.M68k.mt_ArgEMode == 0 )
	{
		// Label -> Dx

		dst_reg = reg;

		DDEBUG( printf( "write : 23.1.1 : Label -> Dx : Reg %p\n", reg ); )
	}

	// Dst is Reg : Ax
	else if ( rt->rt_CPU.M68k.mt_ArgEMode == 1 )
	{
		// Label -> Ax

		dst_reg = reg;

		DDEBUG( printf( "write : 23.2.1 : Label -> Ax : Reg %p\n", reg ); )
	}

	// Dst is Addr : (xxx,Ax)
	else if ( rt->rt_CPU.M68k.mt_ArgEMode == 5 )
	{
		mr = & rt->rt_CPU.M68k.mt_Registers[ M68KREGT_Ax + rt->rt_CPU.M68k.mt_ArgEReg ];

		if (( mr->mr_Type1 == RRT_Label ) && ( mr->mr_Label ))
		{
			S16 off = (( mem[0] << 8 ) | ( mem[1] << 0 ));
			S64 adr = mr->mr_Label->rl_Address + off;

			ERR_CHK( RS4FindLabel_File( & ec, rt->rt_File, & dst_rl, adr, __FILE__ ))
		}

		DDEBUG( printf( "write : 23.3.1 : Label -> (xxx,Ax) : dst_rl %p\n", dst_rl ); )
	}

	// Dst is Addr : $00000004.l
	else if (( rt->rt_CPU.M68k.mt_ArgEMode == 7 ) && ( rt->rt_CPU.M68k.mt_ArgEReg == 1 ))	
	{
		U32 val = (( mem[0] << 24 ) | ( mem[1] << 16 ) | ( mem[2] << 8 ) | ( mem[3] << 0 ));

		ERR_CHK( RS4FindLabel_File( & ec, rt->rt_File, & dst_rl, val, __FILE__ ))

		DDEBUG( printf( "write : 23.4.1 : Label -> $xxxxxxxx.l : dst_rl %p\n", dst_rl ); )
	}

	// Dst is Unknown : Unsupported
	else
	{
		DDEBUG( printf( "write : 23.5.1 : Label -> ???\n" ); )
		// Do Nothing
	}

	// -- Update Label -> Label

	/**/ if ( dst_rl )
	{
		DDEBUG( printf( "write : 24.1.1 : Label -> Label :\n" ); )

		while( TRUE )
		{
			// Label -> Label

			if ( dst_rl->rl_UserLocked )
			{
				DDEBUG( printf( "write : 24.1.2 : Label -> Label : User locked\n" ); )
				break;
			}

			if (( src_rl->rl_Type1 == RS4LabelType_Unset )
			&&	( dst_rl->rl_Type1 == RS4LabelType_Unset ))
			{
				// Both Labels have unset types, all okay
				DDEBUG( printf( "write : 24.1.3 : Label -> Label : Both unset\n" ); )
				break;
			}

			if (( src_rl->rl_Type1 == dst_rl->rl_Type1 )
			&&	( src_rl->rl_Type2 == dst_rl->rl_Type2 )
			&&	( src_rl->rl_Type3 == dst_rl->rl_Type3 ))
			{
				// Same type, all okay
				DDEBUG( printf( "write : 24.1.4 : Label -> Label : Same type : Src %d %d %d : Dst %d %d %d\n", 
					src_rl->rl_Type1, src_rl->rl_Type2, src_rl->rl_Type3,
					dst_rl->rl_Type1, dst_rl->rl_Type2, dst_rl->rl_Type3 ); )
				break;
			}

			if ( dst_rl->rl_Type1 == RS4LabelType_Unset )
			{
				// Copy Src -> Dst
				DDEBUG( printf( "write : 24.1.5 : Label -> Label : Copy\n" ); )
				dst_rl->rl_Type1 = src_rl->rl_Type1;
				dst_rl->rl_Type2 = src_rl->rl_Type2;
				dst_rl->rl_Type3 = src_rl->rl_Type3;
			}
			else
			{
				// We have two diffrent types, set it to unknown
				DDEBUG( printf( "write : 24.1.6 : Label -> Label : Unknown\n" ); )
				dst_rl->rl_Type1 = RS4LabelType_Unknown;
				dst_rl->rl_Type2 = 0;
				dst_rl->rl_Type3 = 0;
			}

			break;
		}
	}

	// -- Update Label -> Reg

	else if ( dst_reg )
	{
		DDEBUG( printf( "write : 24.2.1 : Label -> Reg :\n" ); )

		// Default
		dst_reg->mr_Type1 = RRT_Unknown;

		switch( src_rl->rl_Type1 )
		{
			case RS4LabelType_Pointer:
			{
				DDEBUG( printf( "write : 24.2.1 : Label -> Reg : Pointer :\n" ); )

				switch( src_rl->rl_Type2 )
				{
					case RS4LabelPtrType_Library:
					{
						DDEBUG( printf( "write : 24.2.2 : Label -> Reg : Pointer : Library :\n" ); )

						dst_reg->mr_Type1	= RRT_Library;
						dst_reg->mr_Type2	= src_rl->rl_Type3;
						dst_reg->mr_Address = 0;
						dst_reg->mr_Label	= NULL;
						break;
					}

					default:
					{
						DDEBUG( printf( "write : 24.2.3 : Label -> Reg : Pointer : ?? :\n" ); )
						break;
					}
				}
				break;
			}

			case RS4LabelType_Unset:
			{
				DDEBUG( printf( "write : 24.3.1 : Label -> Reg : Pointer :\n" ); )

				dst_reg->mr_Type1	= RRT_Label;
				dst_reg->mr_Type2	= 0;
				dst_reg->mr_Address = 0;
				dst_reg->mr_Label	= src_rl;
				break;
			}

			default:
			{
				DDEBUG( printf( "write : 24.4.1 : Label -> Reg : Type1 #%d : ?? :\n", src_rl->rl_Type1 ); )
				break;
			}
		}
	}

	// -- Update Label -> ??

	else
	{
		DDEBUG( printf( "write : 24.5.1 : Label -> ??? :\n" ); )
	}

	// --

	ec = RS4ErrStat_Okay;

bailout:

	return( ec );
}

// --
