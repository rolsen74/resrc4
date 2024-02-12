
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

void Cmd_FMOVE( struct M68kStruct *ms )
{
int mode;
int src;
int dst;

// printf( "Cmd_FMOVE 1\n" );

	src  = ( ms->ms_Opcode & 0x00001c00 ) >> 10;
	dst  = ( ms->ms_Opcode & 0x00000380 ) >> 7;
	mode = ( ms->ms_Opcode & 0x0000007f );

	if ( mode != 0 )
	{
		printf( "Unsupported 'FMove1' Opcode (Mode $%03x) at %08x\n", mode, ms->ms_MemoryAdr );
		ms->ms_DecodeStatus = MSTAT_Error;
		goto bailout;
	}

	ms->ms_Str_Opcode = "FMove.x";
	ms->ms_OpcodeSize = 4;

	sprintf( ms->ms_Buf_Argument, "%s,%s", FPx_RegNames[src], FPx_RegNames[dst] );

bailout:

	return;
}

// --

void Cmd_FMOVE2( struct M68kStruct *ms )
{
int emode;
int ereg;
int mode;
int src;
int dst;
int len;

// printf( "Cmd_FMOVE 2\n" );

	emode= ( ms->ms_Opcode & 0x00380000 ) >> 19;
	ereg = ( ms->ms_Opcode & 0x00070000 ) >> 16;
	src  = ( ms->ms_Opcode & 0x00001c00 ) >> 10;
	dst  = ( ms->ms_Opcode & 0x00000380 ) >> 7;
	mode = ( ms->ms_Opcode & 0x0000007f );

	if ( mode != 0 )
	{
		printf( "Unsupported 'FMove2' Opcode (Mode $%03x) at %08x\n", mode, ms->ms_MemoryAdr );
		ms->ms_DecodeStatus = MSTAT_Error;
		goto bailout;
	}

	switch( src )
	{
		case 0:
		{
			ms->ms_ArgType = OS_Long;
			ms->ms_Str_Opcode = "FMove.l";
			break;
		}

		case 1:
		{
			ms->ms_ArgType = OS_Single;
			ms->ms_Str_Opcode = "FMove.s";
			break;
		}

		case 2:
		{
			ms->ms_ArgType = OS_Extended;
			ms->ms_Str_Opcode = "FMove.x";
			break;
		}

		case 3:
		{
			ms->ms_ArgType = OS_Packed;
			ms->ms_Str_Opcode = "FMove.p";
			break;
		}

		case 4:
		{
			ms->ms_ArgType = OS_Word;
			ms->ms_Str_Opcode = "FMove.w";
			break;
		}

		case 5:
		{
			ms->ms_ArgType = OS_Double;
			ms->ms_Str_Opcode = "FMove.d";
			break;
		}

		case 6:
		{
			ms->ms_ArgType = OS_Byte;
			ms->ms_Str_Opcode = "FMove.b";
			break;
		}

		default:
		{
			printf( "Unsupported 'FMove2' Opcode (Size %d) at %08x\n", src, ms->ms_MemoryAdr );
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

void Cmd_FMOVE3( struct M68kStruct *ms )
{
int emode;
int ereg;
int fmt;
int src;
//int len;
//int k;

// printf( "Cmd_FMOVE 3 : at %08x\n", ms->ms_MemoryAdr );

	emode= ( ms->ms_Opcode & 0x00380000 ) >> 19;
	ereg = ( ms->ms_Opcode & 0x00070000 ) >> 16;
	fmt  = ( ms->ms_Opcode & 0x00001c00 ) >> 10;
	src  = ( ms->ms_Opcode & 0x00000380 ) >> 7;
//	k	 = ( ms->ms_Opcode & 0x0000007f );

	switch( fmt )
	{
		case 0:
		{
			ms->ms_ArgType = OS_Long;
			ms->ms_Str_Opcode = "FMove.l";
			break;
		}

		case 1:
		{
			ms->ms_ArgType = OS_Single;
			ms->ms_Str_Opcode = "FMove.s";
			break;
		}

		case 2:
		{
			ms->ms_ArgType = OS_Extended;
			ms->ms_Str_Opcode = "FMove.x";
			break;
		}

		#if 0
		case 3:
		{
			ms->ms_ArgType = OS_Packed;
			ms->ms_Str_Opcode = "FMove.p";
			break;
		}
		#endif

		case 4:
		{
			ms->ms_ArgType = OS_Word;
			ms->ms_Str_Opcode = "FMove.w";
			break;
		}

		case 5:
		{
			ms->ms_ArgType = OS_Double;
			ms->ms_Str_Opcode = "FMove.d";
			break;
		}

		case 6:
		{
			ms->ms_ArgType = OS_Byte;
			ms->ms_Str_Opcode = "FMove.b";
			break;
		}

		#if 0
		case 7:
		{
			ms->ms_ArgType = OS_Packed;
			ms->ms_Str_Opcode = "FMove.p";
			break;
		}
		#endif

		default:
		{
			printf( "Unsupported 'FMove3' Opcode (Size %d) at %08x\n", fmt, ms->ms_MemoryAdr );
			ms->ms_DecodeStatus = MSTAT_Error;
			goto bailout;
		}
	}

	// --

//	len = strlen( ms->ms_Buf_Argument );
//
//	sprintf( & ms->ms_Buf_Argument[len], ",%s", FPx_RegNames[src] );

	sprintf( ms->ms_Buf_Argument, "%s", FPx_RegNames[src] );

	// --

	ms->ms_ArgEMode = emode;
	ms->ms_ArgEReg  = ereg;
	ms->ms_ArgSize	= 4;

	ms->ms_CurRegister = & ms->ms_SrcRegister;

	M68k_EffectiveAddress( ms );

	// --

	ms->ms_OpcodeSize = ms->ms_ArgSize;

bailout:

	return;
}

// --

void Cmd_FMOVE4( struct M68kStruct *ms )
{
char *name;
int emode;
int ereg;
int len;
int reg;
int dr;

	emode	= ( ms->ms_Opcode & 0x00380000 ) >> 19;
	ereg	= ( ms->ms_Opcode & 0x00070000 ) >> 16;
	dr		= ( ms->ms_Opcode & 0x00002000 );
	reg 	= ( ms->ms_Opcode & 0x00001c00 ) >> 10;

	switch( reg )
	{
		case 0x1:
		{
			// Floating-Point Instruction Address Regiser
			name = "fpiar";
			break;
		}

		case 0x2:
		{
			// Floating-Point Instruction Status Regiser
			name = "fpsr";
			break;
		}

		case 0x4:
		{
			// Floating-Point Instruction Control Regiser
			name = "fpcr";
			break;
		}

		default:
		{
			printf( "Unsupported 'FMove4' Reg Type (%d) at %08x\n", reg, ms->ms_MemoryAdr );
			ms->ms_DecodeStatus = MSTAT_Error;
			goto bailout;
		}
	}

	ms->ms_CurRegister = & ms->ms_SrcRegister;
	ms->ms_Str_Opcode = "FMove.l";
	ms->ms_ArgType	= OS_Long;
	ms->ms_ArgEMode = emode;
	ms->ms_ArgEReg  = ereg;
	ms->ms_ArgSize	= 4;

	if ( dr )
	{
		sprintf( ms->ms_Buf_Argument, "%s", name );

		M68k_EffectiveAddress( ms );
	}
	else
	{
		M68k_EffectiveAddress( ms );

		len = strlen( ms->ms_Buf_Argument );

		sprintf( & ms->ms_Buf_Argument[len], ",%s", name );
	}

	ms->ms_OpcodeSize = ms->ms_ArgSize;

bailout:

	return;
}

// --
