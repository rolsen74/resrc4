 
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

void Cmd_UNLK( struct M68kStruct *ms )
{
int reg;

	reg = ( ms->ms_Opcode & 0x00070000 ) >> 16;

	ms->ms_Str_Opcode = "Unlk";
	sprintf( ms->ms_Buf_Argument, "%s", Ax_RegNames[reg] );

	ms->ms_Registers[REG_Ax+reg].mr_Type = RT_Unknown;
	ms->ms_OpcodeSize = 2;
}

// --
