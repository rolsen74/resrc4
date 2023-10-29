
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

void Cmd_EXG( struct M68kStruct *ms )
{
struct M68kRegister tmp;
int mode;
int yreg;
int xreg;

	xreg = ( ms->ms_Opcode & 0x0e000000 ) >> 25;
	mode = ( ms->ms_Opcode & 0x00f80000 ) >> 19;
	yreg = ( ms->ms_Opcode & 0x00070000 ) >> 16;

	switch( mode )
	{
		case 8:
		{
			tmp = ms->ms_Registers[ REG_Dx + xreg ];
			ms->ms_Registers[ REG_Dx + xreg ] = ms->ms_Registers[ REG_Dx + yreg ];
			ms->ms_Registers[ REG_Dx + yreg ] = tmp;

			// --

			ms->ms_Str_Opcode = "Exg.l";
			sprintf( ms->ms_Buf_Argument, "%s,%s", Dx_RegNames[xreg], Dx_RegNames[yreg] );
			break;
		}

		case 9:
		{
			tmp = ms->ms_Registers[ REG_Ax + xreg ];
			ms->ms_Registers[ REG_Ax + xreg ] = ms->ms_Registers[ REG_Ax + yreg ];
			ms->ms_Registers[ REG_Ax + yreg ] = tmp;

			// --

			ms->ms_Str_Opcode = "Exg.l";
			sprintf( ms->ms_Buf_Argument, "%s,%s", Ax_RegNames[xreg], Ax_RegNames[yreg] );
			break;
		}

		case 17:
		{
			tmp = ms->ms_Registers[ REG_Dx + xreg ];
			ms->ms_Registers[ REG_Dx + xreg ] = ms->ms_Registers[ REG_Ax + yreg ];
			ms->ms_Registers[ REG_Ax + yreg ] = tmp;

			// --

			ms->ms_Str_Opcode = "Exg.l";
			sprintf( ms->ms_Buf_Argument, "%s,%s", Dx_RegNames[xreg], Ax_RegNames[yreg] );
			break;
		}

		default:	
		{
			printf( "Unsupported 'Exg' Opcode (Mode: %d)\n", mode );
			ms->ms_DecodeStatus = MSTAT_Error;
			break;
		}
	}
}
