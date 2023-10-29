
/*
 * Copyright (c) 2014-2023 Rene W. Olsen < renewolsen @ gmail . com >
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

// $f227e07c		-- 2.0.1	fmovem.x	FP2-FP6,-(a7)
// $f228f0ff,$003c	-- 2.2.1	fmovem.x	fp0-fp7,$003c(a7)
// $f21fd030		-- 2.2.0	fmovem.x	(a7)+,FP2-FP3

// --

static void RegMask( struct M68kStruct *ms, int reverse )
{
char *buf;
uint32_t mask;
uint32_t pos;
uint32_t bit;
int start;
int end;
int cnt;

	mask = ms->ms_Opcode & 0x00ff;

	start = end = -1;

	pos = strlen( ms->ms_Buf_Argument );

	if ( pos )
	{
		sprintf( & ms->ms_Buf_Argument[ pos ], "," );
		pos++;
	}

	buf = & ms->ms_Buf_Argument[pos];

	bit = ( ! reverse ) ? 0x01 : 0x80;

	for( cnt=0 ; cnt<9 ; cnt++ )
	{
		if (( mask & bit ) && ( cnt != 8 ))
		{
			if ( start == -1 )
			{
				start = end = cnt;
			}
			else
			{
				end = cnt;
			}
		}
		else
		{
			if ( start != -1 )	
			{
				pos = strlen( buf );

				if ( pos )
				{
					buf[pos++] = '/';
				}

				if ( start == end )
				{
					sprintf( &buf[pos], "%s", FPx_RegNames[start] );
				}
				else
				{
					sprintf( &buf[pos], "%s-%s", FPx_RegNames[start], FPx_RegNames[end] );
				}

				start = -1;
			}
		}

		if ( cnt != 8 )
		{
			bit = ( ! reverse ) ? bit * 2 : bit / 2;
		}
	}
}

// --

#if 0
void Cmd_FMOVEM_1_0( struct M68kStruct *ms, int emode, int ereg, int dr )
{
}

// --

void Cmd_FMOVEM_1_1( struct M68kStruct *ms, int emode, int ereg, int dr )
{
}

// --

void Cmd_FMOVEM_1_2( struct M68kStruct *ms, int emode, int ereg, int dr )
{
}

// --

void Cmd_FMOVEM_1_3( struct M68kStruct *ms, int emode, int ereg, int dr )
{
}
#endif

// --

void Cmd_FMOVEM_2_0( struct M68kStruct *ms, int emode, int ereg, int dr )
{
struct HunkRef *isRef;

	if ( dr )
	{
		// FMovem.x FP2-FP3,<EA>
		// $f227e07c		-- 2.0.1	fmovem.x	FP2-FP6,-(a7)

// printf( "Unsupported 'FMovem 2 0' Opcode %d at %08x\n", dr, ms->ms_MemoryAdr );

		RegMask( ms, false );

		ms->ms_ArgEMode = emode;
		ms->ms_ArgEReg  = ereg;
		ms->ms_ArgSize	= 4;

		ms->ms_CurRegister = & ms->ms_SrcRegister;

		isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

		if ( M68k_EffectiveAddress( ms, isRef, 0 ))
		{
			isRef->hr_Used = true;
		}

		ms->ms_OpcodeSize = ms->ms_ArgSize;
	}
	else
	{
printf( "Unsupported 'FMovem 2 0' Opcode %d at %08x\n", dr, ms->ms_MemoryAdr );

		ms->ms_DecodeStatus = MSTAT_Error;
	}
}

// --

#if 0
void Cmd_FMOVEM_2_1( struct M68kStruct *ms, int emode, int ereg, int dr )
{
}
#endif

// --

void Cmd_FMOVEM_2_2( struct M68kStruct *ms, int emode, int ereg, int dr )
{
struct HunkRef *isRef;

	if ( dr )
	{
		// $f228f0ff,$003c -- 2.2.1	fmovem.x	fp0-fp7,$003c(a7)

// printf( "Unsupported 'FMovem 2 2' Opcode %d at %08x\n", dr, ms->ms_MemoryAdr );

		RegMask( ms, true );

		ms->ms_ArgEMode = emode;
		ms->ms_ArgEReg  = ereg;
		ms->ms_ArgSize	= 4;

		ms->ms_CurRegister = & ms->ms_SrcRegister;

		isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

		if ( M68k_EffectiveAddress( ms, isRef, 0 ))
		{
			isRef->hr_Used = true;
		}

		ms->ms_OpcodeSize = ms->ms_ArgSize;
	}
	else
	{
// printf( "Unsupported 'FMovem 2 2' Opcode %d at %08x\n", dr, ms->ms_MemoryAdr );

		// FMovem.x <EA>,FP2-FP3
		// $f21fd030 -- fmovem.x	(a7)+,FP2-FP3

		ms->ms_ArgEMode = emode;
		ms->ms_ArgEReg  = ereg;
		ms->ms_ArgSize	= 4;

		ms->ms_CurRegister = & ms->ms_SrcRegister;

		isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

		if ( M68k_EffectiveAddress( ms, isRef, 0 ))
		{
			isRef->hr_Used = true;
		}

		RegMask( ms, true );

		ms->ms_OpcodeSize = ms->ms_ArgSize;
	}
}

// --

#if 0
void Cmd_FMOVEM_2_3( struct M68kStruct *ms, int emode, int ereg, int dr )
{
}
#endif

// --

void Cmd_FMOVEM( struct M68kStruct *ms )
{
int emode;
int ereg;
int mode;
int dr;

	ms->ms_Str_Opcode = "FMovem.x";
	ms->ms_ArgType = OS_Extended;

	emode= ( ms->ms_Opcode & 0x00380000 ) >> 19;
	ereg = ( ms->ms_Opcode & 0x00070000 ) >> 16;
	dr	 = ( ms->ms_Opcode & 0x00002000 ) ? true : false ;
	mode = ( ms->ms_Opcode & 0x00001800 ) >> 11;

	switch( mode )
	{
		#if 0
		case 0:
		{
			Cmd_FMOVEM_1_0( ms, emode, ereg, dr );
			break;
		}

		case 1:
		{
			Cmd_FMOVEM_1_1( ms, emode, ereg, dr );
			break;
		}

		case 2:
		{
			Cmd_FMOVEM_1_2( ms, emode, ereg, dr );
			break;
		}

		case 3:
		{
			Cmd_FMOVEM_1_3( ms, emode, ereg, dr );
			break;
		}
		#endif

		default:
		{
			printf( "Unsupported 'FMovem' Opcode (Mode %d, emode %d, ereg %d, dr %d) at %08x\n", mode, emode, ereg, dr, ms->ms_MemoryAdr );
			ms->ms_DecodeStatus = MSTAT_Error;
			break;
		}
	}
}

// --

void Cmd_FMOVEM2( struct M68kStruct *ms )
{
int emode;
int ereg;
int mode;
int dr;

	ms->ms_Str_Opcode = "FMovem.x";
	ms->ms_ArgType = OS_Extended;

	emode= ( ms->ms_Opcode & 0x00380000 ) >> 19;
	ereg = ( ms->ms_Opcode & 0x00070000 ) >> 16;
	dr	 = ( ms->ms_Opcode & 0x00002000 ) ? true : false ;
	mode = ( ms->ms_Opcode & 0x00001800 ) >> 11;

	switch( mode )
	{
		case 0:
		{
			Cmd_FMOVEM_2_0( ms, emode, ereg, dr );
			break;
		}

		#if 0
		case 1:
		{
			Cmd_FMOVEM_2_1( ms, emode, ereg, dr );
			break;
		}
		#endif

		case 2:
		{
			Cmd_FMOVEM_2_2( ms, emode, ereg, dr );
			break;
		}

		#if 0
		case 3:
		{
			Cmd_FMOVEM_2_3( ms, emode, ereg, dr );
			break;
		}
		#endif

		default:
		{
			printf( "Unsupported 'FMovem2' Opcode (Mode %d, emode %d, ereg %d, dr %d) at %08x\n", mode, emode, ereg, dr, ms->ms_MemoryAdr );
			ms->ms_DecodeStatus = MSTAT_Error;
			break;
		}

	}
}

// --
