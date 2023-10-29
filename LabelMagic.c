 
/*
 * Copyright (c) 2014-2023 Rene W. Olsen < renewolsen @ gmail . com >
 *
 * This software is released under the GNU General Public License, version 3.
 * For the full text of the license, please visit:
 * https://www.gnu.org/licenses/gpl-3.0.html
 *
 * You can also find a copy of the license in the LICENSE file included with this software.
 */

// --

#include "ReSrc4.h"

// --

static int LabelCount = 0;

static int AdjustLabels( struct HunkStruct *hs, struct HunkNode *hn )
{
struct HunkLabel *Sec;
struct HunkLabel *hl;
int32_t adr;
int32_t size;
int error;

	error = true;

	// --

	adr = hn->hn_MemoryAdr;
	size= hn->hn_MemorySize;

	// -- We allways add a Start Label for Hunks

	Sec = Hunk_AddLabel2( hs, hn, hn->hn_MemoryAdr, LT_Unknown );

	if ( Sec == NULL )
	{
		printf( "%s:%04d: Error adding label\n", __FILE__, __LINE__ );
		goto bailout;
	}

	if ( Sec->hl_Label_Name[0] == 0 )
	{
		snprintf( Sec->hl_Label_Name, MAX_LabelName - 1, "%s%04X", SecNames, hn->hn_HunkNr );
		Sec->hl_Label_Name[ MAX_LabelName - 1 ] = 0;
	}

	// --

	hl = myGetHead( & hn->hn_Labels );

	// Process Labels BEFORE Hunk Memory
	while(( hl ) && ( hl->hl_Label_Offset < 0 ))
	{
		hl->hl_Parent = Sec;
		snprintf( hl->hl_Label_Name, MAX_LabelName - 1, "!ERROR!" );
		hl = myGetNext( hl );
	}

	// Process Labels INSIDE Hunk Memory Area
	while(( hl ) && ( hl->hl_Label_Offset <= size ))
	{
		if ( hl->hl_Label_Name[0] == 0 )
		{
			if ( hl->hl_Parent )
			{
				snprintf( hl->hl_Label_Name, MAX_LabelName - 1, "!ERROR!" );
			}
			else
			{
				snprintf( hl->hl_Label_Name, MAX_LabelName - 1, "%s%04X", LabNames, ++LabelCount );
				hl->hl_Label_Name[ MAX_LabelName - 1 ] = 0;
			}
		}

		hl = myGetNext( hl );
	}

	// Process Label AFTER Hunk Memory
	if (( hl ) && ( hl->hl_Label_Offset > size ))
	{
		Sec = Hunk_AddLabel2( hs, hn, adr+size, LT_Unknown );

		if ( Sec == NULL )
		{
			printf( "%s:%04d: Error adding label\n", __FILE__, __LINE__ );
			goto bailout;
		}

		if ( Sec->hl_Label_Name[0] == 0 )
		{
			snprintf( Sec->hl_Label_Name, MAX_LabelName - 1, "%s%04X", LabNames, ++LabelCount );
			Sec->hl_Label_Name[ MAX_LabelName - 1 ] = 0;
		}

		while( hl )
		{
			hl->hl_Parent = Sec;
			snprintf( hl->hl_Label_Name, MAX_LabelName - 1, "!ERROR!" );
			hl = myGetNext( hl );
		}
	}

	error = false;

bailout:

	return( error );
}

// --

static int CreateLabelNames( struct HunkStruct *hs )
{
struct HunkNode *hn;
int error;
int cnt;

	error = true;

	// --

	hn = hs->hs_HunkArray[ hs->hs_HunkFirst ].hi_HunkNode;

	if ( AdjustLabels( hs, hn ))
	{
		printf( "%s:%04d: Error Label Adjust Failed\n", __FILE__, __LINE__ );
		goto bailout;
	}

	// --

	for( cnt=0 ; cnt < hs->hs_HunkArraySize ; cnt++ )
	{
		if ( cnt == hs->hs_HunkFirst )
		{
			continue;
		}

		hn = hs->hs_HunkArray[ cnt ].hi_HunkNode;

		if ( AdjustLabels( hs, hn ))
		{
			printf( "%s:%04d: Error Label Adjust Failed\n", __FILE__, __LINE__ );
			goto bailout;
		}
	}

	// --

	error = false;

bailout:

	return( error );
}

// --

static int LabAdjust( struct HunkStruct *hs, struct HunkLabel *hl, int32_t adr, int32_t size, int type )
{
struct HunkLabel *parent;
int error;

	error = false;

	while(( hl ) && ( hl->hl_Label_Address < adr ))
	{
		hl = myGetNext( hl );
	}

	if ( hl == NULL )
	{
		goto bailout;
	}

	/**/ if ( hl->hl_Label_Address == adr )
	{
		parent = hl;

		hl = myGetNext( hl );
	}
	else if ( hl->hl_Label_Address < adr + size )
	{
		// Do we need a NEW Parent Label
		parent = Hunk_AddLabel2( hs, hl->hl_HunkNode, adr, type );

		if ( parent == NULL )
		{
			error = true;
			printf( "%s:%04d: Error adding label at %08x\n", __FILE__, __LINE__, adr );
			goto bailout;
		}
	}
	else
	{
		goto bailout;
	}

	while(( hl ) && ( hl->hl_Label_Address < adr + size ))
	{
//		printf( "Move HL %08x -> Parent %08x\n", hl->hl_Label_Address, parent->hl_Label_Address );

		hl->hl_Parent = parent;
		hl = myGetNext( hl );
	}

bailout:

	return( error );
}

// --

static int Decode_String( struct M68kStruct *ms, struct HunkLabel *hl, int32_t max, int32_t *l )
{
int error;

	error = true;

	if ( hl->hl_Label_Type != LT_String )
	{
		printf( "%s:%04d: Error expected String label at %08x\n", __FILE__, __LINE__, ms->ms_StartAddr );
		goto bailout;
	}

// printf( "Label: Size %d, Max %d\n", hl->hl_Label_Size, max );

	if ( hl->hl_Label_Size > max )
	{
		printf( "%s:%04d: Error size mismatch at %08x ( %d != %d )\n", __FILE__, __LINE__, ms->ms_StartAddr, hl->hl_Label_Size, max );
		goto bailout;
	}

	*l = hl->hl_Label_Size;

	#if 0
	if ( LabAdjust( hs, hl, ms->ms_StartAddr, hl->hl_Label_Size, LT_Unknown ))
	{
		printf( "%s:%04d: Error adjusting label at %08x\n", __FILE__, __LINE__, ms->ms_StartAddr );
		goto bailout;
	}
	#endif

	error = false;

bailout:

	return( error );
}

// --

static int Decode_RelativeWord( struct M68kStruct *ms, struct HunkLabel *hl, int32_t pos, int32_t *l )
{
struct HunkLabel *brance;
struct HunkLabel *rel;
int32_t adr;
int32_t tableadr;
int16_t *tablemem;
int16_t off;
int error;
int size;
int len;
int cnt;

	error = true;

	len = 0;

	rel = hl->hl_Label_Ref;

	if ( rel == NULL )
	{
		printf( "%s:%04d: Decode_RelativeWord Error\n", __FILE__, __LINE__ );
		goto bailout;
	}

	tablemem = (void *) hl->hl_Label_Memory;
	tableadr = hl->hl_Label_Address;

	size = hl->hl_Label_Size/2;

	for( cnt=0 ; cnt < size ; cnt++ )
	{
		// --

		while(( hl ) && ( hl->hl_Label_Offset > pos ))
		{
			hl = myGetNext( hl );
		}

		// --

		off = SWAP16( tablemem[cnt] );
		adr = rel->hl_Label_Address;
		adr += off;

		// --

		brance = Hunk_FindLabel( ms->ms_HunkStruct, adr );

		if ( brance == NULL )
		{
			printf( "%s:%04d: Decode_RelativeWord Error\n", __FILE__, __LINE__ );
			goto bailout;
		}

		// --

		if ( LabAdjust( ms->ms_HunkStruct, hl, tableadr, 2, LT_Unknown ))
		{
			printf( "%s:%04d: Error adjusting label at %08x\n", __FILE__, __LINE__, tableadr );
			goto bailout;
		}

		// --
	
		tableadr += 2;
		len += 2;
		pos += 2;
	}

	*l = len;

	error = false;

bailout:

	return( error );
}

// --

static int Decode_Code(
	struct HunkStruct *hs,
	struct M68kStruct *ms,
	struct HunkLabel *hl,
	int32_t size,
	uint8_t *type,
	uint8_t *mem,
	int32_t pos,
	int32_t *len_ptr )
{
struct HunkNode *hn;
int32_t len;
int error;

	error = true;

	len = 0;

	hn = ms->ms_HunkNode;

	if (( hl ) && ( hl->hl_Label_Offset == pos ))
	{
		memcpy( & ms->ms_Registers[0], & hl->hl_Registers[0], sizeof( struct M68kRegister ) * 16 );
	}
	else
	{
		for( int cnt=0 ; cnt<16 ; cnt++ )
		{
			ms->ms_Registers[cnt].mr_Type = RT_Unknown;
		}
	}

	while( true )
	{
		if ( type[pos] != MT_Code )
		{
			break;
		}

		if ( pos >= size )
		{
			break;
		}

		while(( hl ) && ( hl->hl_Label_Offset < pos ))
		{
			hl = myGetNext( hl );
		}

		ms->ms_StartAddr = hn->hn_MemoryAdr + pos;
		ms->ms_MemoryAdr = hn->hn_MemoryAdr + pos;
		ms->ms_MemoryBuf = & mem[ pos ];
		ms->ms_MemoryType = & hn->hn_MemoryType[ pos ];

		M68k_Decode( ms );

		if ( ms->ms_DecodeStatus == MSTAT_Error )
		{
			break;
		}

		// --

		if ( LabAdjust( hs, hl, ms->ms_StartAddr, ms->ms_OpcodeSize, LT_Code ))
		{
			printf( "%s:%04d: Error adjusting label at %08x\n", __FILE__, __LINE__, ms->ms_StartAddr );
			goto bailout;
		}

		// --

		pos += ms->ms_OpcodeSize;
		len += ms->ms_OpcodeSize;

		if ( ms->ms_ClearRegMask )
		{
			for( int cnt=0 ; cnt<16 ; cnt++ )
			{
				if ( ms->ms_ClearRegMask & ( 1 << cnt ))
				{
					ms->ms_Registers[cnt].mr_Type = RT_Unknown;
				}
			}

			ms->ms_ClearRegMask = 0;
		}

		if ( ms->ms_LastOpcode )
		{
			break;
		}
	}

	*len_ptr = len;

	error = false;

bailout:

    return( error );
}

// --

static int Decode_Data( 
	struct HunkStruct *hs, 
	struct M68kStruct *ms, 
	struct HunkLabel *hl,
	struct HunkRef **hr_ptr, 
	int32_t max,
	int32_t *len_ptr )
{
struct HunkRef *hr;
int32_t size;
int error;
int loop;
int mm;
int m;

	error = true;

	hr = *hr_ptr;

	if ( hr )
	{
		if ( hr->hr_Address == ms->ms_MemoryAdr )
		{
			max = 4;			
		}
		else
		{
			m = hr->hr_Address - ms->ms_MemoryAdr;

			mm = MIN( max, m );

			if ( mm >= 4 )
			{
				max  = mm / 4;
				max *= 4;
			}
			else if (( m == 1 ) || ( m == 2 ) || ( m == 3 ))
			{
				max = mm;
			}
		}
	}

	if ( max > 3 )
	{
		// 1 Long or more

		if (( hr ) && ( hr->hr_Address == ms->ms_MemoryAdr ))
		{
			size = 4;

			if ( LabAdjust( hs, hl, ms->ms_MemoryAdr, size, LT_Unknown ))
			{
				printf( "%s:%04d: Error adjusting label at %08x\n", __FILE__, __LINE__, ms->ms_StartAddr );
				goto bailout;
			}
		}
		else
		{
			loop = ( max >= 16 ) ? 4 : max / 4;

			size = loop * 4;
		}
	}
	else if ( max > 1 )
	{
		// 1 Word

		size = 2;
	}
	else
	{
		// 1 Byte

		size = 1;
	}

	*len_ptr = size;

    error = false;

bailout:

	return( error );
}

// --

static int Decode_BSS( int32_t max, int32_t *len_ptr )
{
	*len_ptr = max;

	return( false );
}

// --

static int Scan_Generate_Hunk( struct HunkStruct *hs, struct M68kStruct *ms, struct HunkNode *hn )
{
struct HunkLabel *nexthl;
struct HunkLabel *hl;
struct HunkRef *nexthr;
struct HunkRef *hr;
uint8_t *type;
uint8_t *mem;
int32_t size;
int32_t max;
int32_t len;
int32_t pos;
int32_t cnt;
int error;
int mt;

	error = true;

    // --

	hl		= myGetHead( & hn->hn_Labels );
	hr		= myGetHead( & hn->hn_Refs );
	pos		= 0;
	mem		= hn->hn_MemoryBuf;
	size	= hn->hn_MemorySize;
	type	= hn->hn_MemoryType;

	while( true )
	{
		if ( pos >= size )
		{
			break;
		}

		// --

		while(( hl ) && ( hl->hl_Label_Offset < pos ))
		{
			hl = myGetNext( hl );
		}

		while(( hr ) && ( hr->hr_Offset < pos ))
		{
			hr = myGetNext( hr );
		}

		// -- Set Label

		if (( hl ) && ( hl->hl_Label_Offset == pos ))
		{
			ms->ms_Ptr_Label = hl;
		}
		else
		{
			ms->ms_Ptr_Label = NULL;
		}

		// ----- Calc max Bytes

		max = size - pos;

		// -- Labels

		if ( hl )
		{
			if ( hl->hl_Label_Offset > pos )
			{
				max = MIN( max, hl->hl_Label_Offset - pos );
			}
			else
			{
				nexthl = myGetNext( hl );

				if ( nexthl )
				{
					max = MIN( max, nexthl->hl_Label_Offset - pos );
				}
			}
		}

		// -- Refs

		if ( hr )
		{
			if ( hr->hr_Offset > pos )
			{
				max = MIN( max, hr->hr_Offset - pos );
			}
			else
			{
				nexthr = myGetNext( hr );

				if ( nexthr )
				{
					max = MIN( max, nexthr->hr_Offset - pos );
				}
			}
		}

		// -- Memory Type

		mt = 0;

		for( cnt=pos ; cnt<pos+max ; cnt++ )
		{
			if ( hn->hn_MemoryType[ pos ] != hn->hn_MemoryType[ cnt ] )
			{
				max = mt;
				break;				
			}
			else
			{
				mt++;
			}
		}

		// --

		len = 0;

		ms->ms_StartAddr = hn->hn_MemoryAdr + pos;
		ms->ms_MemoryAdr = hn->hn_MemoryAdr + pos;
		ms->ms_MemoryBuf = & mem[ pos ];
		ms->ms_MemoryType = & hn->hn_MemoryType[ pos ];

		/**/ if ( type[pos] == MT_Unset )
		{
			/**/ if ( hn->hn_HunkType == HUNK_BSS )
			{
				if ( Decode_BSS( max, & len ))
				{
					printf( "%s:%04d: Decoding error\n", __FILE__, __LINE__ );
					goto bailout;
				}
			}
			else if ( hn->hn_DataSize <= pos )
			{
				if ( Decode_BSS( max, & len ))
				{
					printf( "%s:%04d: Decoding error\n", __FILE__, __LINE__ );
					goto bailout;
				}
			}
			else
			{
				if ( Decode_Data( hs, ms, hl, & hr, max, & len ))
				{
					printf( "%s:%04d: Decoding error\n", __FILE__, __LINE__ );
					goto bailout;
				}
			}
		}
		else if ( type[pos] == MT_Code )
		{
			if ( Decode_Code( hs, ms, hl, size, type, mem, pos, & len ))
			{
				printf( "%s:%04d: Decoding error\n", __FILE__, __LINE__ );
				goto bailout;
			}
		}
		else if ( type[pos] == MT_Data )
		{
			/**/ if (( hl ) && ( hl->hl_Label_Offset == pos ) && ( hl->hl_Label_Type == LT_Code ))
			{
				if ( Decode_Code( hs, ms, hl, size, type, mem, pos, & len ))
				{
					printf( "%s:%04d: Decoding error\n", __FILE__, __LINE__ );
					goto bailout;
				}
			}
			else if (( hl ) && ( hl->hl_Label_Offset == pos ) && ( hl->hl_Label_Type == LT_RelativeWord ))
			{
				if ( Decode_RelativeWord( ms, hl, pos, & len ))
				{
					printf( "%s:%04d: Decoding error\n", __FILE__, __LINE__ );
					goto bailout;
				}
			}
			else if (( hl ) && ( hl->hl_Label_Offset == pos ) && ( hl->hl_Label_Type == LT_String ))
			{
				if ( Decode_String( ms, hl, max, & len ))
				{
					printf( "%s:%04d: Decoding error\n", __FILE__, __LINE__ );
					goto bailout;
				}
			}
			else
			{
				if ( Decode_Data( hs, ms, hl, & hr, max, & len ))
				{
					printf( "%s:%04d: Decoding error\n", __FILE__, __LINE__ );
					goto bailout;
				}
			}
		}
		else
		{
			printf( "%s:%04d: Unknown Memory Type (%d)\n", __FILE__, __LINE__, type[ pos ] );
			goto bailout;
		}

		if ( len == 0 )
		{
			printf( "%s:%04d: Decoding length error\n", __FILE__, __LINE__ );
			goto bailout;
		}

		pos += len;
	}

	// --

	error = false;

bailout:

	return( error );
}

// --

int LabelMagic( struct HunkStruct *hs )
{
struct HunkLabel *hl;
struct M68kStruct ms;
struct HunkNode *hn;
char argbuf[256];
int error;
int cnt;

	error = true;

	// --

	hs->hs_PassNr = 2;

	// --

	memset( & ms, 0, sizeof( ms ));

	ms.ms_HunkStruct = hs;
	ms.ms_Buf_Argument = & argbuf[0];

    // --

	hn = hs->hs_HunkArray[ hs->hs_HunkFirst ].hi_HunkNode;

	ms.ms_HunkNode = hn;

	if ( Scan_Generate_Hunk( hs, & ms, hn ))
	{
		printf( "%s:%04d: Error generating hunk labels\n", __FILE__, __LINE__ );
		goto bailout;
	}

	// --

	for( cnt=0 ; cnt < hs->hs_HunkArraySize ; cnt++ )
	{
		if ( cnt == hs->hs_HunkFirst )
		{
			continue;
		}

		hn = hs->hs_HunkArray[ cnt ].hi_HunkNode;

		ms.ms_HunkNode = hn;

		if ( Scan_Generate_Hunk( hs, & ms, hn ))
		{
			printf( "%s:%04d: Error generating hunk labels\n", __FILE__, __LINE__ );
			goto bailout;
		}
	}

	// -- Create External Label Names

	hl = myGetHead( & hs->hs_ExtLabelList );

	while( hl )
	{
		snprintf( hl->hl_Label_Name, MAX_LabelName - 1, "%s%04X", ExtNames, hl->hl_Label_Offset );

		hl = myGetNext( hl );
	}

	// -- Create Hunk Label Names

	if ( CreateLabelNames( hs ))
	{
		printf( "%s:%04d: Error creating label names\n", __FILE__, __LINE__ );
		goto bailout;	
	}

	// --

	error = false;

bailout:

	return( error );
}

// --
