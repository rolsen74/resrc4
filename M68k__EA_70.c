
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
// -- Mode 70 - move.l $0004.w

inline static void MODE_70( struct M68kStruct *ms, char *outstr )
{
struct HunkLabel *hl;
int16_t val;
uint8_t *mem;
int pos;

	mem = ms->ms_MemoryBuf;
	pos = ms->ms_ArgSize;

	val = (( mem[pos] << 8 ) | ( mem[pos+1] << 0 ));

	if ( ms->ms_DoExternal )
	{
		hl = Hunk_AddExtLabel( ms->ms_HunkStruct, val, LT_Unset );

		if (( hl ) && ( hl->hl_Label_Name[0] ))
		{
			sprintf( outstr, "%s.w", hl->hl_Label_Name );
		}
		else
		{
			sprintf( outstr, "($%04x).w", val );
		}
	}
	else
	{
		if (( ms->ms_IsPea ) && ( val < 0 ))
		{
			sprintf( outstr, "(%d).w", val );
		}
		else
		{
			sprintf( outstr, "($%04x).w", val );
		}
	}

	if ( ms->ms_CurRegister )
	{
		ms->ms_CurRegister->mr_Type = RT_Address;
		ms->ms_CurRegister->mr_Address = val;
	}

	ms->ms_ArgSize += 2;
}

// --
