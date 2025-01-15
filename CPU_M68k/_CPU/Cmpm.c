
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

#include "Resourcer/ReSrc4.h"

// --

enum RS4DecodeStat M68kCmd_CMPM( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
int size;
int yreg;
int xreg;

	xreg = ( rt->rt_CPU.M68k.mt_Opcode & 0x0e000000 ) >> 25;
	size = ( rt->rt_CPU.M68k.mt_Opcode & 0x00c00000 ) >> 22;
	yreg = ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;

	switch( size )
	{
		case 0:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Cmpm.b";
			sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "(%s)+,(%s)+", Ax_RegNames[yreg], Ax_RegNames[xreg] );
			break;
		}

		case 1:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Cmpm.w";
			sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "(%s)+,(%s)+", Ax_RegNames[yreg], Ax_RegNames[xreg] );
			break;
		}

		case 2:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Cmpm.l";
			sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "(%s)+,(%s)+", Ax_RegNames[yreg], Ax_RegNames[xreg] );
			break;
		}

		default:	
		{
			printf( "Unsupported 'Cmpm' Opcode (Mode: %d)\n", size );
			ds = RS4DecodeStat_Error;
			goto bailout;
		}
	}

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
