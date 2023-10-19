 
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

static inline void Args6( struct M68kStruct *ms )
{
int size;
int reg;
int ir;

	size	= ( ms->ms_Opcode & 0x0e000000 ) >> 25;
	reg		= ( ms->ms_Opcode & 0x00070000 ) >> 16;
	ir		= ( ms->ms_Opcode & 0x00200000 );

	if (( size == 0 ) && ( ir == 0 ))
	{
		size = 8;
	}

	if ( ir )
	{
		sprintf( ms->ms_Buf_Argument, "%s,%s", Dx_RegNames[size], Dx_RegNames[reg] );
	}
	else
	{
		sprintf( ms->ms_Buf_Argument, "#%d,%s", size, Dx_RegNames[reg] );
	}

	ms->ms_Registers[reg].mr_Type = RT_Unknown;
}

// --

void Cmd_ROXL( struct M68kStruct *ms )
{
int size;

	size = ( ms->ms_Opcode & 0x00c00000 ) >> 22;

	switch( size )
	{
		case 0:
		{
			ms->ms_Str_Opcode = "Roxl.b";
			break;
		}

		case 1:
		{
			ms->ms_Str_Opcode = "Roxl.w";
			break;
		}

		case 2:
    	{
			ms->ms_Str_Opcode = "Roxl.l";
			break;
		}

		default:
		{
			printf( "Unsupported 'Roxl' Opcode (Mode: %d)\n", size );
			ms->ms_DecodeStatus = MSTAT_Error;
			goto bailout;
		}
	}

	Args6( ms );

bailout:

	return;
}

// --

void Cmd_ROXL2( struct M68kStruct *ms )
{
struct HunkRef *isRef;

	ms->ms_Str_Opcode = "Roxl.w" ;

	ms->ms_ArgType  = OS_Word;
	ms->ms_ArgEMode = ( ms->ms_Opcode & 0x00380000 ) >> 19;
	ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x00070000 ) >> 16;

	ms->ms_CurRegister = & ms->ms_SrcRegister;

	isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

	if ( M68k_EffectiveAddress( ms, isRef, 0 ))
	{
		isRef->hr_Used = true;
	}

	ms->ms_CurRegister->mr_Type = RT_Unknown;
	ms->ms_OpcodeSize = ms->ms_ArgSize;
}

// --
