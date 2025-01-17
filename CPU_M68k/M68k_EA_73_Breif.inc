
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
// -- Mode 73 - Brief Extension Word Format

inline static enum RS4DecodeStat MODE_73_Brief( enum RS4ErrorCode *errcode, RS4Trace *rt, char *outstr )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Label *rl;
uint32_t adr;
uint8_t *mem;
int8_t Offset;
char labname[ MAX_LabelName + 8 ];
int SCALE;
int REG;
int pos;
int AD;
int WL;

	// --

	ec = RS4ErrStat_Okay;
	ds = RS4DecodeStat_Okay;

	// --

	// Brief Extension Word Format
	//
	// [A/D] [REG] [W/L] [Scale] [0] [ signed offset ]
	//   1     3     1      2     1          8
	// [  4 bit  ] [     4 bit     ] [     8 bit     ]

	// ( d8 , PC , Xn.SIZE * SCALE )

	mem = rt->rt_CurMemBuf;
	pos = rt->rt_CPU.M68k.mt_ArgSize;

	AD		= ( mem[ pos + 0 ] & 0x80 );
	REG		= ( mem[ pos + 0 ] & 0x70 ) >> 4;
	WL		= ( mem[ pos + 0 ] & 0x08 );
	SCALE	= ( mem[ pos + 0 ] & 0x06 ) >> 1;
	Offset	= ( mem[ pos + 1 ] );

	// --

	adr = rt->rt_CurMemAdr + 2 + Offset;

	rl = RS4AddLabel_Sec( & ec, rt->rt_Section, adr, RS4LabelType_Unset );

	if (( rl ) && ( rl->rl_Name[0] ))
	{
		fs = RS4BuildLabelString( & ec, rl, labname );

		if ( fs != RS4FuncStat_Okay )
		{
			// ec allready set
			ds = RS4DecodeStat_Error;

			#ifdef DEBUG
			printf( "%s:%04d: Error '%s'\n", __FILE__, __LINE__, labname );
			#endif

			goto bailout;
		}

		sprintf( outstr, "(%s,PC,%s%s%s)",
			labname,
			( AD ) ? Ax_RegNames[REG] : Dx_RegNames[REG],
			( WL ) ? ".l" : ".w",
			scale_Names[SCALE]
		);
	}
	else
	{
		sprintf( outstr, "(%d,PC,%s%s%s)",
			Offset,
			( AD ) ? Ax_RegNames[REG] : Dx_RegNames[REG],
			( WL ) ? ".l" : ".w",
			scale_Names[SCALE]
		);
	}

	// --

	rt->rt_CPU.M68k.mt_ArgSize += 2;

	// --

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( ds );
}

// --
