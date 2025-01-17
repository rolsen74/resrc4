
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

enum RS4DecodeStat M68kCmd_EXG( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
struct M68kRegister tmp;
int mode;
int yreg;
int xreg;

	xreg = ( rt->rt_CPU.M68k.mt_Opcode & 0x0e000000 ) >> 25;
	mode = ( rt->rt_CPU.M68k.mt_Opcode & 0x00f80000 ) >> 19;
	yreg = ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;

	switch( mode )
	{
		case 8:
		{
			tmp = rt->rt_CPU.M68k.mt_Registers[ M68KREGT_Dx + xreg ];
			/* */ rt->rt_CPU.M68k.mt_Registers[ M68KREGT_Dx + xreg ] = rt->rt_CPU.M68k.mt_Registers[ M68KREGT_Dx + yreg ];
			/* */ rt->rt_CPU.M68k.mt_Registers[ M68KREGT_Dx + yreg ] = tmp;

			// --

			rt->rt_Container.Hunk.ms_Str_Opcode = "Exg.l";
			sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "%s,%s", Dx_RegNames[xreg], Dx_RegNames[yreg] );
			break;
		}

		case 9:
		{
			tmp = rt->rt_CPU.M68k.mt_Registers[ M68KREGT_Ax + xreg ];
			/* */ rt->rt_CPU.M68k.mt_Registers[ M68KREGT_Ax + xreg ] = rt->rt_CPU.M68k.mt_Registers[ M68KREGT_Ax + yreg ];
			/* */ rt->rt_CPU.M68k.mt_Registers[ M68KREGT_Ax + yreg ] = tmp;

			// --

			rt->rt_Container.Hunk.ms_Str_Opcode = "Exg.l";
			sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "%s,%s", Ax_RegNames[xreg], Ax_RegNames[yreg] );
			break;
		}

		case 17:
		{
			tmp = rt->rt_CPU.M68k.mt_Registers[ M68KREGT_Dx + xreg ];
			/* */ rt->rt_CPU.M68k.mt_Registers[ M68KREGT_Dx + xreg ] = rt->rt_CPU.M68k.mt_Registers[ M68KREGT_Ax + yreg ];
			/* */ rt->rt_CPU.M68k.mt_Registers[ M68KREGT_Ax + yreg ] = tmp;

			// --

			rt->rt_Container.Hunk.ms_Str_Opcode = "Exg.l";
			sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "%s,%s", Dx_RegNames[xreg], Ax_RegNames[yreg] );
			break;
		}

		default:	
		{
			printf( "Unsupported 'Exg' Opcode (Mode: %d)\n", mode );
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
