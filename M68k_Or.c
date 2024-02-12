
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

void Cmd_OR( struct M68kStruct *ms )
{
uint32_t opmode;

	opmode = ( ms->ms_Opcode & 0x01c00000 ) >> 22;

	switch( opmode )
	{
		case 0:
		{
			ms->ms_Str_Opcode = "Or.b";
			ms->ms_ArgType  = OS_Byte;
			ms->ms_ArgEMode = ( ms->ms_Opcode & 0x00380000 ) >> 19;
			ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x00070000 ) >> 16;

			ms->ms_CurRegister = & ms->ms_SrcRegister;

			M68k_EffectiveAddress( ms );

			ms->ms_ArgEMode = 0x00; // Dx Reg
			ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x0e000000 ) >> 25;

			ms->ms_CurRegister = & ms->ms_DstRegister;

			M68k_EffectiveAddress( ms );

			ms->ms_CurRegister->mr_Type = RT_Unknown;
			ms->ms_OpcodeSize = ms->ms_ArgSize;
			break;
		}

		case 1:
		{
			ms->ms_Str_Opcode = "Or.w";

			ms->ms_ArgType  = OS_Word;
			ms->ms_ArgEMode = ( ms->ms_Opcode & 0x00380000 ) >> 19;
			ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x00070000 ) >> 16;

			ms->ms_CurRegister = & ms->ms_SrcRegister;

			M68k_EffectiveAddress( ms );

			ms->ms_ArgEMode = 0x00; // Dx Reg
			ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x0e000000 ) >> 25;

			ms->ms_CurRegister = & ms->ms_DstRegister;

			M68k_EffectiveAddress( ms );

			ms->ms_CurRegister->mr_Type = RT_Unknown;
			ms->ms_OpcodeSize = ms->ms_ArgSize;
			break;
		}

		case 2:
		{
			ms->ms_Str_Opcode = "Or.l";

			ms->ms_ArgType  = OS_Long;
			ms->ms_ArgEMode = ( ms->ms_Opcode & 0x00380000 ) >> 19;
			ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x00070000 ) >> 16;

			ms->ms_CurRegister = & ms->ms_SrcRegister;

			M68k_EffectiveAddress( ms );

			ms->ms_ArgEMode = 0x00; // Dx Reg
			ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x0e000000 ) >> 25;

			ms->ms_CurRegister = & ms->ms_DstRegister;

			M68k_EffectiveAddress( ms );

			ms->ms_CurRegister->mr_Type = RT_Unknown;
			ms->ms_OpcodeSize = ms->ms_ArgSize;
			break;
		}

		// 3 = Divu.w

		case 4:
		{
			ms->ms_Str_Opcode = "Or.b";

			ms->ms_ArgType  = OS_Byte;
			ms->ms_ArgEMode = 0x00; // Dx Reg
			ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x0e000000 ) >> 25;

			ms->ms_CurRegister = & ms->ms_SrcRegister;

			M68k_EffectiveAddress( ms );

			ms->ms_ArgEMode = ( ms->ms_Opcode & 0x00380000 ) >> 19;
			ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x00070000 ) >> 16;

			ms->ms_CurRegister = & ms->ms_DstRegister;

			M68k_EffectiveAddress( ms );

			ms->ms_CurRegister->mr_Type = RT_Unknown;
			ms->ms_OpcodeSize = ms->ms_ArgSize;
			break;
		}

		case 5:
		{
			ms->ms_Str_Opcode = "Or.w";

			ms->ms_ArgType  = OS_Word;
			ms->ms_ArgEMode = 0x00; // Dx Reg
			ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x0e000000 ) >> 25;

			ms->ms_CurRegister = & ms->ms_SrcRegister;

			M68k_EffectiveAddress( ms );

			ms->ms_ArgEMode = ( ms->ms_Opcode & 0x00380000 ) >> 19;
			ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x00070000 ) >> 16;

			ms->ms_CurRegister = & ms->ms_DstRegister;

			M68k_EffectiveAddress( ms );

			ms->ms_CurRegister->mr_Type = RT_Unknown;
			ms->ms_OpcodeSize = ms->ms_ArgSize;
			break;
		}

		case 6:
		{
			ms->ms_Str_Opcode = "Or.l";

			ms->ms_ArgType  = OS_Long;
			ms->ms_ArgEMode = 0x00; // Dx Reg
			ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x0e000000 ) >> 25;

			ms->ms_CurRegister = & ms->ms_SrcRegister;

			M68k_EffectiveAddress( ms );

			ms->ms_ArgEMode = ( ms->ms_Opcode & 0x00380000 ) >> 19;
			ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x00070000 ) >> 16;

			ms->ms_CurRegister = & ms->ms_DstRegister;

			M68k_EffectiveAddress( ms );

			ms->ms_CurRegister->mr_Type = RT_Unknown;
			ms->ms_OpcodeSize = ms->ms_ArgSize;
			break;
		}

		// 7 : Divs.w

		default:
		{
			printf( "Unsupported 'Or' Opcode (Mode: %d)\n", opmode );
			ms->ms_DecodeStatus = MSTAT_Error;
			break;
		}
	}
}
