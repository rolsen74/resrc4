
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

enum RS4DecodeStat M68kCmd_FMOVE( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
int mode;
int src;
int dst;

// printf( "M68kCmd_FMOVE 1\n" );

	src  = ( rt->rt_CPU.M68k.mt_Opcode & 0x00001c00 ) >> 10;
	dst  = ( rt->rt_CPU.M68k.mt_Opcode & 0x00000380 ) >> 7;
	mode = ( rt->rt_CPU.M68k.mt_Opcode & 0x0000007f );

	if ( mode != 0 )
	{
		printf( "Unsupported 'FMove1' Opcode (Mode $%03x) at $%08" PRIx64 "\n", mode, rt->rt_CurMemAdr );
		ds = RS4DecodeStat_Error;
		goto bailout;
	}

	rt->rt_Container.Hunk.ms_Str_Opcode = "FMove.x";
	rt->rt_CPU.M68k.mt_OpcodeSize = 4;

	sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "%s,%s", FPx_RegNames[src], FPx_RegNames[dst] );

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

enum RS4DecodeStat M68kCmd_FMOVE2( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
int emode;
int ereg;
int mode;
int src;
int dst;
int len;

// printf( "M68kCmd_FMOVE 2\n" );

	emode= ( rt->rt_CPU.M68k.mt_Opcode & 0x00380000 ) >> 19;
	ereg = ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;
	src  = ( rt->rt_CPU.M68k.mt_Opcode & 0x00001c00 ) >> 10;
	dst  = ( rt->rt_CPU.M68k.mt_Opcode & 0x00000380 ) >> 7;
	mode = ( rt->rt_CPU.M68k.mt_Opcode & 0x0000007f );

	if ( mode != 0 )
	{
		printf( "Unsupported 'FMove2' Opcode (Mode $%03x) at $%08" PRIx64 "\n", mode, rt->rt_CurMemAdr );
		ds = RS4DecodeStat_Error;
		goto bailout;
	}

	switch( src )
	{
		case 0:
		{
			rt->rt_CPU.M68k.mt_ArgType = M68KSIZE_Long;
			rt->rt_Container.Hunk.ms_Str_Opcode = "FMove.l";
			break;
		}

		case 1:
		{
			rt->rt_CPU.M68k.mt_ArgType = M68KSIZE_Single;
			rt->rt_Container.Hunk.ms_Str_Opcode = "FMove.s";
			break;
		}

		case 2:
		{
			rt->rt_CPU.M68k.mt_ArgType = M68KSIZE_Extended;
			rt->rt_Container.Hunk.ms_Str_Opcode = "FMove.x";
			break;
		}

		case 3:
		{
			rt->rt_CPU.M68k.mt_ArgType = M68KSIZE_Packed;
			rt->rt_Container.Hunk.ms_Str_Opcode = "FMove.p";
			break;
		}

		case 4:
		{
			rt->rt_CPU.M68k.mt_ArgType = M68KSIZE_Word;
			rt->rt_Container.Hunk.ms_Str_Opcode = "FMove.w";
			break;
		}

		case 5:
		{
			rt->rt_CPU.M68k.mt_ArgType = M68KSIZE_Double;
			rt->rt_Container.Hunk.ms_Str_Opcode = "FMove.d";
			break;
		}

		case 6:
		{
			rt->rt_CPU.M68k.mt_ArgType = M68KSIZE_Byte;
			rt->rt_Container.Hunk.ms_Str_Opcode = "FMove.b";
			break;
		}

		default:
		{
			printf( "Unsupported 'FMove2' Opcode (Size %d) at $%08" PRIx64 "\n", src, rt->rt_CurMemAdr );
			ds = RS4DecodeStat_Error;
			goto bailout;
		}
	}

	// --

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

	// --

	len = strlen( rt->rt_Container.Hunk.ms_Buf_Argument );

	sprintf( & rt->rt_Container.Hunk.ms_Buf_Argument[len], ",%s", FPx_RegNames[dst] );

	// --

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

enum RS4DecodeStat M68kCmd_FMOVE3( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
int emode;
int ereg;
int fmt;
int src;
//int len;
//int k;

// printf( "M68kCmd_FMOVE 3 : at $%08" PRIx64 "\n", rt->rt_CurMemAdr );

	emode= ( rt->rt_CPU.M68k.mt_Opcode & 0x00380000 ) >> 19;
	ereg = ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;
	fmt  = ( rt->rt_CPU.M68k.mt_Opcode & 0x00001c00 ) >> 10;
	src  = ( rt->rt_CPU.M68k.mt_Opcode & 0x00000380 ) >> 7;
//	k	 = ( rt->rt_CPU.M68k.mt_Opcode & 0x0000007f );

	switch( fmt )
	{
		case 0:
		{
			rt->rt_CPU.M68k.mt_ArgType = M68KSIZE_Long;
			rt->rt_Container.Hunk.ms_Str_Opcode = "FMove.l";
			break;
		}

		case 1:
		{
			rt->rt_CPU.M68k.mt_ArgType = M68KSIZE_Single;
			rt->rt_Container.Hunk.ms_Str_Opcode = "FMove.s";
			break;
		}

		case 2:
		{
			rt->rt_CPU.M68k.mt_ArgType = M68KSIZE_Extended;
			rt->rt_Container.Hunk.ms_Str_Opcode = "FMove.x";
			break;
		}

		#if 0
		case 3:
		{
			rt->rt_CPU.M68k.mt_ArgType = M68KSIZE_Packed;
			rt->rt_Container.Hunk.ms_Str_Opcode = "FMove.p";
			break;
		}
		#endif

		case 4:
		{
			rt->rt_CPU.M68k.mt_ArgType = M68KSIZE_Word;
			rt->rt_Container.Hunk.ms_Str_Opcode = "FMove.w";
			break;
		}

		case 5:
		{
			rt->rt_CPU.M68k.mt_ArgType = M68KSIZE_Double;
			rt->rt_Container.Hunk.ms_Str_Opcode = "FMove.d";
			break;
		}

		case 6:
		{
			rt->rt_CPU.M68k.mt_ArgType = M68KSIZE_Byte;
			rt->rt_Container.Hunk.ms_Str_Opcode = "FMove.b";
			break;
		}

		#if 0
		case 7:
		{
			rt->rt_CPU.M68k.mt_ArgType = M68KSIZE_Packed;
			rt->rt_Container.Hunk.ms_Str_Opcode = "FMove.p";
			break;
		}
		#endif

		default:
		{
			printf( "Unsupported 'FMove3' Opcode (Size %d) at $%08" PRIx64 "\n", fmt, rt->rt_CurMemAdr );
			ds = RS4DecodeStat_Error;
			goto bailout;
		}
	}

	// --

//	len = strlen( rt->rt_Container.Hunk.ms_Buf_Argument );
//
//	sprintf( & rt->rt_Container.Hunk.ms_Buf_Argument[len], ",%s", FPx_RegNames[src] );

	sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "%s", FPx_RegNames[src] );

	// --

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

	// --

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

enum RS4DecodeStat M68kCmd_FMOVE4( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
char *name;
int emode;
int ereg;
int len;
int reg;
int dr;

	// --

	ec = RS4ErrStat_Error;
	ds = RS4DecodeStat_Error;

	// --

	emode	= ( rt->rt_CPU.M68k.mt_Opcode & 0x00380000 ) >> 19;
	ereg	= ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;
	dr		= ( rt->rt_CPU.M68k.mt_Opcode & 0x00002000 );
	reg 	= ( rt->rt_CPU.M68k.mt_Opcode & 0x00001c00 ) >> 10;

	switch( reg )
	{
		case 0x1:
		{
			// Floating-Point Instruction Address Regiser
			name = "fpiar";
			break;
		}

		case 0x2:
		{
			// Floating-Point Instruction Status Regiser
			name = "fpsr";
			break;
		}

		case 0x4:
		{
			// Floating-Point Instruction Control Regiser
			name = "fpcr";
			break;
		}

		default:
		{
			printf( "Unsupported 'FMove4' Reg Type (%d) at $%08" PRIx64 "\n", reg, rt->rt_CurMemAdr );
			ds = RS4DecodeStat_Error;
			goto bailout;
		}
	}

	rt->rt_CPU.M68k.mt_CurRegister = & rt->rt_CPU.M68k.mt_SrcRegister;
	rt->rt_Container.Hunk.ms_Str_Opcode = "FMove.l";
	rt->rt_CPU.M68k.mt_ArgType	= M68KSIZE_Long;
	rt->rt_CPU.M68k.mt_ArgEMode = emode;
	rt->rt_CPU.M68k.mt_ArgEReg  = ereg;
	rt->rt_CPU.M68k.mt_ArgSize	= 4;

	if ( dr )
	{
		sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "%s", name );

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
		ds = M68k_EffectiveAddress( & ec, rt );

		if ( ds != RS4DecodeStat_Okay )
		{
			// ec allready set

			#ifdef DEBUG
			printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
			#endif

			goto bailout;
		}

		len = strlen( rt->rt_Container.Hunk.ms_Buf_Argument );

		sprintf( & rt->rt_Container.Hunk.ms_Buf_Argument[len], ",%s", name );
	}

	rt->rt_CPU.M68k.mt_OpcodeSize = rt->rt_CPU.M68k.mt_ArgSize;

	// --

	ds = RS4DecodeStat_Okay;
	ec = RS4ErrStat_Okay;

	// --

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( ds );
}
