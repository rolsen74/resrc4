
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

void Cmd_MOVEP( struct M68kStruct *ms )
{
struct HunkRef *isRef;
int mode;
int areg;
int dreg;

	areg = ( ms->ms_Opcode & 0x00070000 ) >> 16;
	dreg = ( ms->ms_Opcode & 0x07000000 ) >> 24;
	mode = ( ms->ms_Opcode & 0x00e00000 ) >> 21;

	switch( mode )
	{
		case 4:
		{
			ms->ms_Str_Opcode = "Movep.w";
			ms->ms_ArgType  = OS_Word;
	
			ms->ms_ArgEReg	= areg;
			ms->ms_ArgEMode	= 0x50;
	
			ms->ms_CurRegister = & ms->ms_SrcRegister;

			isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

			if ( M68k_EffectiveAddress( ms, isRef, 0 ))
			{
				isRef->hr_Used = true;
			}

			// --

			ms->ms_ArgEReg	= dreg;
			ms->ms_ArgEMode	= 0x00;

			ms->ms_CurRegister = & ms->ms_DstRegister;

			isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

			if ( M68k_EffectiveAddress( ms, isRef, 0 ))
			{
				isRef->hr_Used = true;
			}

			ms->ms_CurRegister->mr_Type = RT_Unknown;
			break;
		}

		case 5:
		{
			ms->ms_Str_Opcode = "Movep.l";
			ms->ms_ArgType  = OS_Long;

			ms->ms_ArgEReg	= areg;
			ms->ms_ArgEMode	= 0x50;
	
			ms->ms_CurRegister = & ms->ms_SrcRegister;

			isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

			if ( M68k_EffectiveAddress( ms, isRef, 0 ))
			{
				isRef->hr_Used = true;
			}

			// --

			ms->ms_ArgEReg	= dreg;
			ms->ms_ArgEMode	= 0x00;

			ms->ms_CurRegister = & ms->ms_DstRegister;

			isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

			if ( M68k_EffectiveAddress( ms, isRef, 0 ))
			{
				isRef->hr_Used = true;
			}

			ms->ms_CurRegister->mr_Type = RT_Unknown;
			break;
		}

		case 6:
		{
			ms->ms_Str_Opcode = "Movep.w";
			ms->ms_ArgType  = OS_Word;

			ms->ms_ArgEReg	= dreg;
			ms->ms_ArgEMode	= 0x00;
	
			ms->ms_CurRegister = & ms->ms_SrcRegister;

			isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

			if ( M68k_EffectiveAddress( ms, isRef, 0 ))
			{
				isRef->hr_Used = true;
			}

			// --

			ms->ms_ArgEReg	= areg;
			ms->ms_ArgEMode	= 0x50;

			ms->ms_CurRegister = & ms->ms_DstRegister;

			isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

			if ( M68k_EffectiveAddress( ms, isRef, 0 ))
			{
				isRef->hr_Used = true;
			}

			ms->ms_CurRegister->mr_Type = RT_Unknown;
			break;
		}

		case 7:
		{
			ms->ms_Str_Opcode = "Movep.l";
			ms->ms_ArgType  = OS_Long;

			ms->ms_ArgEReg	= dreg;
			ms->ms_ArgEMode	= 0x00;
	
			ms->ms_CurRegister = & ms->ms_SrcRegister;

			isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

			if ( M68k_EffectiveAddress( ms, isRef, 0 ))
			{
				isRef->hr_Used = true;
			}

			// --

			ms->ms_ArgEReg	= areg;
			ms->ms_ArgEMode	= 0x50;

			ms->ms_CurRegister = & ms->ms_DstRegister;

			isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

			if ( M68k_EffectiveAddress( ms, isRef, 0 ))
			{
				isRef->hr_Used = true;
			}

			ms->ms_CurRegister->mr_Type = RT_Unknown;
			break;
		}

		default:
		{
			printf( "Unsupported 'Movep' Opcode (Mode: %d)\n", mode );
			ms->ms_DecodeStatus = MSTAT_Error;
			goto bailout;
		}
	}

    // --

	ms->ms_OpcodeSize = 4;

bailout:
    
    return;
}
