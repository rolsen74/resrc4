
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

void Cmd_SUBX( struct M68kStruct *ms )
{
int size;
int rm;
int rx;
int ry;

	ry		= ( ms->ms_Opcode & 0x0e000000 ) >> 25;
	size	= ( ms->ms_Opcode & 0x00c00000 ) >> 22;
	rm		= ( ms->ms_Opcode & 0x00080000 );
	rx		= ( ms->ms_Opcode & 0x00070000 ) >> 16;

	switch( size )
	{
		case 0: // Subx.b
		{
			ms->ms_Str_Opcode = "Subx.b";
			ms->ms_ArgType  = OS_Byte;
			break;
		}

		case 1: // Subx.w
		{
			ms->ms_Str_Opcode = "Subx.w";
			ms->ms_ArgType  = OS_Word;
			break;
		}

		case 2: // Subx.l
		{
			ms->ms_Str_Opcode = "Subx.l";
			ms->ms_ArgType  = OS_Long;
			break;
		}

		default:
		{
			printf( "Unsupported 'Subx' Opcode (Size: %d)\n", size );
			ms->ms_DecodeStatus = MSTAT_Error;
			goto bailout;
		}
	}

	if ( rm )
	{
		sprintf( ms->ms_Buf_Argument, "-(%s),-(%s)", Ax_RegNames[rx], Ax_RegNames[ry] );
	}
	else
	{
		sprintf( ms->ms_Buf_Argument, "%s,%s", Dx_RegNames[rx], Dx_RegNames[ry] );
	}

	// --

bailout:

	return;
}

// --
