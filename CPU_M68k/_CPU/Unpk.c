
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

enum RS4DecodeStat M68kCmd_UNPK( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
uint16_t val;
int rx;
int ry;
int rm;

	rt->rt_Container.Hunk.ms_Str_Opcode = "Unpk";

	ry = ( rt->rt_CPU.M68k.mt_Opcode & 0x0e000000 ) >> 25;
	rm = ( rt->rt_CPU.M68k.mt_Opcode & 0x00080000 ) >> 19;
	rx = ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;

	val = ( rt->rt_CPU.M68k.mt_Opcode & 0x0000ffff );

	if ( rm )
	{
		rt->rt_CPU.M68k.mt_ClearRegMask |= 1U << ( M68KREGT_A0 + rx ); 
		rt->rt_CPU.M68k.mt_ClearRegMask |= 1U << ( M68KREGT_A0 + ry ); 
		sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "-(%s),-(%s),#%04x", Ax_RegNames[ rx ], Ax_RegNames[ ry ], val );
	}
	else
	{
		rt->rt_CPU.M68k.mt_ClearRegMask |= 1U << ( M68KREGT_D0 + rx ); 
		rt->rt_CPU.M68k.mt_ClearRegMask |= 1U << ( M68KREGT_D0 + ry ); 
		sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "%s,%s,#%04x", Dx_RegNames[ rx ], Dx_RegNames[ ry ], val );
	}

	rt->rt_CPU.M68k.mt_OpcodeSize = 4;

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
