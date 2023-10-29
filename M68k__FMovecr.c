
/*
 * Copyright (c) 2014-2023 Rene W. Olsen < renewolsen @ gmail . com >
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

void Cmd_FMOVECR( struct M68kStruct *ms )
{
int reg;
int rom;

	reg  = ( ms->ms_Opcode & 0x00000380 ) >> 7;
	rom  = ( ms->ms_Opcode & 0x0000007f );

	ms->ms_Str_Opcode = "FMovecr.x";
	ms->ms_OpcodeSize = 4;


//	printf( "Unsupported 'FMovecr.x' Opcode at %08x\n", ms->ms_MemoryAdr );

	sprintf( ms->ms_Buf_Argument, "#$%02x,%s", rom, FPx_RegNames[reg] );
}
