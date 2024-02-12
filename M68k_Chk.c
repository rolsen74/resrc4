
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

void Cmd_CHK( struct M68kStruct *ms )
{
int size;
int reg;
int pos;

	reg  = ( ms->ms_Opcode & 0x0e000000 ) >> 25;
	size = ( ms->ms_Opcode & 0x01800000 ) >> 23;

	switch( size )
	{
		case 2:
		{
			ms->ms_Str_Opcode = "Chk.l";
			ms->ms_ArgType  = OS_Long;
			break;
		}

		case 3:
		{
			ms->ms_Str_Opcode = "Chk.w";
			ms->ms_ArgType  = OS_Word;
			break;
		}

		default:
		{
			printf( "Unsupported 'Chk' Opcode (Size: %d) at %08x\n", size, ms->ms_MemoryAdr );
			ms->ms_DecodeStatus = MSTAT_Error;
			goto bailout;
		}
	}

	// --

	ms->ms_ArgEMode = ( ms->ms_Opcode & 0x00380000 ) >> 19;
	ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x00070000 ) >> 16;

	ms->ms_CurRegister = & ms->ms_DstRegister;

	M68k_EffectiveAddress( ms );

	// --

	pos = strlen( ms->ms_Buf_Argument );
	sprintf( & ms->ms_Buf_Argument[pos], ",%s", Dx_RegNames[reg] );

	// --

	ms->ms_OpcodeSize = ms->ms_ArgSize;

bailout:

	return;
}

// --
