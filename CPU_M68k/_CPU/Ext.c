
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

enum RS4DecodeStat M68kCmd_EXT( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
U32 opmode;
U32 reg;

	opmode	= ( rt->rt_CPU.M68k.mt_Opcode & 0x01c00000 ) >> 22;
	reg		= ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;

	switch( opmode )
	{
		case 2:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Ext.w";
			break;
		}

		case 3:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Ext.l";
			break;
		}

		case 7:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Extb.l";
			break;
		}

		default:
		{
			printf( "Unsupported 'Ext' Opcode (Mode: %d)\n", opmode );
			ds = RS4DecodeStat_Error;
			goto bailout;
		}
	}

	sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "%s", Dx_RegNames[ reg ] );

	rt->rt_CPU.M68k.mt_ClearRegMask |= 1U << ( M68KREGT_D0 + reg ); 
	rt->rt_CPU.M68k.mt_OpcodeSize = 2;

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
