
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
// -- Mode 50 - jsr (d16,Ax)

inline static void MODE_50( struct M68kStruct *ms, char *outstr )
{
int16_t val;
uint8_t *mem;
char *lvostr;
int pos;

	mem = ms->ms_MemoryBuf;
	pos = ms->ms_ArgSize;

	val = (( mem[pos] << 8 ) | ( mem[pos+1] << 0 ));

	if (( ms->ms_LibCall ) 
	&&	( ms->ms_Registers[ REG_Ax + ms->ms_ArgEReg ].mr_Type & 0xffff ) == RT_Library )
	{
		lvostr = M68k_FindLibFunc( ms, val );
	}
	else
	{
		lvostr = NULL;
	}

	if ( lvostr )
	{
		sprintf( outstr, "(%s,%s)", lvostr, Ax_RegNames[ ms->ms_ArgEReg ] );
	}
	else
	{
		// Most proberly a Library call, so we use Decimal
		sprintf( outstr, "(%d,%s)", val, Ax_RegNames[ ms->ms_ArgEReg ] );
	}

	ms->ms_ArgSize += 2;
}

// --
