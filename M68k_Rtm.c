
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

void Cmd_RTM( struct M68kStruct *ms )
{
int reg;
int da;

	ms->ms_Str_Opcode = "Rtm";
	ms->ms_LastOpcode = true;

	reg = ( ms->ms_Opcode & 0x00070000 ) >> 16;
	da  = ( ms->ms_Opcode & 0x00080000 );

	if ( da )
	{
		sprintf( ms->ms_Buf_Argument, "%s", Ax_RegNames[ reg ] );
	}
	else
	{
		sprintf( ms->ms_Buf_Argument, "%s", Dx_RegNames[ reg ] );
	}
}
