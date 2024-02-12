
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

void Cmd_BCHG( struct M68kStruct *ms )
{
int nr;

	ms->ms_Str_Opcode = "BChg";

	nr = ms->ms_Opcode & 0x000000ff;

	sprintf( ms->ms_Buf_Argument, "#$%02x", nr );

	// --

	ms->ms_ArgSize  = 4;
	ms->ms_ArgType  = OS_Byte;
	ms->ms_ArgEMode = ( ms->ms_Opcode & 0x00380000 ) >> 19;
	ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x00070000 ) >> 16;

	ms->ms_CurRegister = & ms->ms_DstRegister;

	M68k_EffectiveAddress( ms );

	// --

	ms->ms_CurRegister->mr_Type = RT_Unknown;
	ms->ms_OpcodeSize = ms->ms_ArgSize;
}

void Cmd_BCHG2( struct M68kStruct *ms )
{
int reg;

	ms->ms_Str_Opcode = "BChg";

	reg = ( ms->ms_Opcode & 0x0e000000 ) >> 25;

	sprintf( ms->ms_Buf_Argument, "%s", Dx_RegNames[reg] );

	// --

	ms->ms_ArgType  = OS_Byte;
	ms->ms_ArgEMode = ( ms->ms_Opcode & 0x00380000 ) >> 19;
	ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x00070000 ) >> 16;

	ms->ms_CurRegister = & ms->ms_DstRegister;

	M68k_EffectiveAddress( ms );

	// --

	ms->ms_CurRegister->mr_Type = RT_Unknown;
	ms->ms_OpcodeSize = ms->ms_ArgSize;
}

// --
