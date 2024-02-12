
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

void Cmd_DIV( struct M68kStruct *ms )
{
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

	M68k_EffectiveAddress( ms );

	ms->ms_ArgEMode = 0x00; // Dx Reg
	ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x0e000000 ) >> 25;

	ms->ms_CurRegister = & ms->ms_DstRegister;

	M68k_EffectiveAddress( ms );

	ms->ms_CurRegister->mr_Type = RT_Unknown;
	ms->ms_OpcodeSize = ms->ms_ArgSize;

	// --

bailout:

	return;
}

// --

void Cmd_DIVS_L( struct M68kStruct *ms )
{
uint32_t pos;
int EMode;
int EReg;
int Size;
int Dq;
int Dr;

	EMode	= ( ms->ms_Opcode & 0x00380000 ) >> 19;
	EReg	= ( ms->ms_Opcode & 0x00070000 ) >> 16;
	Dq		= ( ms->ms_Opcode & 0x00007000 ) >> 12;
	Size	= ( ms->ms_Opcode & 0x00000400 );
	Dr		= ( ms->ms_Opcode & 0x00000007 );

	// --

	ms->ms_DecMode	= 1;
	ms->ms_ArgSize	= 4;
	ms->ms_ArgType	= OS_Long;
	ms->ms_ArgEMode	= EMode;
	ms->ms_ArgEReg	= EReg;

	ms->ms_CurRegister = & ms->ms_SrcRegister;

	M68k_EffectiveAddress( ms );

	ms->ms_OpcodeSize = ms->ms_ArgSize;

	// --

	if ( Size )
	{
		ms->ms_Str_Opcode = "Divs.l";

		if ( Dr == Dq )
		{
			// Not seen

			ms->ms_ArgEMode	= 0x00; // Dx Reg
			ms->ms_ArgEReg	= Dq;

			M68k_EffectiveAddress( ms );
		}
		else
		{
			ms->ms_ArgEMode	= 0x00; // Dx Reg
			ms->ms_ArgEReg	= Dr;

			M68k_EffectiveAddress( ms );

			pos = strlen( ms->ms_Buf_Argument );

			ms->ms_ArgEMode	= 0x00; // Dx Reg
			ms->ms_ArgEReg	= Dq;

			M68k_EffectiveAddress( ms );

			ms->ms_Buf_Argument[ pos ] = ':';
		}
	}
	else
	{
		if ( Dr == Dq )
		{
			ms->ms_Str_Opcode = "Divs.l";

			ms->ms_ArgEMode	= 0x00; // Dx Reg
			ms->ms_ArgEReg	= Dq;

			M68k_EffectiveAddress( ms );
		}
		else
		{
			ms->ms_Str_Opcode = "Divsl.l";

			ms->ms_ArgEMode	= 0x00; // Dx Reg
			ms->ms_ArgEReg	= Dr;

			M68k_EffectiveAddress( ms );

			pos = strlen( ms->ms_Buf_Argument );

			ms->ms_ArgEMode	= 0x00; // Dx Reg
			ms->ms_ArgEReg	= Dq;

			M68k_EffectiveAddress( ms );

			ms->ms_Buf_Argument[ pos ] = ':';
		}
	}
}

// --

void Cmd_DIVU_L( struct M68kStruct *ms )
{
uint32_t pos;
int EMode;
int EReg;
int Size;
int Dq;
int Dr;

	EMode	= ( ms->ms_Opcode & 0x00380000 ) >> 19;
	EReg	= ( ms->ms_Opcode & 0x00070000 ) >> 16;
	Dq		= ( ms->ms_Opcode & 0x00007000 ) >> 12;
	Size	= ( ms->ms_Opcode & 0x00000400 );
	Dr		= ( ms->ms_Opcode & 0x00000007 );

	// --

	ms->ms_DecMode	= 2;
	ms->ms_ArgSize	= 4;
	ms->ms_ArgType	= OS_Long;
	ms->ms_ArgEMode	= EMode;
	ms->ms_ArgEReg	= EReg;

	ms->ms_CurRegister = & ms->ms_SrcRegister;

	M68k_EffectiveAddress( ms );

	ms->ms_OpcodeSize = ms->ms_ArgSize;

	// --

	if ( Size )
	{
		ms->ms_Str_Opcode = "Divu.l";

		if ( Dr == Dq )
		{
			// Not seen

			ms->ms_ArgEMode	= 0x00; // Dx Reg
			ms->ms_ArgEReg	= Dq;

			M68k_EffectiveAddress( ms );
		}
		else
		{
			ms->ms_ArgEMode	= 0x00; // Dx Reg
			ms->ms_ArgEReg	= Dr;

			M68k_EffectiveAddress( ms );

			pos = strlen( ms->ms_Buf_Argument );

			ms->ms_ArgEMode	= 0x00; // Dx Reg
			ms->ms_ArgEReg	= Dq;

			M68k_EffectiveAddress( ms );

			ms->ms_Buf_Argument[ pos ] = ':';
		}
	}
	else
	{
		if ( Dr == Dq )
		{
			ms->ms_Str_Opcode = "Divu.l";

			ms->ms_ArgEMode	= 0x00; // Dx Reg
			ms->ms_ArgEReg	= Dq;

			M68k_EffectiveAddress( ms );
		}
		else
		{
			ms->ms_Str_Opcode = "Divul.l";

			ms->ms_ArgEMode	= 0x00; // Dx Reg
			ms->ms_ArgEReg	= Dr;

			M68k_EffectiveAddress( ms );

			pos = strlen( ms->ms_Buf_Argument );

			ms->ms_ArgEMode	= 0x00; // Dx Reg
			ms->ms_ArgEReg	= Dq;

			M68k_EffectiveAddress( ms );

			ms->ms_Buf_Argument[ pos ] = ':';
		}
	}
}

// --
