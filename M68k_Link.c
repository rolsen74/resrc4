 
/*
 * Copyright (c) 2014-2023 Rene W. Olsen <renewolsen@gmail.com>
 * All rights reserved.
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

void Cmd_LINK( struct M68kStruct *ms )
{
int16_t link;
int reg;

	reg  = ( ms->ms_Opcode & 0x00070000 ) >> 16;
	link = ( ms->ms_Opcode & 0x0000ffff );

	ms->ms_Str_Opcode = "Link.w";
	sprintf( ms->ms_Buf_Argument, "%s,#%d", Ax_RegNames[reg], link );

	ms->ms_OpcodeSize = 4;
	ms->ms_Registers[ REG_A7 ].mr_Type = RT_Unknown;
}

// --

void Cmd_LINK_L( struct M68kStruct *ms )
{
uint32_t link;
int reg;

	reg  =  ( ms->ms_Opcode & 0x00070000 ) >> 16;

	link =	(( ms->ms_MemoryBuf[2] << 24 ) |
			 ( ms->ms_MemoryBuf[3] << 16 ) |
			 ( ms->ms_MemoryBuf[4] <<  8 ) |
			 ( ms->ms_MemoryBuf[5] <<  0 ));

	ms->ms_Str_Opcode = "Link.l";
	sprintf( ms->ms_Buf_Argument, "%s,#%d", Ax_RegNames[reg], link );

	ms->ms_OpcodeSize = 6;
	ms->ms_Registers[ REG_A7 ].mr_Type = RT_Unknown;
}

// --