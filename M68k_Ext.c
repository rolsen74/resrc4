
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

void Cmd_EXT( struct M68kStruct *ms )
{
uint32_t opmode;
uint32_t reg;

	opmode	= ( ms->ms_Opcode & 0x01c00000 ) >> 22;
	reg		= ( ms->ms_Opcode & 0x00070000 ) >> 16;

	switch( opmode )
	{
		case 2:
		{
			ms->ms_Str_Opcode = "Ext.w";
			break;
		}

		case 3:
		{
			ms->ms_Str_Opcode = "Ext.l";
			break;
		}

		case 7:
		{
			ms->ms_Str_Opcode = "Extb.l";
			break;
		}

		default:
		{
			printf( "Unsupported 'Ext' Opcode (Mode: %d)\n", opmode );
			ms->ms_DecodeStatus = MSTAT_Error;
			break;
		}
	}

	sprintf( ms->ms_Buf_Argument, "%s", Dx_RegNames[ reg ] );

	ms->ms_Registers[reg].mr_Type = RT_Unknown;
	ms->ms_OpcodeSize = 2;
}
