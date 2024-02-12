
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

void Cmd_FSNEG( struct M68kStruct *ms )
{
int src;
int dst;

	src  = ( ms->ms_Opcode & 0x00001c00 ) >> 10;
	dst  = ( ms->ms_Opcode & 0x00000380 ) >> 7;

	ms->ms_Str_Opcode = "FSNeg.x";
	ms->ms_OpcodeSize = 4;

	if ( src == dst )
	{
		sprintf( ms->ms_Buf_Argument, "%s", FPx_RegNames[src] );
	}
	else
	{
		sprintf( ms->ms_Buf_Argument, "%s,%s", FPx_RegNames[src], FPx_RegNames[dst] );
	}
}

// --

void Cmd_FSNEG2( struct M68kStruct *ms )
{
int emode;
int ereg;
int src;
int dst;
int len;

	emode= ( ms->ms_Opcode & 0x00380000 ) >> 19;
	ereg = ( ms->ms_Opcode & 0x00070000 ) >> 16;
	src  = ( ms->ms_Opcode & 0x00001c00 ) >> 10;
	dst  = ( ms->ms_Opcode & 0x00000380 ) >> 7;

	switch( src )
	{
		case 0:
		{
			ms->ms_ArgType = OS_Long;
			ms->ms_Str_Opcode = "FSNeg.l";
			break;
		}

		case 1:
		{
			ms->ms_ArgType = OS_Single;
			ms->ms_Str_Opcode = "FSNeg.s";
			break;
		}

		case 2:
		{
			ms->ms_ArgType = OS_Extended;
			ms->ms_Str_Opcode = "FSNeg.x";
			break;
		}

		case 3:
		{
			ms->ms_ArgType = OS_Packed;
			ms->ms_Str_Opcode = "FSNeg.p";
			break;
		}

		case 4:
		{
			ms->ms_ArgType = OS_Word;
			ms->ms_Str_Opcode = "FSNeg.w";
			break;
		}

		case 5:
		{
			ms->ms_ArgType = OS_Double;
			ms->ms_Str_Opcode = "FSNeg.d";
			break;
		}

		case 6:
		{
			ms->ms_ArgType = OS_Byte;
			ms->ms_Str_Opcode = "FSNeg.b";
			break;
		}

		default:
		{
			printf( "Unsupported 'FSNeg2' Opcode (Size %d) at %08x\n", src, ms->ms_MemoryAdr );
			ms->ms_DecodeStatus = MSTAT_Error;
			goto bailout;
		}
	}

	// --

	ms->ms_ArgEMode = emode;
	ms->ms_ArgEReg  = ereg;
	ms->ms_ArgSize	= 4;

	ms->ms_CurRegister = & ms->ms_SrcRegister;

	M68k_EffectiveAddress( ms );

	// --

	len = strlen( ms->ms_Buf_Argument );

	sprintf( & ms->ms_Buf_Argument[len], ",%s", FPx_RegNames[dst] );

	// --

	ms->ms_OpcodeSize = ms->ms_ArgSize;

bailout:

	return;
}

// --
