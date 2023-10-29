
/*
 * Copyright (c) 2014-2023 Rene W. Olsen < renewolsen @ gmail . com >
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

void Cmd_ABCD( struct M68kStruct *ms )
{
int rx;
int ry;
int rm;

	ms->ms_Str_Opcode = "Abcd.b";
	ms->ms_ArgType  = OS_Byte;

	rx = ( ms->ms_Opcode & 0x0e000000 ) >> 25;
	rm = ( ms->ms_Opcode & 0x00080000 ) >> 19;
	ry = ( ms->ms_Opcode & 0x00070000 ) >> 16;

	if ( rm )
	{
		sprintf( ms->ms_Buf_Argument, "-(%s),-(%s)", Ax_RegNames[ ry ], Ax_RegNames[ rx ] );
	}
	else
	{
		sprintf( ms->ms_Buf_Argument, "%s,%s", Dx_RegNames[ ry ], Dx_RegNames[ rx ] );
		ms->ms_Registers[ rx ].mr_Type = RT_Unknown;
	}
}

// --
