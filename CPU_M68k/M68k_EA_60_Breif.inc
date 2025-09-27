
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
// -- Mode 60 - Brief Extension Word Format

inline static enum RS4DecodeStat MODE_60_Brief( enum RS4ErrorCode *errcode, RS4Trace *rt, STR outstr )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
MEM mem;
S8 Offset;
S32 SCALE;
S32 REG;
S32 pos;
S32 AD;
S32 WL;

	// --

	ec = RS4ErrStat_Okay;
	ds = RS4DecodeStat_Okay;

	// --

	// Brief Extension Word Format
	//
	// [A/D] [REG] [W/L] [Scale] [0] [ signed offset ]
	//   1     3     1      2     1          8
	// [  4 bit  ] [     4 bit     ] [     8 bit     ]

	// Lea $00(Ax,Dx.L),A0
	// lea OFFSET ( ArgReg,   AD[REG],   D0.  WL  *SCALE  )  ,  a1

	// ( d8 , An , Xn.SIZE * SCALE )

	mem = rt->rt_CurMemBuf;
	pos = rt->rt_CPU.M68k.mt_ArgSize;

	AD		= ( mem[ pos + 0 ] & 0x80 );
	REG		= ( mem[ pos + 0 ] & 0x70 ) >> 4;
	WL		= ( mem[ pos + 0 ] & 0x08 );
	SCALE	= ( mem[ pos + 0 ] & 0x06 ) >> 1;
	Offset	= ( mem[ pos + 1 ] );

	sprintf( outstr, "(%d,%s,%s%s%s)",
		Offset,
		Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ],
		( AD ) ? Ax_RegNames[REG] : Dx_RegNames[REG],
		( WL ) ? ".l" : ".w",
		scale_Names[SCALE]
	);

	rt->rt_CPU.M68k.mt_ArgSize += 2;

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( ds );
}

// --
