
/*
 * Copyright (c) 2014-2025 Rene W. Olsen < renewolsen @ gmail . com >
 *
 * This software is released under the GNU General Public License, version 3.
 * For the full text of the license, please visit:
 * https://www.gnu.org/licenses/gpl-3.0.html
 *
 * You can also find a copy of the license in the LICENSE file included with this software.
 */

// --

#include "Resourcer/ReSrc4.h"

// --

#define DDEBUG(x)

// --

enum RS4ErrorCode Misc_Set__Src_is_Label( 
	struct AmigaOS3_Misc_Move_GetSetStruct *gss,
	struct M68kRegister *dst_reg, 
	RS4Trace *rt,
	MEM mem )
{
enum RS4ErrorCode ec;
RS4Label *src_rl;
RS4Label *dst_rl;
U32 val;

	ec		= RS4ErrStat_Error;
	src_rl	= gss->Label;

	#ifdef DEBUG
	if ( ! src_rl )
	{
		ec = RS4ErrStat_Internal;
		printf( "%s:%04d: Error - NULL Pointer\n", __FILE__, __LINE__ );
		goto bailout;
	}
	#endif

	// Dst is Addr : $00000004.l
	/**/ if (( rt->rt_CPU.M68k.mt_ArgEMode == 7 ) && ( rt->rt_CPU.M68k.mt_ArgEReg == 1 ))	
	{
		val = (( mem[0] << 24 ) | ( mem[1] << 16 ) | ( mem[2] << 8 ) | ( mem[3] << 0 ));

		DDEBUG( printf( "write : 21 :\n" ); )

		dst_rl = RS4FindLabel_File( rt->rt_File, val );

		while(( dst_rl ) && ( ! dst_rl->rl_UserLocked ))
		{
			// Label -> Label

			DDEBUG( printf( "write : 22 :\n" ); )

			if (( src_rl->rl_Type1	== RS4LabelType_Unset )
			&&	( dst_rl->rl_Type1	== RS4LabelType_Unset ))
			{
				// Both Labels have unset types, all okay
				break;
			}

			if (( src_rl->rl_Type1	== dst_rl->rl_Type1	)
			&&	( src_rl->rl_Type2	== dst_rl->rl_Type2 ))
			{
				// Same type, all okay
				break;
			}

			if ( dst_rl->rl_Type1 == RS4LabelType_Unset )
			{
				// Copy Src -> Dst
				dst_rl->rl_Type1	= src_rl->rl_Type1;
				dst_rl->rl_Type2	= src_rl->rl_Type2;
			}
			else
			{
				// We have two diffrent types, set it to unknown
				dst_rl->rl_Type1	= RS4LabelType_Unknown;
			}

			break;
		}
	}

	// Dst is Reg : Dx / Ax
	else if (( rt->rt_CPU.M68k.mt_ArgEMode == 0 ) || ( rt->rt_CPU.M68k.mt_ArgEMode == 1 ))
	{
		// Label -> Reg

		DDEBUG( printf( "write : 23 : Src Type1 %d\n", src_rl->rl_Type1 ); )

		switch( src_rl->rl_Type1 )
		{
			case RS4LabelType_Pointer:
			{
				DDEBUG( printf( "write : 24 : \n" ); )

				switch( src_rl->rl_Type2 )
				{
					case RS4LabelPtrType_Library:
					{
						DDEBUG( printf( "write : 25 : \n" ); )

//	mr_Type1	= RRT_Library,
//	mr_Type2	= AOS3_LibType_
//	mr_Label	= 
//	mr_Address	= 

//	rl_Type1 = RS4LabelType_Pointer,		// From current label Pos .. 4 bytes
//	rl_Type2 = RS4LabelPtrType_			RS4LabelPtrType_Library		RS4LabelPtrType_Struct
//	rl_Type3 = type					AOS3_LibType_


						dst_reg->mr_Type1	= RRT_Library;
						dst_reg->mr_Type2	= src_rl->rl_Type3;
						dst_reg->mr_Address = 0;
						dst_reg->mr_Label	= NULL;
						break;
					}

					default:
					{
						#ifdef DEBUG
						printf( "%s:%04d: Error Type 2 : %d\n", __FILE__, __LINE__, src_rl->rl_Type2 );
						#endif
						goto bailout;
					}
				}

				break;
			}

			default:
//			case RS4LabelType_Code:
//			case RS4LabelType_Unset:
//			case RS4LabelType_String:
//			case RS4LabelType_Unknown:
//			case RS4LabelType_RelativeWord:
			{
				dst_reg->mr_Type1 = RRT_Unknown;
				break;
			}
		}
	}

	// Dst is Unknown : Unsupported
	else
	{
		DDEBUG( printf( "write : 26 :\n" ); )
		// Do Nothing
	}

	ec = RS4ErrStat_Okay;

bailout:

	return( ec );
}

// --
