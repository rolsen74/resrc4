
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

	if (( ms->ms_ArgEMode == 5 ) && ( ms->ms_ArgEReg == 6 ))
	{
		// (d16,An)
		// Clear D0-D1 and A0-A1
		ms->ms_ClearRegMask = ( 1 << REG_D0 ) | ( 1 << REG_D1 ) | ( 1 << REG_A0 ) | ( 1 << REG_A1 );
	}
	else
	{
		// Clear all Regs
		ms->ms_ClearRegMask = -1;
	}

	isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

	if ( M68k_EffectiveAddress( ms, isRef, LT_Code ))
	{
		isRef->hr_Used = true;
	}

	// --

	ms->ms_OpcodeSize = ms->ms_ArgSize;
}
