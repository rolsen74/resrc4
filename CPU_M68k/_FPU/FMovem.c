
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

// $f227e07c		-- 2.0.1	fmovem.x	FP2-FP6,-(a7)
// $f228f0ff,$003c	-- 2.2.1	fmovem.x	fp0-fp7,$003c(a7)
// $f21fd030		-- 2.2.0	fmovem.x	(a7)+,FP2-FP3

// --

static enum RS4DecodeStat RegMask( enum RS4ErrorCode *errcode, RS4Trace *rt, int reverse )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
char *buf;
uint32_t mask;
uint32_t pos;
uint32_t bit;
int start;
int end;
int cnt;

	mask = rt->rt_CPU.M68k.mt_Opcode & 0x00ff;

	start = end = -1;

	pos = strlen( rt->rt_Container.Hunk.ms_Buf_Argument );

	if ( pos )
	{
		sprintf( & rt->rt_Container.Hunk.ms_Buf_Argument[ pos ], "," );
		pos++;
	}

	buf = & rt->rt_Container.Hunk.ms_Buf_Argument[pos];

	bit = ( ! reverse ) ? 0x01 : 0x80;

	for( cnt=0 ; cnt<9 ; cnt++ )
	{
		if (( mask & bit ) && ( cnt != 8 ))
		{
			if ( start == -1 )
			{
				start = end = cnt;
			}
			else
			{
				end = cnt;
			}
		}
		else
		{
			if ( start != -1 )	
			{
				pos = strlen( buf );

				if ( pos )
				{
					buf[pos++] = '/';
				}

				if ( start == end )
				{
					sprintf( &buf[pos], "%s", FPx_RegNames[start] );
				}
				else
				{
					sprintf( &buf[pos], "%s-%s", FPx_RegNames[start], FPx_RegNames[end] );
				}

				start = -1;
			}
		}

		if ( cnt != 8 )
		{
			bit = ( ! reverse ) ? bit * 2 : bit / 2;
		}
	}

	// --

	ds = RS4DecodeStat_Okay;
	ec = RS4ErrStat_Okay;

	// --

//bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( ds );
}

// --

#if 0
enum RS4DecodeStat M68kCmd_FMOVEM_1_0( RS4Trace *rt, int emode, int ereg, int dr )
{
}

// --

enum RS4DecodeStat M68kCmd_FMOVEM_1_1( RS4Trace *rt, int emode, int ereg, int dr )
{
}

// --

enum RS4DecodeStat M68kCmd_FMOVEM_1_2( RS4Trace *rt, int emode, int ereg, int dr )
{
}

// --

enum RS4DecodeStat M68kCmd_FMOVEM_1_3( RS4Trace *rt, int emode, int ereg, int dr )
{
}
#endif

// --

enum RS4DecodeStat M68kCmd_FMOVEM_2_0( enum RS4ErrorCode *errcode, RS4Trace *rt, int emode, int ereg, int dr )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;

	if ( dr )
	{
		// FMovem.x FP2-FP3,<EA>
		// $f227e07c		-- 2.0.1	fmovem.x	FP2-FP6,-(a7)

		ds = RegMask( & ec, rt, FALSE );

		rt->rt_CPU.M68k.mt_ArgEMode = emode;
		rt->rt_CPU.M68k.mt_ArgEReg  = ereg;
		rt->rt_CPU.M68k.mt_ArgSize	= 4;

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
	}
	else
	{
		printf( "Unsupported 'FMovem 2 0' Opcode %d at $%08" PRIx64 "\n", dr, rt->rt_CurMemAdr );
		ds = RS4DecodeStat_Error;
		goto bailout;
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

#if 0
enum RS4DecodeStat M68kCmd_FMOVEM_2_1( RS4Trace *rt, int emode, int ereg, int dr )
{
}
#endif

// --

enum RS4DecodeStat M68kCmd_FMOVEM_2_2( enum RS4ErrorCode *errcode, RS4Trace *rt, int emode, int ereg, int dr )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;

	if ( dr )
	{
		// $f228f0ff,$003c -- 2.2.1	fmovem.x	fp0-fp7,$003c(a7)

		ds = RegMask( & ec, rt, TRUE );

		if ( ds != RS4DecodeStat_Okay )
		{
			// ec allready set

			#ifdef DEBUG
			printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
			#endif

			goto bailout;
		}

		rt->rt_CPU.M68k.mt_ArgEMode = emode;
		rt->rt_CPU.M68k.mt_ArgEReg  = ereg;
		rt->rt_CPU.M68k.mt_ArgSize	= 4;

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
	}
	else
	{
		// FMovem.x <EA>,FP2-FP3
		// $f21fd030 -- fmovem.x	(a7)+,FP2-FP3

		rt->rt_CPU.M68k.mt_ArgEMode = emode;
		rt->rt_CPU.M68k.mt_ArgEReg  = ereg;
		rt->rt_CPU.M68k.mt_ArgSize	= 4;

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

		ds = RegMask( & ec, rt, TRUE );

		if ( ds != RS4DecodeStat_Okay )
		{
			// ec allready set

			#ifdef DEBUG
			printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
			#endif

			goto bailout;
		}

		rt->rt_CPU.M68k.mt_OpcodeSize = rt->rt_CPU.M68k.mt_ArgSize;
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

#if 0
enum RS4DecodeStat M68kCmd_FMOVEM_2_3( RS4Trace *rt, int emode, int ereg, int dr )
{
}
#endif

// --

enum RS4DecodeStat M68kCmd_FMOVEM( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
int emode;
int ereg;
int list;
int dr;

	rt->rt_Container.Hunk.ms_Str_Opcode = "FMovem.l";
	rt->rt_CPU.M68k.mt_ArgType = M68KSIZE_Long;

	emode= ( rt->rt_CPU.M68k.mt_Opcode & 0x00380000 ) >> 19;
	ereg = ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;
	dr	 = ( rt->rt_CPU.M68k.mt_Opcode & 0x00002000 );
	list = ( rt->rt_CPU.M68k.mt_Opcode & 0x00001c00 ) >> 10;

	// List hold 3 bits (FPCR, FPSR, FPIAR) wich regs to be moved
	// atleat two must be selected, well if only 1 is select its a fmove4

	printf( "Unsupported 'FMovem' Opcode (List %x, emode %d, ereg %d, dr %d) at $%08" PRIx64 "\n", list, emode, ereg, dr, rt->rt_CurMemAdr );
	ds = RS4DecodeStat_Error;
	ec = RS4ErrStat_Internal;

	// --

//	ds = RS4DecodeStat_Okay;
//	ec = RS4ErrStat_Okay;

//bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	return( ds );
}

// --

enum RS4DecodeStat M68kCmd_FMOVEM2( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
int emode;
int ereg;
int mode;
int dr;

	rt->rt_Container.Hunk.ms_Str_Opcode = "FMovem.x";
	rt->rt_CPU.M68k.mt_ArgType = M68KSIZE_Extended;

	emode= ( rt->rt_CPU.M68k.mt_Opcode & 0x00380000 ) >> 19;
	ereg = ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;
	dr	 = ( rt->rt_CPU.M68k.mt_Opcode & 0x00002000 ) ? TRUE : FALSE ;
	mode = ( rt->rt_CPU.M68k.mt_Opcode & 0x00001800 ) >> 11;

	switch( mode )
	{
		case 0:
		{
			M68kCmd_FMOVEM_2_0( & ec, rt, emode, ereg, dr );
			break;
		}

		#if 0
		case 1:
		{
			M68kCmd_FMOVEM_2_1( & ec, rt, emode, ereg, dr );
			break;
		}
		#endif

		case 2:
		{
			M68kCmd_FMOVEM_2_2( & ec, rt, emode, ereg, dr );
			break;
		}

		#if 0
		case 3:
		{
			M68kCmd_FMOVEM_2_3( & ec, rt, emode, ereg, dr );
			break;
		}
		#endif

		default:
		{
			printf( "Unsupported 'FMovem2' Opcode (Mode %d, emode %d, ereg %d, dr %d) at $%08" PRIx64 "\n", mode, emode, ereg, dr, rt->rt_CurMemAdr );
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
