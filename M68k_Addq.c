
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

void Cmd_ADDQ( struct M68kStruct *ms )
{
int data;
int size;

	data = ( ms->ms_Opcode & 0x0e000000 ) >> 25;
	size = ( ms->ms_Opcode & 0x00c00000 ) >> 22;

	if ( data == 0 )
	{
		data = 8;
	}

	switch( size )
	{
		case 0:
		{
			ms->ms_Str_Opcode = "Addq.b";
			ms->ms_ArgType  = OS_Byte;
			break;
		}

		case 1:
    	{
			ms->ms_Str_Opcode = "Addq.w";
			ms->ms_ArgType  = OS_Word;
			break;
		}

		case 2:
		{
			ms->ms_Str_Opcode = "Addq.l";
			ms->ms_ArgType  = OS_Long;
			break;
		}

		default:
		{
			printf( "Unsupported 'Addq' Opcode (Mode: %d)\n", size );
			ms->ms_DecodeStatus = MSTAT_Error;
			goto bailout;
		}
	}

	// --

	sprintf( ms->ms_Buf_Argument, "#%d", data );

	// --

	ms->ms_ArgEMode = ( ms->ms_Opcode & 0x00380000 ) >> 19;
	ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x00070000 ) >> 16;

	ms->ms_CurRegister = & ms->ms_DstRegister;

	M68k_EffectiveAddress( ms );

	ms->ms_CurRegister->mr_Type = RT_Unknown;
	ms->ms_OpcodeSize = ms->ms_ArgSize;

	// --

bailout:

	return;
}

// --
