
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

enum RS4DecodeStat M68kCmd_CMPI( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
int size;

	size = ( rt->rt_CPU.M68k.mt_Opcode & 0x00c00000 ) >> 22;

	switch( size )
	{
		case 0:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = ( ShortOpcodes ) ? "Cmp.b" : "Cmpi.b" ;
			rt->rt_CPU.M68k.mt_ArgType  = M68KSIZE_Byte;
			break;
		}

		case 1:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = ( ShortOpcodes ) ? "Cmp.w" : "Cmpi.w" ;
			rt->rt_CPU.M68k.mt_ArgType  = M68KSIZE_Word;
			break;
		}

		case 2:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = ( ShortOpcodes ) ? "Cmp.l" : "Cmpi.l" ;
			rt->rt_CPU.M68k.mt_ArgType  = M68KSIZE_Long;
			break;
		}

		default:
		{
			printf( "Unsupported 'Cmpi' Opcode (Mode: %d)\n", size );
			ds = RS4DecodeStat_Error;
			goto bailout;
		}
	}

	// --

	rt->rt_CPU.M68k.mt_ArgEMode = 7;
	rt->rt_CPU.M68k.mt_ArgEReg  = 4;

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
