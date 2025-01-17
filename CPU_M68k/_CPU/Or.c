
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

enum RS4DecodeStat M68kCmd_OR( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
uint32_t opmode;

	opmode = ( rt->rt_CPU.M68k.mt_Opcode & 0x01c00000 ) >> 22;

	switch( opmode )
	{
		case 0:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Or.b";
			rt->rt_CPU.M68k.mt_ArgType  = M68KSIZE_Byte;
			rt->rt_CPU.M68k.mt_ArgEMode = ( rt->rt_CPU.M68k.mt_Opcode & 0x00380000 ) >> 19;
			rt->rt_CPU.M68k.mt_ArgEReg  = ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;

			rt->rt_CPU.M68k.mt_CurRegister = & rt->rt_CPU.M68k.mt_SrcRegister;

			ds = M68k_EffectiveAddress( & ec, rt );

			if ( ds != RS4DecodeStat_Okay )
			{
				// ec allready set

				#ifdef DEBUG
				printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
				#endif

				goto bailout;
			}

			rt->rt_CPU.M68k.mt_ArgEMode = 0x00; // Dx Reg
			rt->rt_CPU.M68k.mt_ArgEReg  = ( rt->rt_CPU.M68k.mt_Opcode & 0x0e000000 ) >> 25;

			rt->rt_CPU.M68k.mt_CurRegister = & rt->rt_CPU.M68k.mt_DstRegister;

			ds = M68k_EffectiveAddress( & ec, rt );

			if ( ds != RS4DecodeStat_Okay )
			{
				// ec allready set

				#ifdef DEBUG
				printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
				#endif

				goto bailout;
			}

			M68k_Set_Cur_to_Unknown( rt );
			rt->rt_CPU.M68k.mt_OpcodeSize = rt->rt_CPU.M68k.mt_ArgSize;
			break;
		}

		case 1:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Or.w";

			rt->rt_CPU.M68k.mt_ArgType  = M68KSIZE_Word;
			rt->rt_CPU.M68k.mt_ArgEMode = ( rt->rt_CPU.M68k.mt_Opcode & 0x00380000 ) >> 19;
			rt->rt_CPU.M68k.mt_ArgEReg  = ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;

			rt->rt_CPU.M68k.mt_CurRegister = & rt->rt_CPU.M68k.mt_SrcRegister;

			ds = M68k_EffectiveAddress( & ec, rt );

			if ( ds != RS4DecodeStat_Okay )
			{
				// ec allready set

				#ifdef DEBUG
				printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
				#endif

				goto bailout;
			}

			rt->rt_CPU.M68k.mt_ArgEMode = 0x00; // Dx Reg
			rt->rt_CPU.M68k.mt_ArgEReg  = ( rt->rt_CPU.M68k.mt_Opcode & 0x0e000000 ) >> 25;

			rt->rt_CPU.M68k.mt_CurRegister = & rt->rt_CPU.M68k.mt_DstRegister;

			ds = M68k_EffectiveAddress( & ec, rt );

			if ( ds != RS4DecodeStat_Okay )
			{
				// ec allready set

				#ifdef DEBUG
				printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
				#endif

				goto bailout;
			}

			M68k_Set_Cur_to_Unknown( rt );
			rt->rt_CPU.M68k.mt_OpcodeSize = rt->rt_CPU.M68k.mt_ArgSize;
			break;
		}

		case 2:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Or.l";

			rt->rt_CPU.M68k.mt_ArgType  = M68KSIZE_Long;
			rt->rt_CPU.M68k.mt_ArgEMode = ( rt->rt_CPU.M68k.mt_Opcode & 0x00380000 ) >> 19;
			rt->rt_CPU.M68k.mt_ArgEReg  = ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;

			rt->rt_CPU.M68k.mt_CurRegister = & rt->rt_CPU.M68k.mt_SrcRegister;

			ds = M68k_EffectiveAddress( & ec, rt );

			if ( ds != RS4DecodeStat_Okay )
			{
				// ec allready set

				#ifdef DEBUG
				printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
				#endif

				goto bailout;
			}

			rt->rt_CPU.M68k.mt_ArgEMode = 0x00; // Dx Reg
			rt->rt_CPU.M68k.mt_ArgEReg  = ( rt->rt_CPU.M68k.mt_Opcode & 0x0e000000 ) >> 25;

			rt->rt_CPU.M68k.mt_CurRegister = & rt->rt_CPU.M68k.mt_DstRegister;

			ds = M68k_EffectiveAddress( & ec, rt );

			if ( ds != RS4DecodeStat_Okay )
			{
				// ec allready set

				#ifdef DEBUG
				printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
				#endif

				goto bailout;
			}

			M68k_Set_Cur_to_Unknown( rt );
			rt->rt_CPU.M68k.mt_OpcodeSize = rt->rt_CPU.M68k.mt_ArgSize;
			break;
		}

		// 3 = Divu.w

		case 4:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Or.b";

			rt->rt_CPU.M68k.mt_ArgType  = M68KSIZE_Byte;
			rt->rt_CPU.M68k.mt_ArgEMode = 0x00; // Dx Reg
			rt->rt_CPU.M68k.mt_ArgEReg  = ( rt->rt_CPU.M68k.mt_Opcode & 0x0e000000 ) >> 25;

			rt->rt_CPU.M68k.mt_CurRegister = & rt->rt_CPU.M68k.mt_SrcRegister;

			ds = M68k_EffectiveAddress( & ec, rt );

			if ( ds != RS4DecodeStat_Okay )
			{
				// ec allready set

				#ifdef DEBUG
				printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
				#endif

				goto bailout;
			}

			rt->rt_CPU.M68k.mt_ArgEMode = ( rt->rt_CPU.M68k.mt_Opcode & 0x00380000 ) >> 19;
			rt->rt_CPU.M68k.mt_ArgEReg  = ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;

			rt->rt_CPU.M68k.mt_CurRegister = & rt->rt_CPU.M68k.mt_DstRegister;

			ds = M68k_EffectiveAddress( & ec, rt );

			if ( ds != RS4DecodeStat_Okay )
			{
				// ec allready set

				#ifdef DEBUG
				printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
				#endif

				goto bailout;
			}

			M68k_Set_Cur_to_Unknown( rt );
			rt->rt_CPU.M68k.mt_OpcodeSize = rt->rt_CPU.M68k.mt_ArgSize;
			break;
		}

		case 5:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Or.w";

			rt->rt_CPU.M68k.mt_ArgType  = M68KSIZE_Word;
			rt->rt_CPU.M68k.mt_ArgEMode = 0x00; // Dx Reg
			rt->rt_CPU.M68k.mt_ArgEReg  = ( rt->rt_CPU.M68k.mt_Opcode & 0x0e000000 ) >> 25;

			rt->rt_CPU.M68k.mt_CurRegister = & rt->rt_CPU.M68k.mt_SrcRegister;

			ds = M68k_EffectiveAddress( & ec, rt );

			if ( ds != RS4DecodeStat_Okay )
			{
				// ec allready set

				#ifdef DEBUG
				printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
				#endif

				goto bailout;
			}

			rt->rt_CPU.M68k.mt_ArgEMode = ( rt->rt_CPU.M68k.mt_Opcode & 0x00380000 ) >> 19;
			rt->rt_CPU.M68k.mt_ArgEReg  = ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;

			rt->rt_CPU.M68k.mt_CurRegister = & rt->rt_CPU.M68k.mt_DstRegister;

			ds = M68k_EffectiveAddress( & ec, rt );

			if ( ds != RS4DecodeStat_Okay )
			{
				// ec allready set

				#ifdef DEBUG
				printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
				#endif

				goto bailout;
			}

			M68k_Set_Cur_to_Unknown( rt );
			rt->rt_CPU.M68k.mt_OpcodeSize = rt->rt_CPU.M68k.mt_ArgSize;
			break;
		}

		case 6:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Or.l";

			rt->rt_CPU.M68k.mt_ArgType  = M68KSIZE_Long;
			rt->rt_CPU.M68k.mt_ArgEMode = 0x00; // Dx Reg
			rt->rt_CPU.M68k.mt_ArgEReg  = ( rt->rt_CPU.M68k.mt_Opcode & 0x0e000000 ) >> 25;

			rt->rt_CPU.M68k.mt_CurRegister = & rt->rt_CPU.M68k.mt_SrcRegister;

			ds = M68k_EffectiveAddress( & ec, rt );

			if ( ds != RS4DecodeStat_Okay )
			{
				// ec allready set

				#ifdef DEBUG
				printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
				#endif

				goto bailout;
			}

			rt->rt_CPU.M68k.mt_ArgEMode = ( rt->rt_CPU.M68k.mt_Opcode & 0x00380000 ) >> 19;
			rt->rt_CPU.M68k.mt_ArgEReg  = ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;

			rt->rt_CPU.M68k.mt_CurRegister = & rt->rt_CPU.M68k.mt_DstRegister;

			ds = M68k_EffectiveAddress( & ec, rt );

			if ( ds != RS4DecodeStat_Okay )
			{
				// ec allready set

				#ifdef DEBUG
				printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
				#endif

				goto bailout;
			}

			M68k_Set_Cur_to_Unknown( rt );
			rt->rt_CPU.M68k.mt_OpcodeSize = rt->rt_CPU.M68k.mt_ArgSize;
			break;
		}

		// 7 : Divs.w

		default:
		{
			printf( "Unsupported 'Or' Opcode (Mode: %d)\n", opmode );
			ds = RS4DecodeStat_Error;
			goto bailout;
		}
	}

	// --

	ds = RS4DecodeStat_Okay;
	ec = RS4ErrStat_Okay;

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	return( ds );
}
