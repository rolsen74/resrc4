
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

void Cmd_MOVE( struct M68kStruct *ms )
{
struct Misc_Move_GetSetStruct gss;
uint32_t size;
uint8_t *mem;

	size = ( ms->ms_Opcode & 0x30000000 ) >> 28;

	switch( size )
	{
		case 1:
		{
			ms->ms_Str_Opcode = "Move.b";
			ms->ms_ArgType = OS_Byte;

			// --

			ms->ms_ArgEMode	= ( ms->ms_Opcode & 0x00380000 ) >> 19;
			ms->ms_ArgEReg	= ( ms->ms_Opcode & 0x00070000 ) >> 16;

			ms->ms_CurRegister = & ms->ms_SrcRegister;

			M68k_EffectiveAddress( ms );

			// --

			ms->ms_ArgEReg	= ( ms->ms_Opcode & 0x0e000000 ) >> 25;
			ms->ms_ArgEMode	= ( ms->ms_Opcode & 0x01c00000 ) >> 22;

			ms->ms_CurRegister = & ms->ms_DstRegister;

			M68k_EffectiveAddress( ms );

			// --

			ms->ms_OpcodeSize = ms->ms_ArgSize;
			break;
		}

		case 3:
		{
			ms->ms_Str_Opcode = "Move.w";
			ms->ms_ArgType  = OS_Word;

			// --

			ms->ms_ArgEMode	= ( ms->ms_Opcode & 0x00380000 ) >> 19;
			ms->ms_ArgEReg	= ( ms->ms_Opcode & 0x00070000 ) >> 16;

			ms->ms_CurRegister = & ms->ms_SrcRegister;

			M68k_EffectiveAddress( ms );

			// --

			ms->ms_ArgEReg	= ( ms->ms_Opcode & 0x0e000000 ) >> 25;
			ms->ms_ArgEMode	= ( ms->ms_Opcode & 0x01c00000 ) >> 22;

			ms->ms_CurRegister = & ms->ms_DstRegister;

			M68k_EffectiveAddress( ms );

			// --

			ms->ms_OpcodeSize = ms->ms_ArgSize;
			break;
		}

		case 2:
		{
			ms->ms_Str_Opcode = "Move.l";
			ms->ms_ArgType  = OS_Long;

			// --

			ms->ms_ArgEMode	= ( ms->ms_Opcode & 0x00380000 ) >> 19;
			ms->ms_ArgEReg	= ( ms->ms_Opcode & 0x00070000 ) >> 16;

			ms->ms_CurRegister = & ms->ms_SrcRegister;

			mem = & ms->ms_MemoryBuf[ ms->ms_ArgSize ];

			M68k_EffectiveAddress( ms );

			Misc_Move_Get( ms, ms->ms_CurRegister, mem, & gss );

			// --

			ms->ms_ArgEReg	= ( ms->ms_Opcode & 0x0e000000 ) >> 25;
			ms->ms_ArgEMode	= ( ms->ms_Opcode & 0x01c00000 ) >> 22;

			ms->ms_CurRegister = & ms->ms_DstRegister;

			mem = & ms->ms_MemoryBuf[ ms->ms_ArgSize ];

			M68k_EffectiveAddress( ms );

			Misc_Move_Set( ms, ms->ms_CurRegister, mem, & gss );

			// --

			ms->ms_OpcodeSize = ms->ms_ArgSize;
			break;
		}

		default:
		{
			printf( "Unsupported 'Move' Opcode (Mode: %d)\n", size );
			ms->ms_DecodeStatus = MSTAT_Error;
			goto bailout;
		}
	}

    // --

bailout:

    return;
}
