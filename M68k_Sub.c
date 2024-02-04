
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

void Cmd_SUB( struct M68kStruct *ms )
{
struct HunkRef *isRef;
int opmode;

	opmode = ( ms->ms_Opcode & 0x01c00000 ) >> 22;

	switch( opmode )
	{
		case 0:
		{
			ms->ms_Str_Opcode = "Sub.b";

			ms->ms_ArgType  = OS_Byte;
			ms->ms_ArgEMode = ( ms->ms_Opcode & 0x00380000 ) >> 19;
			ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x00070000 ) >> 16;

			ms->ms_CurRegister = & ms->ms_SrcRegister;

			isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

			if ( M68k_EffectiveAddress( ms, isRef, 0 ))
			{
				isRef->hr_Used = true;
			}

			ms->ms_ArgEMode = 0x00; // Dx Reg
			ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x0e000000 ) >> 25;

			ms->ms_CurRegister = & ms->ms_DstRegister;

			M68k_EffectiveAddress( ms, NULL, 0 );

			ms->ms_CurRegister->mr_Type = RT_Unknown;
			ms->ms_OpcodeSize = ms->ms_ArgSize;
			break;
		}

		case 1:
		{
			ms->ms_Str_Opcode = "Sub.w";

			ms->ms_ArgType  = OS_Word;
			ms->ms_ArgEMode = ( ms->ms_Opcode & 0x00380000 ) >> 19;
			ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x00070000 ) >> 16;

			ms->ms_CurRegister = & ms->ms_SrcRegister;

			isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

			if ( M68k_EffectiveAddress( ms, isRef, 0 ))
			{
				isRef->hr_Used = true;
			}

			ms->ms_ArgEMode = 0x00; // Dx Reg
			ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x0e000000 ) >> 25;

			ms->ms_CurRegister = & ms->ms_DstRegister;

			M68k_EffectiveAddress( ms, NULL, 0 );

			ms->ms_CurRegister->mr_Type = RT_Unknown;
			ms->ms_OpcodeSize = ms->ms_ArgSize;
			break;
		}

		case 2:
		{
			ms->ms_Str_Opcode = "Sub.l";

			ms->ms_ArgType  = OS_Long;
			ms->ms_ArgEMode = ( ms->ms_Opcode & 0x00380000 ) >> 19;
			ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x00070000 ) >> 16;

			ms->ms_CurRegister = & ms->ms_SrcRegister;

			isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

			if ( M68k_EffectiveAddress( ms, isRef, 0 ))
			{
				isRef->hr_Used = true;
			}

			ms->ms_ArgEMode = 0x00; // Dx Reg
			ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x0e000000 ) >> 25;

			ms->ms_CurRegister = & ms->ms_DstRegister;

			M68k_EffectiveAddress( ms, NULL, 0 );

			ms->ms_CurRegister->mr_Type = RT_Unknown;
			ms->ms_OpcodeSize = ms->ms_ArgSize;
			break;
		}

		// 3 : Suba.w

		case 4:
		{
			ms->ms_Str_Opcode = "Sub.b";

			ms->ms_ArgType  = OS_Byte;
			ms->ms_ArgEMode = 0x00; // Dx Reg
			ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x0e000000 ) >> 25;

			ms->ms_CurRegister = & ms->ms_SrcRegister;

			M68k_EffectiveAddress( ms, NULL, 0 );

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

		case 5:
		{
			ms->ms_Str_Opcode = "Sub.w";

			ms->ms_ArgType  = OS_Word;
			ms->ms_ArgEMode = 0x00; // Dx Reg
			ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x0e000000 ) >> 25;

			ms->ms_CurRegister = & ms->ms_SrcRegister;

			M68k_EffectiveAddress( ms, NULL, 0 );

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

		case 6:
		{
			ms->ms_Str_Opcode = "Sub.l";

			ms->ms_ArgType  = OS_Long;
			ms->ms_ArgEMode = 0x00; // Dx Reg
			ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x0e000000 ) >> 25;

			ms->ms_CurRegister = & ms->ms_SrcRegister;

			M68k_EffectiveAddress( ms, NULL, 0 );

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

		// 7 : Suba.l

		default:
		{
			printf( "Unsupported 'Sub' Opcode (Mode: %d)\n", opmode );
			ms->ms_DecodeStatus = MSTAT_Error;
			break;
		}
	}
}
