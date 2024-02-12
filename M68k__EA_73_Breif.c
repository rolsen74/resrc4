
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
// -- Mode 73 - Brief Extension Word Format

inline static void MODE_73_Brief( struct M68kStruct *ms, char *outstr )
{
struct HunkLabel *hl;
uint32_t adr;
uint8_t *mem;
int8_t Offset;
char labname[ MAX_LabelName + 8 ];
int SCALE;
int REG;
int pos;
int AD;
int WL;

	// Brief Extension Word Format
	//
	// [A/D] [REG] [W/L] [Scale] [0] [ signed offset ]
	//   1     3     1      2     1          8
	// [  4 bit  ] [     4 bit     ] [     8 bit     ]

	// ( d8 , PC , Xn.SIZE * SCALE )

	mem = ms->ms_MemoryBuf;
	pos = ms->ms_ArgSize;

	AD		= ( mem[ pos + 0 ] & 0x80 );
	REG		= ( mem[ pos + 0 ] & 0x70 ) >> 4;
	WL		= ( mem[ pos + 0 ] & 0x08 );
	SCALE	= ( mem[ pos + 0 ] & 0x06 ) >> 1;
	Offset	= ( mem[ pos + 1 ] );

	// --

	adr = ms->ms_MemoryAdr + 2 + Offset;

	hl = Hunk_AddLabel2( ms->ms_HunkStruct, ms->ms_HunkNode, adr, 0 );

	if (( hl ) && ( hl->hl_Label_Name[0] ))
	{
		BuildLabelString( hl, labname );

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

	ms->ms_ArgSize += 2;
}

// --
