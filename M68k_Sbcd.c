
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

void Cmd_SBCD( struct M68kStruct *ms )
{
int rx;
int ry;
int rm;

	ms->ms_Str_Opcode = "Sbcd.b";

	ry = ( ms->ms_Opcode & 0x0e000000 ) >> 25;
	rm = ( ms->ms_Opcode & 0x00080000 ) >> 19;
	rx = ( ms->ms_Opcode & 0x00070000 ) >> 16;

	if ( rm )
	{
		sprintf( ms->ms_Buf_Argument, "-(%s),-(%s)", Ax_RegNames[ rx ], Ax_RegNames[ ry ] );
	}
	else
	{
		sprintf( ms->ms_Buf_Argument, "%s,%s", Dx_RegNames[ rx ], Dx_RegNames[ ry ] );
		ms->ms_Registers[ ry ].mr_Type = RT_Unknown;
	}

}

// --
