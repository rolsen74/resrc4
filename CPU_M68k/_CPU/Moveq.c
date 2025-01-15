
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

enum RS4DecodeStat M68kCmd_MOVEQ( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
uint32_t reg;
int8_t value;

	reg		= ( rt->rt_CPU.M68k.mt_Opcode & 0x0e000000 ) >> 25;
	value	= ( rt->rt_CPU.M68k.mt_Opcode & 0x00ff0000 ) >> 16;

	rt->rt_Container.Hunk.ms_Str_Opcode = ( ShortOpcodes ) ? "Moveq" : "Moveq.l";
	sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "#%d,%s", value, Dx_RegNames[reg] );

	rt->rt_CPU.M68k.mt_ClearRegMask |= 1U << ( M68KREGT_D0 + reg ); 

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
