
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

enum RS4DecodeStat M68kCmd_STOP( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
U16 val;

	rt->rt_Container.Hunk.ms_Str_Opcode = "Stop";
	rt->rt_CPU.M68k.mt_OpcodeSize = 4;

	val = ( rt->rt_CPU.M68k.mt_Opcode & 0x0000ffff );

	snprintf( rt->rt_Container.Hunk.ms_Buf_Argument, 64, "#$%04x", val );

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
