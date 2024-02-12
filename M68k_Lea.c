
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

void Cmd_LEA( struct M68kStruct *ms )
{
struct M68kRegister *src;

	ms->ms_Str_Opcode = "Lea";

	// --

	ms->ms_ArgType  = OS_Long;
	ms->ms_ArgEMode = ( ms->ms_Opcode & 0x00380000 ) >> 19;
	ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x00070000 ) >> 16;

	ms->ms_CurRegister = & ms->ms_SrcRegister;

	M68k_EffectiveAddress( ms );

	src = ms->ms_CurRegister;

	// --

	ms->ms_ArgEMode = 0x01; // Ax Reg
	ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x0e000000 ) >> 25;

	ms->ms_CurRegister = & ms->ms_DstRegister;

	M68k_EffectiveAddress( ms );

	memcpy( ms->ms_CurRegister, src, sizeof( struct M68kRegister ));

//	ms->ms_Registers[ REG_Ax + ms->ms_ArgEReg ] = ms->ms_SrcRegister;

	// --

	ms->ms_OpcodeSize = ms->ms_ArgSize;
}
