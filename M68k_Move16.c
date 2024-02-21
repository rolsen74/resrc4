
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

void Cmd_MOVE16( struct M68kStruct *ms )
{
uint32_t val;
uint8_t *mem;
char labname[ MAX_LabelName + 8 ];
int mode;
int ay;

	mode	= ( ms->ms_Opcode & 0x00180000 ) >> 19;
	ay		= ( ms->ms_Opcode & 0x00070000 ) >> 16;

	mem		= ms->ms_MemoryBuf;

	val = (( mem[2] << 24 ) + ( mem[3] << 16 )) + (( mem[4] << 8 ) + ( mem[5] << 0 ));

	if ( BuildLabelString2( ms, labname, ms->ms_MemoryAdr + ms->ms_ArgSize, val ))
	{
		printf( "%s:%04d: Label error\n", __FILE__, __LINE__ );
		goto bailout;
	}

	switch( mode )
	{
		case 0:
		{
			snprintf( ms->ms_Buf_Argument, 64, "(%s)+,(%s).l", Ax_RegNames[ay], labname );
			break;
		}

		case 1:
		{
			snprintf( ms->ms_Buf_Argument, 64, "(%s).l,(%s)+", labname, Ax_RegNames[ay] );
			break;
		}

		case 2:
		{
			snprintf( ms->ms_Buf_Argument, 64, "(%s),(%s).l", Ax_RegNames[ay], labname );
			break;
		}

		case 3:
		{
			snprintf( ms->ms_Buf_Argument, 64, "(%s),(%s).l", labname, Ax_RegNames[ay] );
			break;
		}

		default:
		{
			printf( "Unsupported 'Move16' Opcode (Mode: %d)\n", mode );
			ms->ms_DecodeStatus = MSTAT_Error;
			goto bailout;
		}
	}

	ms->ms_Str_Opcode = "Move16";
	ms->ms_OpcodeSize = 6;

bailout:

	return;
}

// --

void Cmd_MOVE162( struct M68kStruct *ms )
{
int ax;
int ay;

	ax = ( ms->ms_Opcode & 0x00070000 ) >> 16;
	ay = ( ms->ms_Opcode & 0x00007000 ) >> 12;

	ms->ms_Str_Opcode = "Move16";
	ms->ms_OpcodeSize = 4;

	sprintf( ms->ms_Buf_Argument, "(%s)+,(%s)+", Ax_RegNames[ax], Ax_RegNames[ay] );
}

// --