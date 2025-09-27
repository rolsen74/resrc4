
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

S32 leatst = 0;

enum RS4DecodeStat M68kCmd_LEA( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
struct M68kRegister *src;

	rt->rt_Container.Hunk.ms_Str_Opcode = "Lea";

	leatst = 1;

	// --

	rt->rt_CPU.M68k.mt_ArgType		= M68KSIZE_Long;
	rt->rt_CPU.M68k.mt_DoLabelSize	= FALSE;
	rt->rt_CPU.M68k.mt_ArgEMode		= ( rt->rt_CPU.M68k.mt_Opcode & 0x00380000 ) >> 19;
	rt->rt_CPU.M68k.mt_ArgEReg		= ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;
	rt->rt_CPU.M68k.mt_CurRegister	= & rt->rt_CPU.M68k.mt_SrcRegister;

	ds = M68k_EffectiveAddress( & ec, rt );

	if ( ds != RS4DecodeStat_Okay )
	{
		// ec allready set

		#ifdef DEBUG
		printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	src = rt->rt_CPU.M68k.mt_CurRegister;

	// --

	rt->rt_CPU.M68k.mt_ArgEMode		= 0x01; // Ax Reg
	rt->rt_CPU.M68k.mt_ArgEReg		= ( rt->rt_CPU.M68k.mt_Opcode & 0x0e000000 ) >> 25;
	rt->rt_CPU.M68k.mt_CurRegister	= & rt->rt_CPU.M68k.mt_DstRegister;

	ds = M68k_EffectiveAddress( & ec, rt );

	if ( ds != RS4DecodeStat_Okay )
	{
		// ec allready set

		#ifdef DEBUG
		printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	memcpy( rt->rt_CPU.M68k.mt_CurRegister, src, sizeof( struct M68kRegister ));
	rt->rt_CPU.M68k.mt_Registers[ M68KREGT_Ax + rt->rt_CPU.M68k.mt_ArgEReg ] = rt->rt_CPU.M68k.mt_SrcRegister;
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

	leatst = 0;

	return( ds );
}
