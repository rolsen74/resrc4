 
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

void Cmd_MUL( struct M68kStruct *ms )
{
int opmode;

	opmode = ( ms->ms_Opcode & 0x01c00000 ) >> 22;

	switch( opmode )
	{
		// 0 : And.b

		// 1 : And.w

		// 2 : And.l

		case 3:
		{
			ms->ms_Str_Opcode = "Mulu.w";
			ms->ms_DecMode	= 2;
			break;
		}

		// 4 : And.b

		// 5 : And.w

		// 6 : And.l

		case 7:
		{
			ms->ms_Str_Opcode = "Muls.w";
			ms->ms_DecMode	= 1;
			break;
		}

		default:
		{
			printf( "Unsupported 'Mul' Opcode (Mode: %d)\n", opmode );
			ms->ms_DecodeStatus = MSTAT_Error;
			goto bailout;
		}
	}

	// --

	ms->ms_ArgType  = OS_Word;
	ms->ms_ArgEMode = ( ms->ms_Opcode & 0x00380000 ) >> 19;
	ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x00070000 ) >> 16;

	ms->ms_CurRegister = & ms->ms_SrcRegister;

	M68k_EffectiveAddress( ms );

	// --

	ms->ms_ArgEMode = 0x00; // Dx Reg
	ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x0e000000 ) >> 25;

	ms->ms_CurRegister = & ms->ms_DstRegister;

	M68k_EffectiveAddress( ms );

	ms->ms_CurRegister->mr_Type = RT_Unknown;
	ms->ms_OpcodeSize = ms->ms_ArgSize;

bailout:

	return;
}

// --

void Cmd_MULS_L( struct M68kStruct *ms )
{
uint32_t pos;

	ms->ms_Str_Opcode = "Muls.l";

	ms->ms_DecMode	= 1;
	ms->ms_ArgSize	= 4;
	ms->ms_ArgType	= OS_Long;
	ms->ms_ArgEMode	= ( ms->ms_Opcode & 0x00380000 ) >> 19;
	ms->ms_ArgEReg	= ( ms->ms_Opcode & 0x00070000 ) >> 16;

	ms->ms_CurRegister = & ms->ms_SrcRegister;

	M68k_EffectiveAddress( ms );

	if ( ms->ms_Opcode & 0x00000400 )
	{
		// Divs.l Dx,Dh:Di

		ms->ms_ArgEMode	= 0x00; // Dx Reg
		ms->ms_ArgEReg	= ms->ms_Opcode & 0x00000007;

		M68k_EffectiveAddress( ms );

		ms->ms_CurRegister->mr_Type = RT_Unknown;

		pos = strlen( ms->ms_Buf_Argument );

		ms->ms_ArgEMode	= 0x00; // Dx Reg
		ms->ms_ArgEReg	= ( ms->ms_Opcode & 0x00070000 ) >> 12;

		M68k_EffectiveAddress( ms );

		ms->ms_CurRegister->mr_Type = RT_Unknown;
		ms->ms_Buf_Argument[ pos ] = ':';
	}
	else
	{
		// Divs.l Dx,Di

		ms->ms_ArgEMode	= 0x00; // Dx Reg
		ms->ms_ArgEReg	= ( ms->ms_Opcode & 0x00007000 ) >> 12;

		M68k_EffectiveAddress( ms );

		ms->ms_CurRegister->mr_Type = RT_Unknown;
	}

	ms->ms_OpcodeSize = ms->ms_ArgSize;
}

// --

void Cmd_MULU_L( struct M68kStruct *ms )
{
uint32_t pos;

	ms->ms_Str_Opcode = "Mulu.l";

	ms->ms_DecMode	= 2;
	ms->ms_ArgSize	= 4;
	ms->ms_ArgType	= OS_Long;
	ms->ms_ArgEMode	= ( ms->ms_Opcode & 0x00380000 ) >> 19;
	ms->ms_ArgEReg	= ( ms->ms_Opcode & 0x00070000 ) >> 16;

	ms->ms_CurRegister = & ms->ms_SrcRegister;

	M68k_EffectiveAddress( ms );

	if ( ms->ms_Opcode & 0x00000400 )
	{
		// Divs.l Dx,Dh:Di

		ms->ms_ArgEMode	= 0x00; // Dx Reg
		ms->ms_ArgEReg	= ms->ms_Opcode & 0x00000007;

		M68k_EffectiveAddress( ms );

		pos = strlen( ms->ms_Buf_Argument );

		ms->ms_ArgEMode	= 0x00; // Dx Reg
		ms->ms_ArgEReg	= ( ms->ms_Opcode & 0x00070000 ) >> 12;

		M68k_EffectiveAddress( ms );

		ms->ms_Buf_Argument[ pos ] = ':';
	}
	else
	{
		// Divs.l Dx,Di

		ms->ms_ArgEMode	= 0x00; // Dx Reg
		ms->ms_ArgEReg	= ( ms->ms_Opcode & 0x00007000 ) >> 12;

		M68k_EffectiveAddress( ms );
	}

	ms->ms_OpcodeSize = ms->ms_ArgSize;
}

// --
