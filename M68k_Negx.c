
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

void Cmd_NEGX( struct M68kStruct *ms )
{
int emode;
int ereg;
int size;

	size	= ( ms->ms_Opcode & 0x00c00000 ) >> 22;
	emode	= ( ms->ms_Opcode & 0x00380000 ) >> 19;
	ereg	= ( ms->ms_Opcode & 0x00070000 ) >> 16;

	switch( size )
	{
		case 0:
		{
			ms->ms_Str_Opcode = "Negx.b";
			ms->ms_ArgType  = OS_Byte;
			break;
		}

		case 1:
		{
			ms->ms_Str_Opcode = "Negx.w";
			ms->ms_ArgType  = OS_Word;
			break;
		}

		case 2:
		{
			ms->ms_Str_Opcode = "Negx.l";
			ms->ms_ArgType  = OS_Long;
			break;
		}

		default:
		{
			printf( "Unsupported 'Negx' Opcode (Mode: %d)\n", size );
			ms->ms_DecodeStatus = MSTAT_Error;
			goto bailout;
		}
	}

	ms->ms_ArgEMode = emode;
	ms->ms_ArgEReg  = ereg;

	ms->ms_CurRegister = & ms->ms_DstRegister;

	M68k_EffectiveAddress( ms );

	ms->ms_CurRegister->mr_Type = RT_Unknown;
	ms->ms_OpcodeSize = ms->ms_ArgSize;

bailout:

	return;
}

// --
