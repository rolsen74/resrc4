 
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

#include "ReSrc4.h"

// --

static int AddSourceNode( struct HunkStruct *hs, struct M68kStruct *ms );
static int AddEmptyNode( struct HunkStruct *hs, struct M68kStruct *ms );

// --

enum STRINGMODE
{
	SM_ASCII = 11,
	SM_Decimal = 19,
};

#define MAX_LINELEN		38

static int Decode_String( struct M68kStruct *ms, struct HunkLabel *hl, int32_t max, int32_t *l )
{
enum STRINGMODE mode;
struct HunkNode *hn;
int32_t adr;
uint8_t *mem;
char *arg;
char comment[128];
int linelen;
int bytes;
int error;
int size;
int chr;
int len;
int pos;
int cnt;
int p;

	error = true;

	if ( hl == NULL )
	{
		printf( "%s:%04d: NULL Pointer\n", __FILE__, __LINE__ );
		goto bailout;
	}

	hn  = ms->ms_HunkNode;
	arg = ms->ms_Buf_Argument;
	mem = hn->hn_MemoryBuf;
	adr = hl->hl_Label_Address;
	pos = hl->hl_Label_Offset;
	size= hl->hl_Label_Size;
	len = 0;

	ms->ms_Buf_Comment = comment;
	ms->ms_Str_Opcode = "dc.b";
	ms->ms_Ptr_Label = hl;

	if ( size <= 0 )
	{
		printf( "%s:%04d: Zero Length\n", __FILE__, __LINE__ );
		goto bailout;
	}

	do
	{
		// Main Loop, build one line of max 20 chars

		ms->ms_StartAddr = adr;

		mode = SM_Decimal;
		linelen = 0;
		bytes = 0;
		arg[0] = 0;
		p = pos;

		while( size > 0 )
		{
			chr = mem[pos];

			if (( chr >= 0x20 ) && ( chr < 0x7f ) && ( chr != '"' ))
			{
				// Chr is an ASCII type

				if ( mode == SM_ASCII )
				{
					// Make sure there are space for: Char

					if ( linelen > MAX_LINELEN - 1 )
					{
						break;
					}

					// We are in ascii mode, just add
					sprintf( & arg[linelen], "%c", chr );
				}
				else
				{
					// We are not in ascii mode, so lets switch

					if ( linelen == 0 )
					{
						sprintf( & arg[linelen], "\"%c", chr );
					}
					else
					{
						// Make sure there are space for: ," + %c

						if ( linelen > MAX_LINELEN - 3 )
						{
							break;
						}

						sprintf( & arg[linelen], ",\"%c", chr );
					}

					mode = SM_ASCII;
				}
			}
			else
			{
				if ( mode == SM_Decimal )
				{
					// Make sure there are space for: , %d

					if ( linelen == 0 )
					{
						// We are not in ascii mode, just add
						sprintf( & arg[linelen], "%d", chr );
					}
					else
					{
						if ( linelen > MAX_LINELEN - 4 ) // %d may be 3 bytes
						{
							break;
						}

						// We are not in ascii mode, just add
						sprintf( & arg[linelen], ",%d", chr );
					}
				}
				else
				{
					// We are in ascii mode, so lets switch

					// Make sure there are space for: ", %d

					if ( linelen > MAX_LINELEN - 5 ) // %d may be 3 bytes
					{
						break;
					}

					sprintf( & arg[linelen], "\",%d", chr );

					mode = SM_Decimal;
				}
			}

			pos++;
			size--;
			bytes++;
			linelen = strlen( arg );

			if ( chr == 10 )
			{
				break;
			}
		}

		if ( mode == SM_ASCII )
		{
			arg[linelen++] = '"';
		}

		arg[linelen] = 0;

		// --

		comment[0] = 0;

		for( cnt=0 ; cnt<bytes ; cnt++ )
		{
			linelen = strlen( comment );

			if (( cnt == 0 ) || ( cnt & 1 ))
			{
				sprintf( & comment[linelen], "%02x", mem[ p + cnt ] );
			}
			else
			{
				sprintf( & comment[linelen], " %02x", mem[ p + cnt ] );
			}
		}

		// --

		if ( AddSourceNode( ms->ms_HunkStruct, ms ))
		{
			printf( "%s:%04d: Error adding source node\n", __FILE__, __LINE__ );
			goto bailout;
		}

		ms->ms_Ptr_Label = NULL;
		adr += bytes;
		len	+= bytes;
	}
	while( size > 0 );

	if ( hl->hl_Label_Size != len )
	{
		printf( "%s:%04d: Error invalid length ( %d != %d )\n", __FILE__, __LINE__, hl->hl_Label_Size, len );
		goto bailout;
	}

	if ( len > max )
	{
		printf( "%s:%04d: Error invalid length ( %d != %d )\n", __FILE__, __LINE__, max, len );
		goto bailout;
	}

	ms->ms_Buf_Comment = NULL;

	*l = len;

	error = false;

bailout:

	return( error );
}

#undef MAX_LINELEN

// --

static int Decode_RelativeWord( struct M68kStruct *ms, struct HunkLabel *hl, int32_t pos, int32_t *l )
{
struct HunkLabel *parent;
struct HunkLabel *brance;
struct HunkLabel *rel;
struct HunkNode *hn;
int32_t adr;
int16_t *data;
int16_t off;
char buf[32];
int error;
int size;
int len;
int cnt;
int p;

	error = true;

	hn = ms->ms_HunkNode;

	len = 0;

	rel = hl->hl_Label_Ref;

	if ( rel == NULL )
	{
		printf( "%s:%04d: Decode_RelativeWord Error\n", __FILE__, __LINE__ );
		goto bailout;
	}

	data = (void *) hl->hl_Label_Memory;
	size = hl->hl_Label_Size/2;

	ms->ms_Buf_Comment = buf;

	for( cnt=0 ; cnt < size ; cnt++ )
	{
		// --

		while(( hl ) && ( hl->hl_Label_Offset > pos ))
		{
			hl = myGetNext( hl );
		}

		// --

		off = SWAP16( data[cnt] );
		adr = rel->hl_Label_Address;

		adr += off;

		brance = Hunk_FindLabel( ms->ms_HunkStruct, adr );

		if ( brance == NULL )
		{
			printf( "%s:%04d: Decode_RelativeWord Error\n", __FILE__, __LINE__ );
			goto bailout;
		}

		ms->ms_StartAddr = hn->hn_MemoryAdr + pos;

		ms->ms_Str_Opcode = "dc.w";

		// --

		parent = brance->hl_Parent;

		if ( parent )
		{
			int off = brance->hl_Label_Offset - parent->hl_Label_Offset;

			if ( off < 0 )
			{
				sprintf( ms->ms_Buf_Argument, "(%s%d", parent->hl_Label_Name, off );
			}
			else
			{
				sprintf( ms->ms_Buf_Argument, "(%s+%d", parent->hl_Label_Name, off );
			}
//			sprintf( ms->ms_Buf_Argument, "(%s+%d", parent->hl_Label_Name, brance->hl_Label_Offset - parent->hl_Label_Offset );
		}
		else
		{
			sprintf( ms->ms_Buf_Argument, "(%s", brance->hl_Label_Name );
		}

		// --

		p = strlen( ms->ms_Buf_Argument );

		parent = rel->hl_Parent;

		if ( parent )
		{
			int off = rel->hl_Label_Offset - parent->hl_Label_Offset;

			if ( off < 0 )
			{
				sprintf( ms->ms_Buf_Argument, "-%s%d)", parent->hl_Label_Name, off );
			}
			else
			{
				sprintf( ms->ms_Buf_Argument, "-%s+%d)", parent->hl_Label_Name, off );
			}
//			sprintf( & ms->ms_Buf_Argument[p], "-%s+%d)", parent->hl_Label_Name, rel->hl_Label_Offset - parent->hl_Label_Offset );
		}
		else
		{
			sprintf( & ms->ms_Buf_Argument[p], "-%s)", rel->hl_Label_Name );
		}

		// --

//		sprintf( ms->ms_Buf_Argument, "(%s-%s)", brance->hl_Label_Name, rel->hl_Label_Name );
		sprintf( ms->ms_Buf_Comment, "%02x%02x", hn->hn_MemoryBuf[ pos + 0 ], hn->hn_MemoryBuf[ pos + 1 ] );

		if (( hl ) && ( hl->hl_Label_Offset == pos ))
		{
			ms->ms_Ptr_Label = hl;
			
			hl = myGetNext( hl );
		}
		else
		{
			ms->ms_Ptr_Label = NULL;
		}

		if ( AddSourceNode( ms->ms_HunkStruct, ms ))
		{
			printf( "%s:%04d: Error adding source node\n", __FILE__, __LINE__ );
			goto bailout;
		}

		len += 2;
		pos += 2;
	}

	#if 0
	if ( AddEmptyNode( ms->ms_HunkStruct, ms ))
	{
		printf( "%s:%04d: Error adding source node\n", __FILE__, __LINE__ );
		goto bailout;
	}
	#endif

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
uint32_t len;
char buf[256];
int error;
int cnt;

	ms->ms_Buf_Comment = buf;

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

		if (( hl ) && ( hl->hl_Label_Offset == pos ))
		{
			ms->ms_Ptr_Label = hl;
		}
		else
		{
			ms->ms_Ptr_Label = NULL;
		}

		ms->ms_StartAddr = hn->hn_MemoryAdr + pos;
		ms->ms_MemoryAdr = hn->hn_MemoryAdr + pos;
		ms->ms_MemoryBuf = & mem[ pos ];
		ms->ms_MemoryType = & hn->hn_MemoryType[ pos ];

		M68k_Decode( ms );

		if ( ms->ms_DecodeStatus != MSTAT_Opcode )
		{
			break;
		}

		// --

		buf[0] = 0;

		for( cnt=0 ; cnt<ms->ms_OpcodeSize ; cnt++ )
		{
			int p = strlen( buf );

			if (( cnt & 1 ) || ( cnt == 0 ))
			{
				sprintf( & buf[p], "%02x", mem[pos+cnt] );
			}
			else
			{
				sprintf( & buf[p], " %02x", mem[pos+cnt] );
			}
		}

		if ( AddSourceNode( hs, ms ))
		{
			printf( "%s:%04d: Error adding source node\n", __FILE__, __LINE__ );
			goto bailout;
		}

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
			if ((( ms->ms_Opcode & 0xffff0000 ) != ( 0x4ef90000 ))	// Last Opcode == Jmp
			|| ( mem[pos+0] != 0x4e ) || ( mem[pos+1] != 0xf9 ))	// Next Opcode == Jmp
			{
				if ( AddEmptyNode( hs, ms ))
				{
					printf( "%s:%04d: Error adding source node\n", __FILE__, __LINE__ );
					goto bailout;
				}
			}
			break;
		}
	}

	*len_ptr = len;

	error = false;

bailout:

	ms->ms_Buf_Comment = NULL;

    return( error );
}

// --

static int Decode_Data( 
	struct HunkStruct *hs, 
	struct M68kStruct *ms, 
	struct HunkLabel *hl,
	struct HunkRef *hr, 
	int32_t max,
	int32_t *len_ptr )
{
struct HunkLabel *newhl;
uint32_t offset;
uint32_t size;
uint32_t val;
uint32_t len;
uint8_t c;
char buf[256];
int doblk;
int doptr;
int error;
int loop;
int cnt;
int mm;
int m;

	ms->ms_Buf_Comment = NULL;

	error = true;
	doblk = false;
	doptr = false;

	if (( hr ) && ( hr->hr_Address == ms->ms_MemoryAdr ))
	{
		hr->hr_Used = true;
		doptr = true;
	}
	else 
	{
		if (( max > 15 ) && (( hl ) && ( hl->hl_Label_Address == ms->ms_MemoryAdr )))
		{
			c = ms->ms_MemoryBuf[ 0 ];

			for( cnt=1 ; cnt<max ; cnt++ )
			{
				if ( ms->ms_MemoryBuf[ cnt ] != c )
				{
					break;
				}
			}

			if ( cnt == max )
			{
				doblk = true;
			}
		}

		if ( doblk == false )
		{
			mm = max;

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

	if ( doptr )
	{
		ms->ms_Str_Opcode = "dc.l";

		val =  (( ms->ms_MemoryBuf[ 0 ] << 24 )
			|   ( ms->ms_MemoryBuf[ 1 ] << 16 )
			|   ( ms->ms_MemoryBuf[ 2 ] <<  8 )
			|   ( ms->ms_MemoryBuf[ 3 ] <<  0 ));

		ms->ms_Buf_Argument[0] = 0;

		newhl = Hunk_FindLabel( ms->ms_HunkStruct, val );
//		newhl = Hunk_AddLabel( ms->ms_HunkStruct, val, LT_Unset );
//		newhl = Hunk_AddLabel2( ms->ms_HunkStruct, ms->ms_HunkNode, val, LT_Unset );

		if ( newhl )
		{
			BuildLabelString( newhl, ms->ms_Buf_Argument );
		}

		ms->ms_Buf_Comment = buf;

		sprintf( buf, "%02x%02x %02x%02x",
			ms->ms_MemoryBuf[ 0 ],
			ms->ms_MemoryBuf[ 1 ],
			ms->ms_MemoryBuf[ 2 ],
			ms->ms_MemoryBuf[ 3 ]
		);

		size = 4;
	}
	else if ( doblk )
	{
		ms->ms_Str_Opcode = "blk.b";
		size = max;

		if ( ms->ms_MemoryBuf[ 0 ] == 0 )
		{
			sprintf( ms->ms_Buf_Argument, "%d,0", max );
		}
		else
		{
			sprintf( ms->ms_Buf_Argument, "%d,$%02x", max, ms->ms_MemoryBuf[ 0 ] );
		}
	}
	else if ( max > 3 )
	{
		// 1 Long or more

		ms->ms_Str_Opcode = "dc.l";

		loop = ( max >= 16 ) ? 4 : max / 4;

		offset = 0;

		ms->ms_Buf_Argument[0] = 0;

		for( cnt=0 ; cnt<loop ; cnt++ )
		{
			val =  (( ms->ms_MemoryBuf[ offset+0 ] << 24 )
				|   ( ms->ms_MemoryBuf[ offset+1 ] << 16 )
				|   ( ms->ms_MemoryBuf[ offset+2 ] <<  8 )
				|   ( ms->ms_MemoryBuf[ offset+3 ] <<  0 ));

			len = strlen( ms->ms_Buf_Argument );

			snprintf( & ms->ms_Buf_Argument[len], 64-len, "$%08x,", val );

			offset += 4;
		}

		size = offset;

		len = strlen( ms->ms_Buf_Argument );

		ms->ms_Buf_Argument[len-1] = 0;
	}
	else if ( max > 1 )
	{
		// 1 Word

		size = 2;

		val = (( ms->ms_MemoryBuf[ 0 ] << 8 ) | ( ms->ms_MemoryBuf[ 1 ] << 0 ));

		ms->ms_Str_Opcode = "dc.w";
	    snprintf( ms->ms_Buf_Argument, 64, "$%04x", val );
	}
	else
	{
		// 1 Byte

		size = 1;

		val = ( ms->ms_MemoryBuf[ 0 ] );

		ms->ms_Str_Opcode = "dc.b";
		snprintf( ms->ms_Buf_Argument, 64, "$%02x", val );
	}

	if ( AddSourceNode( hs, ms ))
	{
		printf( "%s:%04d: Error adding source node\n", __FILE__, __LINE__ );
		goto bailout;
	}

	*len_ptr = size;

	error = false;

bailout:

	return( error );
}

// --

static int Decode_BSS( struct HunkStruct *hs, struct M68kStruct *ms, int32_t max, int32_t *len_ptr )
{
int32_t size;
int error;

	error = true;
	size = 1 * max;

	ms->ms_Str_Opcode = "ds.b";
	snprintf( ms->ms_Buf_Argument, 64, "%d", size );

	if ( AddSourceNode( hs, ms ))
	{
		printf( "%s:%04d: Error adding source node\n", __FILE__, __LINE__ );
		goto bailout;
	}

	*len_ptr = size;

	error = false;

bailout:

	return( error );
}

// --

static int AddSourceNode( struct HunkStruct *hs, struct M68kStruct *ms )
{
struct SourceNode *sn;
char *str;
int comlen;
int error;
int len;

	error = true;

	if (( hs == NULL ) || ( ms == NULL ))
	{
		printf( "%s:%04d: NULL Pointer\n", __FILE__, __LINE__ );
		goto bailout;
	}

	if ( ms->ms_Str_Opcode == NULL )
	{
		printf( "%s:%04d: NULL Pointer\n", __FILE__, __LINE__ );
		goto bailout;
	}

	len = strlen( & ms->ms_Buf_Argument[0] ) + 1;

	if ( ms->ms_Buf_Comment )
	{
		comlen = strlen( ms->ms_Buf_Comment ) + 1;
	}
	else
	{
		comlen = 1;
	}

	len += comlen;

	sn = calloc( 1, sizeof( struct SourceNode ) + len );

	if ( sn == NULL )
	{
		printf( "%s:%04d: Error allocating memory (%d)\n", __FILE__, __LINE__, (int) sizeof( struct SourceNode ) + len );
		goto bailout;
	}

	str = (char *)( sn + 1 );

	sn->sn_Address	= ms->ms_StartAddr;
	sn->sn_Opcode	= ms->ms_Str_Opcode;
	sn->sn_Label	= ms->ms_Ptr_Label;
	sn->sn_Comment	= str;
	sn->sn_Argument = str + comlen;

	if ( ms->ms_Buf_Comment )
	{
		strcpy( sn->sn_Comment, ms->ms_Buf_Comment );
	}

	strcpy( sn->sn_Argument, ms->ms_Buf_Argument );

	myAddTail( & hs->hs_SourceList, sn );

	error = false;

bailout:

	return( error );
}

// --

static int AddEmptyNode( struct HunkStruct *hs, struct M68kStruct *ms )
{
int error;

	ms->ms_StartAddr = 0;
	ms->ms_Ptr_Label = NULL;
	ms->ms_Str_Opcode = "";
	ms->ms_Buf_Argument[0] = 0;

	error = AddSourceNode( hs, ms );

	return( error );
}

// --

static int Scan_Generate_Hunk( struct HunkStruct *hs, struct M68kStruct *ms, struct HunkNode *hn )
{
struct HunkLabel *nexthl;
struct HunkLabel *hl;
struct HunkRef *nexthr;
struct HunkRef *hr;
int32_t size;
int32_t max;
int32_t len;
int32_t pos;
uint8_t *type;
uint8_t *mem;
char *sectype;
char *memtype;
int error;
int cnt;
int mt;

	error = true;

	// --

	/**/ if ( hn->hn_HunkType == HUNK_CODE )
	{
		sectype = "CODE";
	}
	else if ( hn->hn_HunkType == HUNK_DATA )
	{
		sectype = "DATA";
	}
	else
	{
		sectype = "BSS";
	}

	/**/ if ( hn->hn_HunkMemType == 1 )
	{
		memtype = "_C";
	}
	else if ( hn->hn_HunkMemType == 2 )
	{
		memtype = "_F";
	}
	else // 0 = Public, 3 = ??
	{
		memtype = "";
	}

	if ( AddEmptyNode( hs, ms ))
	{
		printf( "%s:%04d: Error adding source node\n", __FILE__, __LINE__ );
		goto bailout;
	}

	ms->ms_StartAddr = 0;
	ms->ms_Str_Opcode = "SECTION";
	sprintf( ms->ms_Buf_Argument, "SEC%d,%s%s", hn->hn_HunkNr, sectype, memtype );

	if ( AddSourceNode( hs, ms ))
	{
		printf( "%s:%04d: Error adding source node\n", __FILE__, __LINE__ );
		goto bailout;
	}

	if ( AddEmptyNode( hs, ms ))
	{
		printf( "%s:%04d: Error adding source node\n", __FILE__, __LINE__ );
		goto bailout;
	}

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
				if ( Decode_BSS( hs, ms, max, & len ))
				{
					printf( "%s:%04d: Decoding error\n", __FILE__, __LINE__ );
					goto bailout;
				}
			}
			else if ( hn->hn_DataSize <= pos )
			{
				if ( Decode_BSS( hs, ms, max, & len ))
				{
					printf( "%s:%04d: Decoding error\n", __FILE__, __LINE__ );
					goto bailout;
				}
			}
			else
			{
				if ( Decode_Data( hs, ms, hl, hr, max, & len ))
				{
					printf( "%s:%04d: Decoding error\n", __FILE__, __LINE__ );
					goto bailout;
				}
			}
		}
		else
		{
			if (( pos ) && ( type[pos-1] != type[pos] ) && ( type[pos] == MT_Code ))
			{
				if ( AddEmptyNode( hs, ms ))
				{
					printf( "%s:%04d: Error adding source node\n", __FILE__, __LINE__ );
					goto bailout;
				}
			}

			/**/ if ( type[pos] == MT_Code )
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
					if ( Decode_Data( hs, ms, hl, hr, max, & len ))
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
		}

		#if 0
		if ( len == 0 )
		{
			if ( Decode_Data( hs, ms, hl, hr, max, & len ))
			{
				printf( "%s:%04d: Decoding error\n", __FILE__, __LINE__ );
				goto bailout;
			}
		}
		#endif

		if ( len == 0 )
		{
			printf( "%s:%04d: Decoding length error\n", __FILE__, __LINE__ );
			goto bailout;
		}


		pos += len;
	}

	// --
	// One label can be just after Memory BLock

	while(( hl ) && ( hl->hl_Label_Offset < pos ))
	{
		hl = myGetNext( hl );
	}

	if (( hl ) && ( hl->hl_Label_Offset == pos ))
	{
		ms->ms_StartAddr = 0;
		ms->ms_Ptr_Label = hl;
		ms->ms_Str_Opcode = "";
		ms->ms_Buf_Argument[0] = 0;

		if ( AddSourceNode( hs, ms ))
		{	
			printf( "%s:%04d: Error adding source node\n", __FILE__, __LINE__ );
			goto bailout;
		}
	}

	// --

	error = false;

bailout:

	return( error );
}

// --

static void Refs_Clear( struct HunkStruct *hs  )
{
struct HunkNode *hn;
struct HunkRef *hr;
int cnt;

	for( cnt=0 ; cnt<hs->hs_HunkArraySize ; cnt++ )
	{
		hn = hs->hs_HunkArray[cnt].hi_HunkNode;

		hr = myGetHead( & hn->hn_Refs );

		while( hr )
		{
			hr->hr_Used = false;

			hr = myGetNext( hr );
		}
	}
}

// --

static int Refs_Check( struct HunkStruct *hs  )
{
struct HunkNode *hn;
struct HunkRef *hr;
int header;
int error;
int cnt;
int yy;
int i;

	error = false;

	for( cnt=0 ; cnt<hs->hs_HunkArraySize ; cnt++ )
	{
		hn = hs->hs_HunkArray[cnt].hi_HunkNode;

		hr = myGetHead( & hn->hn_Refs );

		header = false;

		yy = 0;
	
		i = 0;

		while( hr )
		{
			if ( hr->hr_Used == false )
			{
				if ( header == false )
				{
					printf( "\nHunk nr #%d\n\nRef ", hn->hn_HunkNr );
					header = true;
				}

				if ( i == 8 )
				{
					printf( "\nRef " );
					i = 0;
				}

				if ( i )
				{
					printf( ", %08x", hr->hr_Address );
				}
				else
				{
					printf( "%08x", hr->hr_Address );
				}

				error = true;
				yy = true;
				i++;
			}

			hr = myGetNext( hr );
		}

		if ( yy )
		{
			printf( "\n" );
		}
	}

	return( error );
}

// --

int BuildSource( struct HunkStruct *hs )
{
struct M68kStruct ms;
struct HunkNode *hn;
char argbuf[256];
int error;
int cnt;

	error = true;

	// --

	hs->hs_PassNr = 3;

	// --

	memset( & ms, 0, sizeof( ms ));

	ms.ms_HunkStruct = hs;
	ms.ms_Buf_Argument = & argbuf[0];

	// --

	Refs_Clear( hs );

	// --

	hn = hs->hs_HunkArray[ hs->hs_HunkFirst ].hi_HunkNode;

	ms.ms_HunkNode = hn;

	if ( Scan_Generate_Hunk( hs, & ms, hn ))
	{
		printf( "%s:%04d: Error generating hunk source\n", __FILE__, __LINE__ );
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
			printf( "%s:%04d: Error generating hunk source\n", __FILE__, __LINE__ );
			goto bailout;
		}
	}

	// --

	if ( Refs_Check( hs ))
	{
		printf( "%s:%04d: Error there are unhandled pointer Refs\n", __FILE__, __LINE__ );
//		goto bailout;
	}

	// --

	error = false;

bailout:

	return( error );
}

// --
