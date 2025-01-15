 
/*
 * Copyright (c) 2014-2024 Rene W. Olsen < renewolsen @ gmail . com >
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

enum RS4DecodeStat M68kCmd_MUL( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
int opmode;

	opmode = ( rt->rt_CPU.M68k.mt_Opcode & 0x01c00000 ) >> 22;

	switch( opmode )
	{
		// 0 : And.b

		// 1 : And.w

		// 2 : And.l

		case 3:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Mulu.w";
			rt->rt_CPU.M68k.mt_DecMode	= M68kDecMode_Unsigned;
			break;
		}

		// 4 : And.b

		// 5 : And.w

		// 6 : And.l

		case 7:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Muls.w";
			rt->rt_CPU.M68k.mt_DecMode	= M68kDecMode_Signed;
			break;
		}

		default:
		{
			printf( "Unsupported 'Mul' Opcode (Mode: %d)\n", opmode );
			ds = RS4DecodeStat_Error;
			goto bailout;
		}
	}

	// --

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

	// --

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
//	rt->rt_CPU.M68k.mt_CurRegister->mr_Type1 = RRT_Unknown;
	rt->rt_CPU.M68k.mt_OpcodeSize = rt->rt_CPU.M68k.mt_ArgSize;

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

// --

enum RS4DecodeStat M68kCmd_MULS_L( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
uint32_t pos;

	rt->rt_Container.Hunk.ms_Str_Opcode = "Muls.l";

	rt->rt_CPU.M68k.mt_DecMode	= M68kDecMode_Signed;
	rt->rt_CPU.M68k.mt_ArgSize	= 4;
	rt->rt_CPU.M68k.mt_ArgType	= M68KSIZE_Long;
	rt->rt_CPU.M68k.mt_ArgEMode	= ( rt->rt_CPU.M68k.mt_Opcode & 0x00380000 ) >> 19;
	rt->rt_CPU.M68k.mt_ArgEReg	= ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;

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

	if ( rt->rt_CPU.M68k.mt_Opcode & 0x00000400 )
	{
		// Divs.l Dx,Dh:Di

		rt->rt_CPU.M68k.mt_ArgEMode	= 0x00; // Dx Reg
		rt->rt_CPU.M68k.mt_ArgEReg	= rt->rt_CPU.M68k.mt_Opcode & 0x00000007;

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
//		rt->rt_CPU.M68k.mt_CurRegister->mr_Type1 = RRT_Unknown;

		pos = strlen( rt->rt_Container.Hunk.ms_Buf_Argument );

		rt->rt_CPU.M68k.mt_ArgEMode	= 0x00; // Dx Reg
		rt->rt_CPU.M68k.mt_ArgEReg	= ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 12;

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
//		rt->rt_CPU.M68k.mt_CurRegister->mr_Type1 = RRT_Unknown;
		rt->rt_Container.Hunk.ms_Buf_Argument[ pos ] = ':';
	}
	else
	{
		// Divs.l Dx,Di

		rt->rt_CPU.M68k.mt_ArgEMode	= 0x00; // Dx Reg
		rt->rt_CPU.M68k.mt_ArgEReg	= ( rt->rt_CPU.M68k.mt_Opcode & 0x00007000 ) >> 12;

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
//		rt->rt_CPU.M68k.mt_CurRegister->mr_Type1 = RRT_Unknown;
	}

	rt->rt_CPU.M68k.mt_OpcodeSize = rt->rt_CPU.M68k.mt_ArgSize;

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

// --

enum RS4DecodeStat M68kCmd_MULU_L( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
uint32_t pos;

	rt->rt_Container.Hunk.ms_Str_Opcode = "Mulu.l";

	rt->rt_CPU.M68k.mt_DecMode	= M68kDecMode_Unsigned;
	rt->rt_CPU.M68k.mt_ArgSize	= 4;
	rt->rt_CPU.M68k.mt_ArgType	= M68KSIZE_Long;
	rt->rt_CPU.M68k.mt_ArgEMode	= ( rt->rt_CPU.M68k.mt_Opcode & 0x00380000 ) >> 19;
	rt->rt_CPU.M68k.mt_ArgEReg	= ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;

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

	if ( rt->rt_CPU.M68k.mt_Opcode & 0x00000400 )
	{
		// Divs.l Dx,Dh:Di

		rt->rt_CPU.M68k.mt_ArgEMode	= 0x00; // Dx Reg
		rt->rt_CPU.M68k.mt_ArgEReg	= rt->rt_CPU.M68k.mt_Opcode & 0x00000007;

		ds = M68k_EffectiveAddress( & ec, rt );

		if ( ds != RS4DecodeStat_Okay )
		{
			// ec allready set

			#ifdef DEBUG
			printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
			#endif

			goto bailout;
		}

		pos = strlen( rt->rt_Container.Hunk.ms_Buf_Argument );

		rt->rt_CPU.M68k.mt_ArgEMode	= 0x00; // Dx Reg
		rt->rt_CPU.M68k.mt_ArgEReg	= ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 12;

		ds = M68k_EffectiveAddress( & ec, rt );

		if ( ds != RS4DecodeStat_Okay )
		{
			// ec allready set

			#ifdef DEBUG
			printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
			#endif

			goto bailout;
		}

		rt->rt_Container.Hunk.ms_Buf_Argument[ pos ] = ':';
	}
	else
	{
		// Divs.l Dx,Di

		rt->rt_CPU.M68k.mt_ArgEMode	= 0x00; // Dx Reg
		rt->rt_CPU.M68k.mt_ArgEReg	= ( rt->rt_CPU.M68k.mt_Opcode & 0x00007000 ) >> 12;

		ds = M68k_EffectiveAddress( & ec, rt );

		if ( ds != RS4DecodeStat_Okay )
		{
			// ec allready set

			#ifdef DEBUG
			printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
			#endif

			goto bailout;
		}
	}

	rt->rt_CPU.M68k.mt_OpcodeSize = rt->rt_CPU.M68k.mt_ArgSize;

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
