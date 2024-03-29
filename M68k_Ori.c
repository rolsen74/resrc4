
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

void Cmd_ORI( struct M68kStruct *ms )
{
int size;

	size = ( ms->ms_Opcode & 0x00c00000 ) >> 22;

	switch( size )
	{
		case 0:
		{
			ms->ms_Str_Opcode = ( ShortOpcodes ) ? "Or.b" : "Ori.b" ;
			ms->ms_ArgType  = OS_Byte;
			break;
		}

		case 1:
		{
			ms->ms_Str_Opcode = ( ShortOpcodes ) ? "Or.w" : "Ori.w" ;
			ms->ms_ArgType  = OS_Word;
			break;
		}

		case 2:
		{
			ms->ms_Str_Opcode = ( ShortOpcodes ) ? "Or.l" : "Ori.l" ;
			ms->ms_ArgType  = OS_Long;
			break;
		}

		default:
		{
			printf( "Unsupported 'Ori' Opcode (Mode: %d)\n", size );
			ms->ms_DecodeStatus = MSTAT_Error;
			goto bailout;
		}
	}

	// --

	ms->ms_ArgEMode = 7;
	ms->ms_ArgEReg  = 4;

	ms->ms_CurRegister = & ms->ms_SrcRegister;

	M68k_EffectiveAddress( ms );

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

void Cmd_ORI2( struct M68kStruct *ms )
{
uint8_t val;

	val = ( ms->ms_Opcode & 0x000000ff );

	ms->ms_Str_Opcode = ( ShortOpcodes ) ? "Or.b" : "Ori.b" ;
	ms->ms_OpcodeSize = 4;
	ms->ms_ArgType = OS_Byte;

	sprintf( ms->ms_Buf_Argument, "#$%02x,CCR", val );
}

// --

void Cmd_ORI3( struct M68kStruct *ms )
{
uint16_t val;

	val = ( ms->ms_Opcode & 0x0000ffff );

	ms->ms_Str_Opcode = ( ShortOpcodes ) ? "Or.w" : "Ori.w" ;
	ms->ms_OpcodeSize = 4;
	ms->ms_ArgType = OS_Word;

	sprintf( ms->ms_Buf_Argument, "#$%04x,SR", val );
}

// --
