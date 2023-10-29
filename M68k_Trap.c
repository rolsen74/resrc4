
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

void Cmd_TRAP( struct M68kStruct *ms )
{
int vec;

	vec = ( ms->ms_Opcode & 0x000f0000 ) >> 16;

	ms->ms_Str_Opcode = "Trap";
	sprintf( ms->ms_Buf_Argument, "#%d", vec );
}

// --
