
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

enum RS4DecodeStat M68kCmd_NEGX( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
int emode;
int ereg;
int size;

	size	= ( rt->rt_CPU.M68k.mt_Opcode & 0x00c00000 ) >> 22;
	emode	= ( rt->rt_CPU.M68k.mt_Opcode & 0x00380000 ) >> 19;
	ereg	= ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;

	switch( size )
	{
		case 0:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Negx.b";
			rt->rt_CPU.M68k.mt_ArgType  = M68KSIZE_Byte;
			break;
		}

		case 1:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Negx.w";
			rt->rt_CPU.M68k.mt_ArgType  = M68KSIZE_Word;
			break;
		}

		case 2:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Negx.l";
			rt->rt_CPU.M68k.mt_ArgType  = M68KSIZE_Long;
			break;
		}

		default:
		{
			printf( "Unsupported 'Negx' Opcode (Mode: %d)\n", size );
			ds = RS4DecodeStat_Error;
			goto bailout;
		}
	}

	rt->rt_CPU.M68k.mt_ArgEMode = emode;
	rt->rt_CPU.M68k.mt_ArgEReg  = ereg;

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
