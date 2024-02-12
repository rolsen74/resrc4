
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
// -- Mode 74

inline static void MODE_74( struct M68kStruct *ms, char *outstr )
{
struct HunkLabel *hl;
struct HunkRef *isRef;
uint8_t *mem;
char labname[ MAX_LabelName + 8 ];
int type;
int pos;

	mem = ms->ms_MemoryBuf;
	pos = ms->ms_ArgSize;

	switch( ms->ms_ArgType )
	{
		case OS_Byte:
		{
			uint8_t val;

			val = mem[pos+1] & 0x00ff;

			/**/ if ( ms->ms_DecMode == 1 )	// Signed
			{
				sprintf( outstr, "#%d", val );
			}
			else if ( ms->ms_DecMode == 2 ) // Unsigned
			{
				sprintf( outstr, "#%u", val );
			}
			else
			{
				sprintf( outstr, "#$%02x", val );
			}

			ms->ms_ArgSize += 2;
			break;
		}

		case OS_Word:
		{
			uint16_t val;

			val = (( mem[pos] << 8 ) + ( mem[pos+1] << 0 ));

			/**/ if ( ms->ms_DecMode == 1 ) // Signed
			{
				sprintf( outstr, "#%d", val );
			}
			else if ( ms->ms_DecMode == 2 ) // Unsigned
			{
				sprintf( outstr, "#%u", val );
			}
			else
			{
				sprintf( outstr, "#$%04x", val );
			}

			ms->ms_ArgSize += 2;
			break;
		}

		case OS_Long:
		{
			uint32_t adr;

			adr = (( mem[pos] << 24 ) + ( mem[pos+1] << 16 ) + ( mem[pos+2] << 8 ) + ( mem[pos+3] << 0 ));

			isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

			if ( isRef )
			{
				isRef->hr_Used = true;

				if ( ms->ms_HunkStruct->hs_PassNr == 1 )
				{
					type = ( ms->ms_LibCall ) ? LT_Code : LT_Unset ;

					hl = Hunk_AddLabel( ms->ms_HunkStruct, adr, type );

					if ( hl == NULL )
					{
						printf( "%s:%04d: Error adding label at %08x\n", __FILE__, __LINE__, ms->ms_MemoryAdr );
						ms->ms_DecodeStatus = MSTAT_Error;
						goto bailout;
					}
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

					sprintf( outstr, "#%s", labname );
				}
				else
				{
					/**/ if ( ms->ms_DecMode == 1 ) // Signed
					{
						sprintf( outstr, "#%d", adr );
					}
					else if ( ms->ms_DecMode == 2 ) // Unsigned
					{
						sprintf( outstr, "#%u", adr );
					}
					else
					{
						sprintf( outstr, "#$%08x", adr );
					}
				}

				if (( hl ) && ( ms->ms_CurRegister ))
				{
					ms->ms_CurRegister->mr_Type = RT_Label;
					ms->ms_CurRegister->mr_LabelNode = hl;
				}
			}
			else
			{
				/**/ if ( ms->ms_DecMode == 1 ) // Signed
				{
					sprintf( outstr, "#%d", adr );
				}
				else if ( ms->ms_DecMode == 2 ) // Unsigned
				{
					sprintf( outstr, "#%u", adr );
				}
				else
				{
					sprintf( outstr, "#$%08x", adr );
				}

				if ( ms->ms_CurRegister )
				{
					ms->ms_CurRegister->mr_Type = RT_Address;
					ms->ms_CurRegister->mr_Address = adr;
				}
			}

			ms->ms_ArgSize += 4;
			break;
		}

		case OS_Single:
		{
			// Bits: 1 sign, 8 expo, 23 frac

			sprintf( outstr, "#$" );

			for( int cnt=0 ; cnt<4 ; cnt++ )
			{
				int len = strlen( outstr );

				sprintf( & outstr[len], "%02x", mem[pos+cnt] );
			}

			ms->ms_ArgSize += 4;
			break;
		}

		case OS_Double:
		{
			// Bits: 1 sign, 11 expo, 52 frac

			sprintf( outstr, "#$" );

			for( int cnt=0 ; cnt<8 ; cnt++ )
			{
				int len = strlen( outstr );

				sprintf( & outstr[len], "%02x", mem[pos+cnt] );
			}

			ms->ms_ArgSize += 8;
			break;
		}

		case OS_Extended:
		{
			// Bits: 1 sign, 15 expo, 16 ZERO, 64 mantissa

			sprintf( outstr, "#$" );

			for( int cnt=0 ; cnt<12 ; cnt++ )
			{
				int len = strlen( outstr );

				sprintf( & outstr[len], "%02x", mem[pos+cnt] );
			}

			ms->ms_ArgSize += 12;
			break;
		}

//	OS_Packed,		// fmove.p

		default:
		{
			printf( "%s:%04d: Unsupported EA mode at %08x\n", __FILE__, __LINE__, ms->ms_MemoryAdr );
			ms->ms_DecodeStatus = MSTAT_Error;
			break;
		}
	}

bailout:

	return;
}

// --
