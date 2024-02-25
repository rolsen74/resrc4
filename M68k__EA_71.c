
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
// -- Mode 71 - move.l $00000004

inline static void MODE_71( struct M68kStruct *ms, char *outstr )
{
struct HunkLabel *hl;
struct HunkRef *isRef;
uint32_t val;
uint8_t *mem;
int type;
int pos;

	mem = ms->ms_MemoryBuf;
	pos = ms->ms_ArgSize;
	val = (( mem[pos] << 24 ) | ( mem[pos+1] << 16 ) | ( mem[pos+2] << 8 ) | ( mem[pos+3] << 0 ));

	isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

	if ( isRef )
	{
		isRef->hr_Used = true;

		if ( ms->ms_HunkStruct->hs_PassNr == 1 )
		{
			type = ( ms->ms_LibCall ) ? LT_Code : LT_Unset ;

			hl = Hunk_AddLabel( ms->ms_HunkStruct, val, type );

			if ( hl == NULL )
			{
				printf( "AddLabel failed at %08x\n", ms->ms_MemoryAdr );
			}
		}
		else
		{
			hl = Hunk_FindLabel( ms->ms_HunkStruct, val );

			if ( hl == NULL )
			{
				printf( "%s:%04d: Error finding label at %08x\n", __FILE__, __LINE__, ms->ms_MemoryAdr );
				ms->ms_DecodeStatus = MSTAT_Error;
				goto bailout;
			}
		}

		if (( hl ) && ( hl->hl_Label_Name[0] ))
		{
			BuildLabelString( hl, outstr );
		}
		else
		{
			// Not an external as it has a Refs 
			sprintf( outstr, "($%08x).l", val );
		}

		if (( hl ) && ( ms->ms_CurRegister ))
		{
			ms->ms_CurRegister->mr_Type = RT_Label;
			ms->ms_CurRegister->mr_LabelNode = hl;
		}
	}
	else
	{
		if ( ms->ms_DoExternal )
		{
			hl = Hunk_AddExtLabel( ms->ms_HunkStruct, val, LT_Unset );

			if (( hl ) && ( hl->hl_Label_Name[0] ))
			{
				sprintf( outstr, "%s", hl->hl_Label_Name );
			}
			else
			{
				sprintf( outstr, "($%08x).l", val );
			}
		}
		else
		{
			sprintf( outstr, "($%08x).l", val );
		}

		if ( ms->ms_CurRegister )
		{
			ms->ms_CurRegister->mr_Type = RT_Address;
			ms->ms_CurRegister->mr_Address = val;
		}
	}

	ms->ms_ArgSize += 4;

bailout:

	return;
}

// --
