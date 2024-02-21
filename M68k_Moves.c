
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

void Cmd_MOVES( struct M68kStruct *ms )
{
const char *rname;
int size;
int reg;
int pos;
int ad;
int dr;

	size	= ( ms->ms_Opcode & 0x00c00000 ) >> 22;
	ad		= ( ms->ms_Opcode & 0x00008000 );
	reg		= ( ms->ms_Opcode & 0x00007000 ) >> 12;
	dr		= ( ms->ms_Opcode & 0x00000800 );

	switch( size )
	{
		case 0:
		{
			ms->ms_Str_Opcode = "Moves.b";
			ms->ms_ArgType = OS_Byte;
			break;
		}

		case 1:
		{
			ms->ms_Str_Opcode = "Moves.w";
			ms->ms_ArgType = OS_Word;
			break;
		}

		case 2:
		{
			ms->ms_Str_Opcode = "Moves.l";
			ms->ms_ArgType = OS_Long;
			break;
		}

		default:
		{
			printf( "Unsupported 'Moves' Opcode (Mode: %d)\n", size );
			ms->ms_DecodeStatus = MSTAT_Error;
			goto bailout;
		}
	}

	rname = ( ad ) ? Ax_RegNames[reg] : Dx_RegNames[reg] ;

	ms->ms_ArgEMode	= ( ms->ms_Opcode & 0x00380000 ) >> 19;
	ms->ms_ArgEReg	= ( ms->ms_Opcode & 0x00070000 ) >> 16;

	ms->ms_CurRegister = & ms->ms_DstRegister;
	ms->ms_ArgSize = 4;

	if ( dr )
	{
		sprintf( ms->ms_Buf_Argument, "%s", rname );

		M68k_EffectiveAddress( ms );
	}
	else
	{
		M68k_EffectiveAddress( ms );

		pos = strlen( ms->ms_Buf_Argument );

		sprintf( & ms->ms_Buf_Argument[pos], ",%s", rname );
	}

	ms->ms_OpcodeSize = ms->ms_ArgSize;

bailout:

	return;
}
