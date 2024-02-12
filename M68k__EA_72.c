
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
// -- Mode 72 - Clr (xxxx.w,pc)
// xxxx is Signed

inline static void MODE_72( struct M68kStruct *ms, char *outstr )
{
struct HunkLabel *hl;
uint32_t adr;
int16_t val;
uint8_t *mem;
char labname[ MAX_LabelName + 8 ];
int pos;

	mem = ms->ms_MemoryBuf;
	pos = ms->ms_ArgSize;

	val = (( mem[pos] << 8 ) + ( mem[pos+1] << 0 ));

	adr = ms->ms_MemoryAdr + 2 + val;

	if ( ms->ms_HunkStruct->hs_PassNr == 1 )
	{
		// We can get away with AddLabel2, as this is a PC function, 
		// so we can handle Labels out side Hunk Memory area
		hl = Hunk_AddLabel2( ms->ms_HunkStruct, ms->ms_HunkNode, adr, 0 );
//		hl = Hunk_AddLabel( ms->ms_HunkStruct, adr, RefType );
	}
	else
	{
		hl = Hunk_FindLabel( ms->ms_HunkStruct, adr );

		if ( hl == NULL )
		{
			printf( "%s:%04d: Error finding label at %08x\n", __FILE__, __LINE__, ms->ms_MemoryAdr );
			ms->ms_DecodeStatus = MSTAT_Error;
			goto bailout;
		}
	}

	if (( hl ) && ( hl->hl_Label_Name[0] ))
	{
		BuildLabelString( hl, labname );

		sprintf( outstr, "(%s,PC)", labname );
	}
	else
	{
		sprintf( outstr, "($%08x,PC)", adr );
	}

	if (( hl ) && ( ms->ms_CurRegister ))
	{
		ms->ms_CurRegister->mr_Type = RT_Label;
		ms->ms_CurRegister->mr_LabelNode = hl;
	}

	ms->ms_ArgSize += 2;

bailout:

	return;
}

// --
