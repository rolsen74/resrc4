
/*
 * Copyright (c) 2014-2023 Rene W. Olsen <renewolsen@gmail.com>
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

void Cmd_JSR( struct M68kStruct *ms )
{
struct HunkRef *isRef;

	ms->ms_Str_Opcode = "Jsr";

	// --

	ms->ms_ArgType  = OS_Long;
	ms->ms_ArgEMode = ( ms->ms_Opcode & 0x00380000 ) >> 19;
	ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x00070000 ) >> 16;

	ms->ms_CurRegister = & ms->ms_JmpRegister;
	ms->ms_LibCall = 1;

	isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

	if ( M68k_EffectiveAddress( ms, isRef, LT_Code ))
	{
		isRef->hr_Used = true;
	}

	// --

	if (( ms->ms_ArgEMode == 5 ) && ( ms->ms_ArgEReg == 6 ))
	{
		// (d16,An)
		ms->ms_ClearRegs = CR_Function;
	}
	else
	{
		ms->ms_ClearRegs = CR_All;
	}

	ms->ms_OpcodeSize = ms->ms_ArgSize;
}
