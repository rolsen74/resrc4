
/*
 * Copyright (c) 2014-2025 Rene W. Olsen < renewolsen @ gmail . com >
 *
 * This software is released under the GNU General Public License, version 3.
 * For the full text of the license, please visit:
 * https://www.gnu.org/licenses/gpl-3.0.html
 *
 * You can also find a copy of the license in the LICENSE file included with this software.
 */

// --

#include "Resourcer/ReSrc4.h"

// --

enum RS4DecodeStat M68kCmd_MOVE16( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
U32 val;
MEM mem;
CHR labname[ MAX_LabelName + 8 ];
S32 mode;
S32 ay;

	ec		= RS4ErrStat_Error;
	ds		= RS4DecodeStat_Error;
	mem		= rt->rt_CurMemBuf;
	mode	= ( rt->rt_CPU.M68k.mt_Opcode & 0x00180000 ) >> 19;
	ay		= ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;
	val		= (( mem[2] << 24 ) | ( mem[3] << 16 ) | ( mem[4] << 8 ) | ( mem[5] << 0 ));

	fs = RS4BuildLabelString2( & ec, rt, labname, rt->rt_CurMemAdr + rt->rt_CPU.M68k.mt_ArgSize, val );

	if ( fs != RS4FuncStat_Okay )
	{
		// ec allready set
		ds = RS4DecodeStat_Error;

		#ifdef DEBUG
		printf( "%s:%04d: Error '%s'\n", __FILE__, __LINE__, labname );
		#endif

		goto bailout;
	}

	switch( mode )
	{
		case 0:
		{
			snprintf( rt->rt_Container.Hunk.ms_Buf_Argument, 64, "(%s)+,(%s).l", Ax_RegNames[ay], labname );
			break;
		}

		case 1:
		{
			snprintf( rt->rt_Container.Hunk.ms_Buf_Argument, 64, "(%s).l,(%s)+", labname, Ax_RegNames[ay] );
			break;
		}

		case 2:
		{
			snprintf( rt->rt_Container.Hunk.ms_Buf_Argument, 64, "(%s),(%s).l", Ax_RegNames[ay], labname );
			break;
		}

		case 3:
		{
			snprintf( rt->rt_Container.Hunk.ms_Buf_Argument, 64, "(%s),(%s).l", labname, Ax_RegNames[ay] );
			break;
		}

		default:
		{
			printf( "Unsupported 'Move16' Opcode (Mode: %d)\n", mode );
			ds = RS4DecodeStat_Error;
			goto bailout;
		}
	}

	rt->rt_Container.Hunk.ms_Str_Opcode = "Move16";
	rt->rt_CPU.M68k.mt_OpcodeSize = 6;

	// --

	ds = RS4DecodeStat_Okay;
	ec = RS4ErrStat_Okay;

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	return( ds );
}

// --

enum RS4DecodeStat M68kCmd_MOVE162( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
S32 ax;
S32 ay;

	ax = ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;
	ay = ( rt->rt_CPU.M68k.mt_Opcode & 0x00007000 ) >> 12;

	rt->rt_Container.Hunk.ms_Str_Opcode = "Move16";
	rt->rt_CPU.M68k.mt_OpcodeSize = 4;

	sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "(%s)+,(%s)+", Ax_RegNames[ax], Ax_RegNames[ay] );

	// --

	ds = RS4DecodeStat_Okay;
	ec = RS4ErrStat_Okay;

	// --

//bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( ds );
}

// --