
/*
 * Copyright (c) 2014-2023 Rene W. Olsen <renewolsen@gmail.com>
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

void Cmd_MOVEQ( struct M68kStruct *ms )
{
uint32_t reg;
int8_t value;

	reg		= ( ms->ms_Opcode & 0x0e000000 ) >> 25;
	value	= ( ms->ms_Opcode & 0x00ff0000 ) >> 16;

	ms->ms_Str_Opcode = ( ShortOpcodes ) ? "Moveq" : "Moveq.l";
	sprintf( ms->ms_Buf_Argument, "#%d,%s", value, Dx_RegNames[reg] );

	ms->ms_Registers[ REG_Dx + reg ].mr_Type = RT_Unknown;
}
