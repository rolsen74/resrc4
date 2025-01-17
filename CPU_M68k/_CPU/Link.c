 
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

enum RS4DecodeStat M68kCmd_LINK( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
int16_t link;
int reg;

	reg  = ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;
	link = ( rt->rt_CPU.M68k.mt_Opcode & 0x0000ffff );

	rt->rt_Container.Hunk.ms_Str_Opcode = "Link.w";
	sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "%s,#%d", Ax_RegNames[reg], link );

	rt->rt_CPU.M68k.mt_OpcodeSize = 4;
	rt->rt_CPU.M68k.mt_ClearRegMask |= ( 1 << M68KREGT_A7 ); 

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

enum RS4DecodeStat M68kCmd_LINK_L( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
uint32_t link;
int reg;

	reg  =  ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;

	link =	(( rt->rt_CurMemBuf[2] << 24 ) |
			 ( rt->rt_CurMemBuf[3] << 16 ) |
			 ( rt->rt_CurMemBuf[4] <<  8 ) |
			 ( rt->rt_CurMemBuf[5] <<  0 ));

	rt->rt_Container.Hunk.ms_Str_Opcode = "Link.l";
	sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "%s,#%d", Ax_RegNames[reg], link );

	rt->rt_CPU.M68k.mt_OpcodeSize = 6;
	rt->rt_CPU.M68k.mt_ClearRegMask |= ( 1 << M68KREGT_A7 ); 

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