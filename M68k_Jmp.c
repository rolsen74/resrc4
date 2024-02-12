
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

void Cmd_JMP( struct M68kStruct *ms )
{
	ms->ms_Str_Opcode = "Jmp";

	// --

	ms->ms_ArgType	= OS_Long;
	ms->ms_ArgEMode	= ( ms->ms_Opcode & 0x00380000 ) >> 19;
	ms->ms_ArgEReg	= ( ms->ms_Opcode & 0x00070000 ) >> 16;

	ms->ms_CurRegister = & ms->ms_JmpRegister;
	ms->ms_LibCall = 1;

	M68k_EffectiveAddress( ms );

	// --

	if (( ms->ms_ArgEMode == 7 ) && ( ms->ms_ArgEReg == 3 ))
	{
		ms->ms_JumpTable = true;
	}

	ms->ms_OpcodeSize = ms->ms_ArgSize;
	ms->ms_LastOpcode = true;
}
