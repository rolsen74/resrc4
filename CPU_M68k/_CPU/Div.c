
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

enum RS4DecodeStat M68kCmd_DIV( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
uint32_t opmode;

	opmode = ( rt->rt_CPU.M68k.mt_Opcode & 0x01c00000 ) >> 22;

	switch( opmode )
	{
		// 0 : Or.b

		// 1 : Or.w

		// 2 : Or.l

		case 3:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Divu.w";
			rt->rt_CPU.M68k.mt_ArgType	= M68KSIZE_Word;
			rt->rt_CPU.M68k.mt_DecMode	= M68kDecMode_Unsigned;
			break;
		}

		// 4 : Or.b

		// 5 : Or.w

		// 6 : Or.l

		case 7:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Divs.w";
			rt->rt_CPU.M68k.mt_ArgType	= M68KSIZE_Word;
			rt->rt_CPU.M68k.mt_DecMode	= M68kDecMode_Signed;
			break;
		}

		default:
		{
			printf( "Unsupported 'Div' Opcode (Mode: %d)\n", opmode );
			ds = RS4DecodeStat_Error;
			goto bailout;
		}
	}

	// --

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

enum RS4DecodeStat M68kCmd_DIVS_L( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
uint32_t pos;
int EMode;
int EReg;
int Size;
int Dq;
int Dr;

	EMode	= ( rt->rt_CPU.M68k.mt_Opcode & 0x00380000 ) >> 19;
	EReg	= ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;
	Dq		= ( rt->rt_CPU.M68k.mt_Opcode & 0x00007000 ) >> 12;
	Size	= ( rt->rt_CPU.M68k.mt_Opcode & 0x00000400 );
	Dr		= ( rt->rt_CPU.M68k.mt_Opcode & 0x00000007 );

	// --

	rt->rt_CPU.M68k.mt_DecMode	= M68kDecMode_Signed;
	rt->rt_CPU.M68k.mt_ArgSize	= 4;
	rt->rt_CPU.M68k.mt_ArgType	= M68KSIZE_Long;
	rt->rt_CPU.M68k.mt_ArgEMode	= EMode;
	rt->rt_CPU.M68k.mt_ArgEReg	= EReg;

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

	rt->rt_CPU.M68k.mt_OpcodeSize = rt->rt_CPU.M68k.mt_ArgSize;

	// --

	if ( Size )
	{
		rt->rt_Container.Hunk.ms_Str_Opcode = "Divs.l";

		if ( Dr == Dq )
		{
			// Not seen

			rt->rt_CPU.M68k.mt_ArgEMode	= 0x00; // Dx Reg
			rt->rt_CPU.M68k.mt_ArgEReg	= Dq;

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
		else
		{
			rt->rt_CPU.M68k.mt_ArgEMode	= 0x00; // Dx Reg
			rt->rt_CPU.M68k.mt_ArgEReg	= Dr;

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
			rt->rt_CPU.M68k.mt_ArgEReg	= Dq;

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
	}
	else
	{
		if ( Dr == Dq )
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Divs.l";

			rt->rt_CPU.M68k.mt_ArgEMode	= 0x00; // Dx Reg
			rt->rt_CPU.M68k.mt_ArgEReg	= Dq;

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
		else
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Divsl.l";

			rt->rt_CPU.M68k.mt_ArgEMode	= 0x00; // Dx Reg
			rt->rt_CPU.M68k.mt_ArgEReg	= Dr;

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
			rt->rt_CPU.M68k.mt_ArgEReg	= Dq;

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

// --

enum RS4DecodeStat M68kCmd_DIVU_L( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
uint32_t pos;
int EMode;
int EReg;
int Size;
int Dq;
int Dr;

	EMode	= ( rt->rt_CPU.M68k.mt_Opcode & 0x00380000 ) >> 19;
	EReg	= ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;
	Dq		= ( rt->rt_CPU.M68k.mt_Opcode & 0x00007000 ) >> 12;
	Size	= ( rt->rt_CPU.M68k.mt_Opcode & 0x00000400 );
	Dr		= ( rt->rt_CPU.M68k.mt_Opcode & 0x00000007 );

	// --

	rt->rt_CPU.M68k.mt_DecMode	= M68kDecMode_Unsigned;
	rt->rt_CPU.M68k.mt_ArgSize	= 4;
	rt->rt_CPU.M68k.mt_ArgType	= M68KSIZE_Long;
	rt->rt_CPU.M68k.mt_ArgEMode	= EMode;
	rt->rt_CPU.M68k.mt_ArgEReg	= EReg;

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

	rt->rt_CPU.M68k.mt_OpcodeSize = rt->rt_CPU.M68k.mt_ArgSize;

	// --

	if ( Size )
	{
		rt->rt_Container.Hunk.ms_Str_Opcode = "Divu.l";

		if ( Dr == Dq )
		{
			// Not seen

			rt->rt_CPU.M68k.mt_ArgEMode	= 0x00; // Dx Reg
			rt->rt_CPU.M68k.mt_ArgEReg	= Dq;

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
		else
		{
			rt->rt_CPU.M68k.mt_ArgEMode	= 0x00; // Dx Reg
			rt->rt_CPU.M68k.mt_ArgEReg	= Dr;

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
			rt->rt_CPU.M68k.mt_ArgEReg	= Dq;

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
	}
	else
	{
		if ( Dr == Dq )
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Divu.l";

			rt->rt_CPU.M68k.mt_ArgEMode	= 0x00; // Dx Reg
			rt->rt_CPU.M68k.mt_ArgEReg	= Dq;

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
		else
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Divul.l";

			rt->rt_CPU.M68k.mt_ArgEMode	= 0x00; // Dx Reg
			rt->rt_CPU.M68k.mt_ArgEReg	= Dr;

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
			rt->rt_CPU.M68k.mt_ArgEReg	= Dq;

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
