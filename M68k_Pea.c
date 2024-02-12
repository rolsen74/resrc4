
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

void Cmd_PEA( struct M68kStruct *ms )
{
	ms->ms_Str_Opcode	= "Pea";
	ms->ms_DoExternal	= 0;
	ms->ms_IsPea		= 1;
	ms->ms_ArgType		= OS_Long;
	ms->ms_ArgEMode		= ( ms->ms_Opcode & 0x00380000 ) >> 19;
	ms->ms_ArgEReg		= ( ms->ms_Opcode & 0x00070000 ) >> 16;

	ms->ms_CurRegister = & ms->ms_SrcRegister;

	M68k_EffectiveAddress( ms );

	ms->ms_Registers[ REG_A7 ].mr_Type = RT_Unknown;
	ms->ms_OpcodeSize = ms->ms_ArgSize;
}
