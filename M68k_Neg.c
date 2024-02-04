
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

void Cmd_NEG( struct M68kStruct *ms )
{
struct HunkRef *isRef;
int size;

	size = ( ms->ms_Opcode & 0x00c00000 ) >> 22;

	switch( size )
	{
		case 0:
		{
			ms->ms_Str_Opcode = "Neg.b";

			ms->ms_ArgType  = OS_Byte;
			ms->ms_ArgEMode = ( ms->ms_Opcode & 0x00380000 ) >> 19;
			ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x00070000 ) >> 16;

			ms->ms_CurRegister = & ms->ms_DstRegister;

			isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

			if ( M68k_EffectiveAddress( ms, isRef, 0 ))
			{
				isRef->hr_Used = true;
			}

			ms->ms_CurRegister->mr_Type = RT_Unknown;
			ms->ms_OpcodeSize = ms->ms_ArgSize;
			break;
		}

		case 1:
		{
			ms->ms_Str_Opcode = "Neg.w";

			ms->ms_ArgType  = OS_Word;
			ms->ms_ArgEMode = ( ms->ms_Opcode & 0x00380000 ) >> 19;
			ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x00070000 ) >> 16;

			ms->ms_CurRegister = & ms->ms_DstRegister;

			isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

			if ( M68k_EffectiveAddress( ms, isRef, 0 ))
			{
				isRef->hr_Used = true;
			}

			ms->ms_CurRegister->mr_Type = RT_Unknown;
			ms->ms_OpcodeSize = ms->ms_ArgSize;
			break;
		}

		case 2:
		{
			ms->ms_Str_Opcode = "Neg.l";

			ms->ms_ArgType  = OS_Long;
			ms->ms_ArgEMode = ( ms->ms_Opcode & 0x00380000 ) >> 19;
			ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x00070000 ) >> 16;

			ms->ms_CurRegister = & ms->ms_DstRegister;

			isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

			if ( M68k_EffectiveAddress( ms, isRef, 0 ))
			{
				isRef->hr_Used = true;
			}

			ms->ms_CurRegister->mr_Type = RT_Unknown;
			ms->ms_OpcodeSize = ms->ms_ArgSize;
			break;
		}

		default:
		{
			printf( "Unsupported 'Neg' Opcode (Mode: %d)\n", size );
			ms->ms_DecodeStatus = MSTAT_Error;
			break;
		}
	}
}

// --
