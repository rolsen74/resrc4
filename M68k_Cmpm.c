
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

void Cmd_CMPM( struct M68kStruct *ms )
{
int size;
int yreg;
int xreg;

	xreg = ( ms->ms_Opcode & 0x0e000000 ) >> 25;
	size = ( ms->ms_Opcode & 0x00c00000 ) >> 22;
	yreg = ( ms->ms_Opcode & 0x00070000 ) >> 16;

	switch( size )
	{
		case 0:
		{
			ms->ms_Str_Opcode = "Cmpm.b";
			sprintf( ms->ms_Buf_Argument, "(%s)+,(%s)+", Ax_RegNames[yreg], Ax_RegNames[xreg] );
			break;
		}

		case 1:
		{
			ms->ms_Str_Opcode = "Cmpm.w";
			sprintf( ms->ms_Buf_Argument, "(%s)+,(%s)+", Ax_RegNames[yreg], Ax_RegNames[xreg] );
			break;
		}

		case 2:
		{
			ms->ms_Str_Opcode = "Cmpm.l";
			sprintf( ms->ms_Buf_Argument, "(%s)+,(%s)+", Ax_RegNames[yreg], Ax_RegNames[xreg] );
			break;
		}

		default:	
		{
			printf( "Unsupported 'Cmpm' Opcode (Mode: %d)\n", size );
			ms->ms_DecodeStatus = MSTAT_Error;
			break;
		}
	}
}
