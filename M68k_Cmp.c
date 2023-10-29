
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

void Cmd_CMP( struct M68kStruct *ms )
{
struct HunkRef *isRef;
int opmode;

	opmode = ( ms->ms_Opcode & 0x01c00000 ) >> 22;

	switch( opmode )
	{
		case 0:
		{
			ms->ms_Str_Opcode = "Cmp.b";
			ms->ms_ArgType  = OS_Byte;
			break;
		}

		case 1:
		{
			ms->ms_Str_Opcode = "Cmp.w";
			ms->ms_ArgType  = OS_Word;
			break;
		}

		case 2:
		{
			ms->ms_Str_Opcode = "Cmp.l";
			ms->ms_ArgType  = OS_Long;
			break;
		}

		// 3 : Cmpa.w

		// 4 : Eor.b

		// 5 : Eor.w

		// 6 : Eor.l

		// 7 : Cmpa.l

		default:
		{
			printf( "Unsupported 'Cmp' Opcode (Mode: %d)\n", opmode );
			ms->ms_DecodeStatus = MSTAT_Error;
			goto bailout;
		}
	}

	// --

	ms->ms_ArgEMode = ( ms->ms_Opcode & 0x00380000 ) >> 19;
	ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x00070000 ) >> 16;

	ms->ms_CurRegister = & ms->ms_SrcRegister;

	isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

	if ( M68k_EffectiveAddress( ms, isRef, 0 ))
	{
		isRef->hr_Used = true;
	}

	// --

	ms->ms_ArgEMode = 0x00; // Dx Reg
	ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x0e000000 ) >> 25;

	ms->ms_CurRegister = & ms->ms_DstRegister;

	M68k_EffectiveAddress( ms, NULL, 0 );

	// --

	ms->ms_OpcodeSize = ms->ms_ArgSize;

bailout:

	return;
}
