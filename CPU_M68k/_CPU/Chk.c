
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

enum RS4DecodeStat M68kCmd_CHK( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
int size;
int reg;
int pos;

	reg  = ( rt->rt_CPU.M68k.mt_Opcode & 0x0e000000 ) >> 25;
	size = ( rt->rt_CPU.M68k.mt_Opcode & 0x01800000 ) >> 23;

	switch( size )
	{
		case 2:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Chk.l";
			rt->rt_CPU.M68k.mt_ArgType  = M68KSIZE_Long;
			break;
		}

		case 3:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Chk.w";
			rt->rt_CPU.M68k.mt_ArgType  = M68KSIZE_Word;
			break;
		}

		default:
		{
			printf( "Unsupported 'Chk' Opcode (Size: %d) at %08lx\n", size, rt->rt_CurMemAdr );
			ds = RS4DecodeStat_Error;
			goto bailout;
		}
	}

	// --

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

	// --

	pos = strlen( rt->rt_Container.Hunk.ms_Buf_Argument );
	sprintf( & rt->rt_Container.Hunk.ms_Buf_Argument[pos], ",%s", Dx_RegNames[reg] );

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
