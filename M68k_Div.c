
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

void Cmd_DIV( struct M68kStruct *ms )
{
struct HunkRef *isRef;
uint32_t opmode;

	opmode = ( ms->ms_Opcode & 0x01c00000 ) >> 22;

	switch( opmode )
	{
		// 0 : Or.b

		// 1 : Or.w

		// 2 : Or.l

		case 3:
		{
			ms->ms_Str_Opcode = "Divu.w";
			ms->ms_ArgType = OS_Word;
			ms->ms_DecMode = 2;
			break;
		}

		// 4 : Or.b

		// 5 : Or.w

		// 6 : Or.l

		case 7:
		{
			ms->ms_Str_Opcode = "Divs.w";
			ms->ms_ArgType = OS_Word;
			ms->ms_DecMode = 1;
			break;
		}

		default:
		{
			printf( "Unsupported 'Div' Opcode (Mode: %d)\n", opmode );
			ms->ms_DecodeStatus = MSTAT_Error;
			goto bailout;
		}
	}

	// --

	ms->ms_ArgEMode = ( ms->ms_Opcode & 0x00380000 ) >> 19;
	ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x00070000 ) >> 16;

	ms->ms_CurRegister = & ms->ms_SrcRegister;

	isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

	if ( M68k_EffectiveAddress( ms, isRef, 0 ))
	{
		isRef->hr_Used = true;
	}

	ms->ms_ArgEMode = 0x00; // Dx Reg
	ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x0e000000 ) >> 25;

	ms->ms_CurRegister = & ms->ms_DstRegister;

	M68k_EffectiveAddress( ms, NULL, 0 );

	ms->ms_CurRegister->mr_Type = RT_Unknown;
	ms->ms_OpcodeSize = ms->ms_ArgSize;

	// --

bailout:

	return;
}

// --

void Cmd_DIVS_L( struct M68kStruct *ms )
{
struct HunkRef *isRef;
uint32_t pos;

	ms->ms_Str_Opcode = "Divs.l";

	ms->ms_DecMode	= 1;
	ms->ms_ArgSize	= 4;
	ms->ms_ArgType	= OS_Long;
	ms->ms_ArgEMode	= ( ms->ms_Opcode & 0x00380000 ) >> 19;
	ms->ms_ArgEReg	= ( ms->ms_Opcode & 0x00070000 ) >> 16;

	ms->ms_CurRegister = & ms->ms_DstRegister;

	isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

	if ( M68k_EffectiveAddress( ms, isRef, 0 ))
	{
		isRef->hr_Used = true;
	}

	if ( ms->ms_Opcode & 0x00000400 )
	{
		// Divsl.l Dx,Dx:Dq

		ms->ms_ArgEMode	= 0x00; // Dx Reg
		ms->ms_ArgEReg	= ms->ms_Opcode & 0x00000007;

		M68k_EffectiveAddress( ms, NULL, 0 );

		ms->ms_CurRegister->mr_Type = RT_Unknown;
		pos = strlen( ms->ms_Buf_Argument );

		ms->ms_ArgEMode	= 0x00; // Dx Reg
		ms->ms_ArgEReg	= ( ms->ms_Opcode & 0x00070000 ) >> 12;

		M68k_EffectiveAddress( ms, NULL, 0 );

		ms->ms_CurRegister->mr_Type = RT_Unknown;
		ms->ms_Buf_Argument[ pos ] = ':';
	}
	else
	{
		// Divsl.l Dx,Dq

		ms->ms_ArgEMode	= 0x00; // Dx Reg
		ms->ms_ArgEReg	= ( ms->ms_Opcode & 0x00007000 ) >> 12;

		M68k_EffectiveAddress( ms, NULL, 0 );

		ms->ms_CurRegister->mr_Type = RT_Unknown;
	}

	ms->ms_OpcodeSize = ms->ms_ArgSize;
}

// --

void Cmd_DIVU_L( struct M68kStruct *ms )
{
struct HunkRef *isRef;
uint32_t pos;

	ms->ms_Str_Opcode = "Divu.l";

	ms->ms_DecMode	= 2;
	ms->ms_ArgSize	= 4;
	ms->ms_ArgType	= OS_Long;
	ms->ms_ArgEMode	= ( ms->ms_Opcode & 0x00380000 ) >> 19;
	ms->ms_ArgEReg	= ( ms->ms_Opcode & 0x00070000 ) >> 16;

	ms->ms_CurRegister = & ms->ms_DstRegister;

	isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

	if ( M68k_EffectiveAddress( ms, isRef, 0 ))
	{
		isRef->hr_Used = true;
	}

	if ( ms->ms_Opcode & 0x00000400 )
	{
		// Divs.l Dx,Dr:Dq

		ms->ms_ArgEMode	= 0x00; // Dx Reg
		ms->ms_ArgEReg	= ms->ms_Opcode & 0x00000007;

		M68k_EffectiveAddress( ms, NULL, 0 );

		ms->ms_CurRegister->mr_Type = RT_Unknown;

		pos = strlen( ms->ms_Buf_Argument );

		ms->ms_ArgEMode	= 0x00; // Dx Reg
		ms->ms_ArgEReg	= ( ms->ms_Opcode & 0x00070000 ) >> 12;

		M68k_EffectiveAddress( ms, NULL, 0 );

		ms->ms_CurRegister->mr_Type = RT_Unknown;
		ms->ms_Buf_Argument[ pos ] = ':';
	}
	else
	{
		// Divs.l Dx,Dq

		ms->ms_ArgEMode	= 0x00;
		ms->ms_ArgEReg	= ( ms->ms_Opcode & 0x00007000 ) >> 12;

		M68k_EffectiveAddress( ms, NULL, 0 );

		ms->ms_CurRegister->mr_Type = RT_Unknown;
	}

	ms->ms_OpcodeSize = ms->ms_ArgSize;
}

// --
