
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

#include "ReSrc4.h"

// --

void Cmd_LPSTOP( struct M68kStruct *ms )
{
uint16_t val;
uint8_t *mem;

	ms->ms_Str_Opcode = "LPStop";
	ms->ms_OpcodeSize = 6;
	ms->ms_ArgType = OS_Word;

	mem	= ms->ms_MemoryBuf;

	val = (( mem[4] << 8 ) | ( mem[5] << 0 ));

	snprintf( ms->ms_Buf_Argument, 64, "#$%04x", val );
}

// --
