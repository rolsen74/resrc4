
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

enum RS4DecodeStat AOS3_Graphics_Func_InitBitMap( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Label *rl;

	ec = RS4ErrStat_Okay;
	ds = RS4DecodeStat_Okay;

//printf( "InitBitMap : $%08lx\n", rt->rt_CurMemAdr );

	#if 0
    InitBitMap( bm, depth, width, height )
                a0   d0     d1      d2

    void InitBitMap( struct BitMap *, BYTE, UWORD, UWORD );
	#endif

	// A0 = struct BitMap
	if ( rt->rt_CPU.M68k.mt_Registers[ M68KREGT_A0 ].mr_Type1 != RRT_Label )
	{
//printf( "InitBitMap : Exit 1\n" );
		goto bailout;
	}

	rl = rt->rt_CPU.M68k.mt_Registers[ M68KREGT_A0 ].mr_Label;

	if ( ! rl )
	{
//printf( "InitBitMap : Exit 2\n" );
		goto bailout;
	}

	// enum RS4FuncStat Mark_Struct( enum RS4ErrorCode *errcode, RS4Label *rl, enum RS4StructID id )

	fs = Mark_Struct( & ec, rl, RS4StructID_BitMap );

	if ( fs != RS4FuncStat_Okay )
	{
		ds = RS4DecodeStat_Error;
		// ec allready set

		#ifdef DEBUG
		printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
		#endif

//		goto bailout;		
	}

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( ds );
}

// --
