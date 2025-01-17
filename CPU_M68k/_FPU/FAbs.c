
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

enum RS4DecodeStat M68kCmd_FABS( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
int src;
int dst;

	src  = ( rt->rt_CPU.M68k.mt_Opcode & 0x00001c00 ) >> 10;
	dst  = ( rt->rt_CPU.M68k.mt_Opcode & 0x00000380 ) >> 7;

	rt->rt_Container.Hunk.ms_Str_Opcode = "FAbs.x";
	rt->rt_CPU.M68k.mt_OpcodeSize = 4;

	sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "%s,%s", FPx_RegNames[src], FPx_RegNames[dst] );

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

enum RS4DecodeStat M68kCmd_FABS2( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
int emode;
int ereg;
int src;
int dst;
int len;

	emode= ( rt->rt_CPU.M68k.mt_Opcode & 0x00380000 ) >> 19;
	ereg = ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;
	src  = ( rt->rt_CPU.M68k.mt_Opcode & 0x00001c00 ) >> 10;
	dst  = ( rt->rt_CPU.M68k.mt_Opcode & 0x00000380 ) >> 7;

	switch( src )
	{
		case 0:
		{
			rt->rt_CPU.M68k.mt_ArgType = M68KSIZE_Long;
			rt->rt_Container.Hunk.ms_Str_Opcode = "FAbs.l";
			break;
		}

		case 1:
		{
			rt->rt_CPU.M68k.mt_ArgType = M68KSIZE_Single;
			rt->rt_Container.Hunk.ms_Str_Opcode = "FAbs.s";
			break;
		}

		case 2:
		{
			rt->rt_CPU.M68k.mt_ArgType = M68KSIZE_Extended;
			rt->rt_Container.Hunk.ms_Str_Opcode = "FAbs.x";
			break;
		}

		case 3:
		{
			rt->rt_CPU.M68k.mt_ArgType = M68KSIZE_Packed;
			rt->rt_Container.Hunk.ms_Str_Opcode = "FAbs.p";
			break;
		}

		case 4:
		{
			rt->rt_CPU.M68k.mt_ArgType = M68KSIZE_Word;
			rt->rt_Container.Hunk.ms_Str_Opcode = "FAbs.w";
			break;
		}

		case 5:
		{
			rt->rt_CPU.M68k.mt_ArgType = M68KSIZE_Double;
			rt->rt_Container.Hunk.ms_Str_Opcode = "FAbs.d";
			break;
		}

		case 6:
		{
			rt->rt_CPU.M68k.mt_ArgType = M68KSIZE_Byte;
			rt->rt_Container.Hunk.ms_Str_Opcode = "FAbs.b";
			break;
		}

		default:
		{
			printf( "Unsupported 'FAbs2' Opcode (Size %d) at $%08" PRIx64 "\n", src, rt->rt_CurMemAdr );
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
