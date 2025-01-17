
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

enum RS4ErrorCode Misc_Set__Src_is_Reg( 
	struct AmigaOS3_Misc_Move_GetSetStruct *gss,
	struct M68kRegister *dst_reg, 
	RS4Trace *rt,
	uint8_t *mem )
{
struct M68kRegister *src_reg;
enum RS4ErrorCode ec;
RS4Label *src_rl;
RS4Label *dst_rl;
uint32_t val;

	ec = RS4ErrStat_Error;

	src_reg = & gss->Reg;

	// Dst is Addr : $00000004.l
	/**/ if (( rt->rt_CPU.M68k.mt_ArgEMode == 7 ) && ( rt->rt_CPU.M68k.mt_ArgEReg == 1 ))	
	{
		val = (( mem[0] << 24 ) | ( mem[1] << 16 ) | ( mem[2] << 8 ) | ( mem[3] << 0 ));

		dst_rl = RS4FindLabel_File( rt->rt_File, val );

		DDEBUG( printf( "write : 31 : dst_rl %p\n", dst_rl ); )

		if (( dst_rl ) && ( ! dst_rl->rl_UserLocked ))
		{
			switch( src_reg->mr_Type1 )
			{
				case RRT_Unknown:
				{
					DDEBUG( printf( "write : 32 :\n" ); )

					// Check if Label Type have been set
					// if yes, then its used for more than one type
					// and we have to set Unknown
					if ( dst_rl->rl_Type1 != RS4LabelType_Unset )
					{
						dst_rl->rl_Type1 = RS4LabelType_Unknown;
					}
					break;
				}

				case RRT_Label:
				{
					DDEBUG( printf( "write : 33 : Adr $%08" PRIx64 ", %x -> \n", rt->rt_CurMemAdr, src_reg->mr_Number ); )

					// Label -> Label

					src_rl = src_reg->mr_Label;

					#ifdef DEBUG
					if ( ! src_rl )
					{
						ec = RS4ErrStat_Internal;
						printf( "%s:%04d: Error - NULL Pointer\n", __FILE__, __LINE__ );
						goto bailout;
					}
					#endif

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

				case RRT_Library:
				{
					DDEBUG( printf( "write : 34 : %x -> \n", src_reg->mr_Number ); )

					// src Reg is a Library
					// Reg -> Label

					if ( dst_rl->rl_Type1	== RS4LabelType_Unset )
					{
						// Set Label Type
						dst_rl->rl_Type1	= RS4LabelType_Pointer;
						dst_rl->rl_Type2	= RS4LabelPtrType_Library;
						dst_rl->rl_Type3	= src_reg->mr_Type2;
						break;
					}

					if (( dst_rl->rl_Type1	== RS4LabelType_Pointer )
					&&	( dst_rl->rl_Type2	== RS4LabelPtrType_Library )
					&&	( dst_rl->rl_Type3	== src_reg->mr_Type2 ))
					{
						// Same type, all okay
						break;
					}

					// We have two diffrent types, set it to unknown
					dst_rl->rl_Type1		= RS4LabelType_Unknown;
					break;
				}

				default:
				{
					#ifdef DEBUG
					printf( "%s:%04d: Error (%d)\n", __FILE__, __LINE__, gss->Reg.mr_Type1 );
					#endif
					goto bailout;
				}
			}
		}
	}

	// Dst is Reg : Dx / Ax
	else if (( rt->rt_CPU.M68k.mt_ArgEMode == 0 ) || ( rt->rt_CPU.M68k.mt_ArgEMode == 1 ))
	{
		// Reg -> Reg

		DDEBUG( printf( "write : 35 : Reg %x -> Reg %x\n", gss->Reg.mr_Number, dst_reg->mr_Number ); )

		*dst_reg = gss->Reg;
	}

	// Dst is Unknown : Unsupported
	else
	{
		DDEBUG( printf( "write : 36 : \n" ); )

		// Do Nothing
	}

	ec = RS4ErrStat_Okay;

bailout:

	return( ec );
}

// --
