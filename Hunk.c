
/*
 * Copyright (c) 2014-2023 by Rene W. Olsen < renewolsen @ gmail . com >
 * All rights reserved.
 *
 */

// --

#include "ReSrc4.h"
#include "MD5.h"

// --

static int DupCount = 0;

static void SetLabelName( struct HunkStruct *hs, struct HunkLabel *hl, int32_t pos, int max )
{
struct HunkLabel *cur;
struct HunkNode *hn;
char name[MAX_LabelName+16];
uint8_t *buf;
int cnt;
int len;
int c;

	// -- Get Name and add NUL

	buf = hs->hs_FileBuffer;

	for( cnt=0 ; cnt<max ; cnt++ )
	{
		c = buf[pos+cnt];

		if ( c == 0 )
		{
			break;
		}

		name[cnt] = c;
	}

	name[cnt] = 0;

	// -- Make sure the name is Unique

	len = strlen( name ) + 1;

	for( cnt=0 ; cnt<hs->hs_HunkArraySize ; cnt++ )
	{
		hn = hs->hs_HunkArray[cnt].hi_HunkNode;

		cur = myGetHead( & hn->hn_Labels );

		while( cur )
		{
			if (( cur->hl_Label_Name[0] ) && ( ! memcmp( cur->hl_Label_Name, name, len )))
			{
				break;
			}
			else
			{
				cur = myGetNext( cur );
			}
		}

		if ( cur )
		{
			break;
		}
	}

	// -- Dup?

	if ( cur )
	{
		sprintf( & name[len-1], "__%X", ++DupCount );
	}

	// -- Set Name if possible

	len = strlen( name );

	if ( len < MAX_LabelName )
	{
		memcpy( hl->hl_Label_Name, name, len+1 );
	}
	else
	{
		printf( "Skipping label name: %s\n", name );
	}

	// --
}

// --

struct HunkLabel *Hunk_AddExtLabel( struct HunkStruct *hs, int32_t addr, enum LabelType type )
{
struct HunkLabel *new;
struct HunkLabel *hl;

	hl = myGetTail( & hs->hs_ExtLabelList );

	while( hl )
	{
		if ( hl->hl_Label_Address <= addr )
		{

			break;
		}
		else
		{
			hl = myGetPrev( hl );
		}
	}

	// Is there already a label with this address?

	if (( hl ) && ( hl->hl_Label_Address == addr ))
	{
		if ( hl->hl_Label_Type == LT_Unknown )
		{
			 hl->hl_Label_Type = type;
		}

		new = hl;
		goto bailout;
	}

	// Create new label

	new = calloc( 1, sizeof( struct HunkLabel ));

	if ( new == NULL )
	{
		printf( "%s:%04d: Error allocating Memory (%d Bytes)\n", __FILE__, __LINE__, (int) sizeof( struct HunkLabel ));
		goto bailout;
	}

//	new->hl_HunkNode		= NULL;
	new->hl_Label_Type		= type;
	new->hl_Label_Address	= addr;
	new->hl_Label_Offset	= addr;

	// Insert node at correct place in the sorted list

	if ( hl )
	{
		myAddAfter( & hs->hs_ExtLabelList, hl, new );
	}
	else
	{
		myAddHead( & hs->hs_ExtLabelList, new );
	}

	// --

bailout:

	return(	new );
}

// --

struct HunkLabel *Hunk_FindLabel( struct HunkStruct *hs, int32_t addr )
{
struct HunkLabel *hl;

	int hash = addr % MAX_LABHASH;

	hl = hs->hs_LabelHash[ hash ];

	while( hl )
	{
		if ( hl->hl_Label_Address == addr )
		{
			break;
		}
		else
		{
			hl = hl->hl_HashPtr;
		}
	}

	return(	hl );
}

// --

struct HunkRef *Hunk_FindRef( struct HunkNode *hn, int32_t adr )
{
struct HunkRef *hr;

	int hash = adr % MAX_REFHASH;

	hr = hn->hn_Refs2[ hash ];

	while( hr )
	{
		if ( hr->hr_Address == adr )
		{
			break;
		}
		else
		{
			hr = hr->hr_HashPtr;
		}
	}

	return( hr );
}

// --

static void Hunk_AddRef( struct HunkNode *hn, int offset )
{
struct HunkRef *new;
struct HunkRef *hr;

// printf( "Add Hunk %03d - Ref %d\n", hn->hn_HunkNr, offset );

	new = calloc( 1, sizeof( struct HunkRef ));

	if ( new == NULL )
	{
		printf( "%s:%04d: Error Allocating Ref Label\n", __FILE__, __LINE__ );
		goto bailout;
	}

	new->hr_Address = offset + hn->hn_MemoryAdr;
	new->hr_Offset	= offset;

	// --

	hr = myGetTail( & hn->hn_Refs );

	while( hr )
	{
		if ( hr->hr_Offset <= offset )
		{
			break;
		}
		else
		{
			hr = myGetPrev( hr );
		}
	}

	if ( hr )
	{
		myAddAfter( & hn->hn_Refs, hr, new );
	}
	else
	{
		myAddHead( & hn->hn_Refs, new );
	}

	// -- Insert Hash

	int hash = new->hr_Address % MAX_REFHASH;

	new->hr_HashPtr = hn->hn_Refs2[hash];

	hn->hn_Refs2[hash] = new;

	// --

bailout:

	return;
}

// --

struct HunkNode *Hunk_FindNode( struct HunkStruct *hs, int32_t addr )
{
struct HunkNode *hn;
int32_t memadr;
int cnt;

	for( cnt=0 ; cnt<hs->hs_HunkArraySize ; cnt++ )
	{
		memadr = hs->hs_HunkArray[cnt].hi_MemoryAdr;

		if ( addr < memadr )
		{
			continue;
		}
		
		memadr += hs->hs_HunkArray[cnt].hi_MemorySize;

		if ( addr > memadr )
		{
			continue;
		}

		break;
	}

	// Hunk found or is it an address outside our code?

	if ( cnt == hs->hs_HunkArraySize )
	{
		hn = NULL;
	}
	else
	{
		hn = hs->hs_HunkArray[cnt].hi_HunkNode;
	}

	return( hn );
}

// --

static void Hunk_FreeLabel( struct HunkLabel *hl )
{
	if ( hl == NULL )
	{
		goto bailout;
	}

	free( hl );

bailout:

	return;
}

// --

struct HunkLabel *Hunk_AddLabel( struct HunkStruct *hs, int32_t addr, enum LabelType type )
{
struct HunkLabel *new;
struct HunkLabel *hl;
struct HunkNode *hn;
int32_t memadr;
int cnt;

	new = NULL;

	// First find the Hunk where the label should be

	for( cnt=0 ; cnt<hs->hs_HunkArraySize ; cnt++ )
	{
		memadr = hs->hs_HunkArray[cnt].hi_MemoryAdr;

		if ( addr < memadr )
		{
			continue;
		}

		memadr += hs->hs_HunkArray[cnt].hi_MemorySize;

		if ( addr > memadr )
		{
			continue;
		}

		break;
	}

	// Hunk found or is it an address outside our code?

	if ( cnt == hs->hs_HunkArraySize )
	{
printf( "AddLabel: External? %08x\n", addr );
		goto bailout;
	}

	// Search Label list, travling from bottom and up
	// The list is sorted so we can stop when address become smaller
	hn = hs->hs_HunkArray[cnt].hi_HunkNode;

	hl = myGetTail( & hn->hn_Labels );

	while( hl )
	{
		if ( hl->hl_Label_Address <= addr )
		{

			break;
		}
		else
		{
			hl = myGetPrev( hl );
		}
	}

	// Is there already a label with this address?

	if (( hl ) && ( hl->hl_Label_Address == addr ))
	{
		if ( hl->hl_Label_Type == LT_Unknown )
		{
			 hl->hl_Label_Type = type;
		}

		new = hl;
		goto bailout;
	}

	// Create new label

	new = calloc( 1, sizeof( struct HunkLabel ));

	if ( new == NULL )
	{
		printf( "%s:%04d: Error allocating Memory (%d Bytes)\n", __FILE__, __LINE__, (int) sizeof( struct HunkLabel ));
		goto bailout;
	}

	new->hl_HunkNode		= hn;
	new->hl_Label_Type		= type;
	new->hl_Label_Address	= addr;
	new->hl_Label_Offset	= addr - hn->hn_MemoryAdr;
	new->hl_Label_Memory	= & hn->hn_MemoryBuf[ new->hl_Label_Offset ];

	// Insert node at correct place in the sorted list

	if ( hl )
	{
		myAddAfter( & hn->hn_Labels, hl, new );
	}
	else
	{
		myAddHead( & hn->hn_Labels, new );
	}

	// -- Insert Hash

	int hash = addr % MAX_LABHASH;

	new->hl_HashPtr = hs->hs_LabelHash[hash];

	hs->hs_LabelHash[hash] = new;

	// --

bailout:

	return(	new );
}

// --
// Needed so we can add Labels outside of Hunk area

struct HunkLabel *Hunk_AddLabel2( struct HunkStruct *hs, struct HunkNode *hn, int32_t addr, enum LabelType type )
{
struct HunkLabel *new;
struct HunkLabel *hl;

	new = NULL;

	// Search Label list, travling from bottom and up
	// The list is sorted so we can stop when address become smaller
	hl = myGetTail( & hn->hn_Labels );

	while( hl )
	{
		if ( hl->hl_Label_Address <= addr )
		{
			break;
		}
		else
		{
			hl = myGetPrev( hl );
		}
	}

	// Is there already a label with this address?

	if (( hl ) && ( hl->hl_Label_Address == addr ))
	{
		if ( hl->hl_Label_Type == LT_Unknown )
		{
			 hl->hl_Label_Type = type;
		}

		new = hl;
		goto bailout;
	}

	// Create new label

	new = calloc( 1, sizeof( struct HunkLabel ));

	if ( new == NULL )
	{
		printf( "%s:%04d: Error allocating Memory (%d Bytes)\n", __FILE__, __LINE__, (int) sizeof( struct HunkLabel ));
		goto bailout;
	}

	new->hl_HunkNode		= hn;
	new->hl_Label_Type		= type;
	new->hl_Label_Address	= addr;
	new->hl_Label_Offset	= addr - hn->hn_MemoryAdr;
	new->hl_Label_Memory	= & hn->hn_MemoryBuf[ new->hl_Label_Offset ];

	// Insert node at correct place in the sorted list

	if ( hl )
	{
		myAddAfter( & hn->hn_Labels, hl, new );
	}
	else
	{
		myAddHead( & hn->hn_Labels, new );
	}

	// -- Insert Hash

	int hash = addr % MAX_LABHASH;

	new->hl_HashPtr = hs->hs_LabelHash[hash];

	hs->hs_LabelHash[hash] = new;

	// --

bailout:

	return(	new );
}

// --

static void Hunk_FreeNode( struct HunkNode *hn )
{
struct HunkLabel *ln;
struct HunkRef *rn;

	if ( hn == NULL )
	{
		goto bailout;
	}

	while(( rn = myRemHead( & hn->hn_Refs )))
	{	
		free( rn );
	}

	while(( ln = myRemHead( & hn->hn_Labels )))
	{
		Hunk_FreeLabel( ln );
	}

	if ( hn->hn_MemoryType )
	{
		free( hn->hn_MemoryType );
		hn->hn_MemoryType = NULL;
	}

	if ( hn->hn_MemoryBuf )
	{
		free( hn->hn_MemoryBuf );
		hn->hn_MemoryBuf = NULL;
	}

	free( hn );

bailout:

	return;
}

// --

static uint32_t CurrentAdr;

static struct HunkNode *Hunk_AllocNode( int hunknr, uint32_t memsize )
{
struct HunkNode *hn;
int error;

	error = true;

	hn = calloc( 1, sizeof( struct HunkNode ));

	if ( hn == NULL )
	{
		printf( "%s:%04d: Error allocating memory (%d Bytes)\n", __FILE__, __LINE__, (int) sizeof( struct HunkNode ) );
		goto bailout;
	}

	// --

	hn->hn_MemoryAdr = CurrentAdr;

	CurrentAdr += 0x00010000;
	CurrentAdr += memsize;
	CurrentAdr &= 0xffff0000;

	// --

	hn->hn_HunkNr = hunknr;

	if ( memsize )
	{
		// Added +10 for safety

		hn->hn_MemorySize	= memsize;
		hn->hn_MemoryBuf	= calloc( 1, memsize + 10 ); // BSS need to be cleared

		if ( hn->hn_MemoryBuf == NULL )
		{
			printf( "%s:%04d: Error allocating memory (%dBytes)\n", __FILE__, __LINE__, memsize );
			goto bailout;
		}

		hn->hn_MemoryType	= calloc( 1, memsize + 10 );

		if ( hn->hn_MemoryType == NULL )
		{
			printf( "%s:%04d: Error allocating memory (%dBytes)\n", __FILE__, __LINE__, memsize );
			goto bailout;
		}

		memset( hn->hn_MemoryType, MT_Unset, memsize );
	}

	error = false;

bailout:

	if (( hn ) && ( error == true ))
	{
		Hunk_FreeNode( hn );
		hn = NULL;
	}

	return(	hn );
}

// --

struct ParseStruct
{
	uint32_t	cache;
	uint32_t *	mem;
	int32_t		pos;
	int			bytes;
};

static uint32_t myReadU32( struct ParseStruct *ps )
{
uint32_t val;
uint32_t pos;

	pos = ps->pos;
	val = ps->mem[ pos ];
	ps->pos++;

	return( SWAP32( val ));
}

// --

static uint32_t myPeekU32( struct ParseStruct *ps, int offset )
{
uint32_t val;
uint32_t pos;

	pos = ps->pos;
	pos += offset;
	val = ps->mem[ pos ];

	return( SWAP32( val ));
}

// --

static uint16_t myReadU16( struct ParseStruct *ps )
{
uint16_t val;

	if ( ps->bytes == 0 )
	{
		ps->cache = SWAP32( ps->mem[ ps->pos ] );
		ps->bytes = 4;
		ps->pos++;
	}

	/**/ if ( ps->bytes == 4 )
	{
		val = ps->cache >> 16;
		ps->bytes = 2;
	}
	else if ( ps->bytes == 2 )
	{
		val = ps->cache;
		ps->bytes = 0;
	}
	else
	{
		val = 0; // Make gcc happy
	}

// printf( "ps->bytes %d : val %04x\n", ps->bytes, val );

	return( val );
}

// --

#define READ_U32()		myReadU32( & ps )
#define PEEK_U32(x)		myPeekU32( & ps, x )
#define READ_U16()		myReadU16( & ps )

static int Hunk_Parse( struct HunkStruct *hs )
{
struct ParseStruct ps;
struct HunkLabel *hl;
struct HunkNode *TargetNode;
struct HunkNode *current;
struct HunkNode *hn;
uint32_t *DestMemory;
uint32_t TargetMemory;
uint32_t tablesize;
uint32_t numreloc;
uint32_t hunktype;
uint32_t size;
uint32_t val32;
uint16_t val16;
uint32_t cnt;
int32_t offset;
int hunknum;
int error;

	// --

	error = true;

	// --

	memset( & ps, 0, sizeof( ps ));

	ps.mem = (void *) hs->hs_FileBuffer;
	ps.pos = 0;

	// --

	if ( READ_U32() != HUNK_HEADER )
	{
		printf( "This is not an Amiga Executable Hunk file\n" );
		goto bailout;
	}

	// -- Skip String(s)

	while( true )
	{
		val32 = READ_U32();

		if ( val32 == 0 )
		{
			break;
		}

		ps.pos += val32;
	}

	//------

	tablesize = READ_U32();

	hs->hs_HunkArray = calloc( 1, tablesize * sizeof( struct HunkInfo ));

	if ( hs->hs_HunkArray == NULL )
	{
		printf( "%s:%04d: Error allocating memory (%d Bytes)\n", __FILE__, __LINE__, (int) ( tablesize * sizeof( struct HunkInfo ) ));
		goto bailout;
	}

	hs->hs_HunkArraySize = tablesize;

	//------

	hs->hs_HunkFirst = READ_U32();
	hs->hs_HunkLast  = READ_U32();

	// We start at Zero to get all Hunks loaded
	// the system starts at 'hunkfirst'

	for( cnt=0 ; cnt<tablesize ; cnt++ )
	{
		val32 = READ_U32();
		size = (( val32 & 0x7fffffff ) << 2 );

		hn = Hunk_AllocNode( cnt, size );

		if ( hn == NULL )
		{
			goto bailout;
		}

		hn->hn_HunkMemType	= ( val32 >> 30 );
		hs->hs_HunkArray[cnt].hi_HunkNode	= hn;
		hs->hs_HunkArray[cnt].hi_MemoryAdr	= hn->hn_MemoryAdr;
		hs->hs_HunkArray[cnt].hi_MemorySize	= size;
	}

	//--------

	current = NULL;

	cnt = 0;

	hn = hs->hs_HunkArray[cnt].hi_HunkNode;

	while( hs->hs_FileSize > ps.pos * 4 )
	{
		val32 = PEEK_U32(0);
		hunktype = val32 & 0x3fffffff;

// printf( "Pos : %08x : Type %08x\n", ps.pos*4 , val32 );

		switch( hunktype )
		{
			case HUNK_CODE:
			case HUNK_DATA:
			{
//	printf( "Code/Data hunk\n" );

				if ( current )
				{
					printf( "\nHunk structure sanity check failure\nMissing HUNK_END\n" );
					goto bailout;
				}

				current = hn;

				val32 = PEEK_U32(1);
				size = val32;

				hn->hn_FileOffset = ( ps.pos * 4 ) + 8;
				hn->hn_HunkType = hunktype;
				hn->hn_DataSize = size * 4;

				#if 0
				// Print info if a Code/Data hunk also have a BSS section at the end
				if ( hn->hn_DataSize != hn->hn_MemorySize )
				{
					printf( "Hunk %d Size Diff : Data %d, Mem %d\n", hn->hn_HunkNr, hn->hn_DataSize, hn->hn_MemorySize );
				}
				#endif

				if ( hn->hn_DataSize > hn->hn_MemorySize )
				{
					printf( "\nHunk structure sanity check failure\nHunk Data size bigger that memory size\n" );
					goto bailout;
				}

				if ( size )
				{
					memcpy(
						hn->hn_MemoryBuf,
						& hs->hs_FileBuffer[ ( ps.pos * 4 ) + 8 ],
						size * 4
					);
				}

				ps.pos += size + 2;
				break;
			}

			case HUNK_BSS:
			{
//	printf( "BSS hunk\n" );

				if ( current )
				{
					printf( "\nHunk structure sanity check failure\nMissing HUNK_END\n" );
					goto bailout;
				}

				current = hn;

				hn->hn_FileOffset = ( ps.pos * 4 ) + 8;
				hn->hn_HunkType = hunktype;

				ps.pos += 2;
				break;
			}

			case HUNK_RELOC32:
			{
//	printf( "HUNK_RELOC32\n" );

				if ( current == NULL )
				{
					printf( "\nHunk structure sanity check failure\nUnexpected HUNK_RELOC32\n" );
					goto bailout;
				}

				ps.pos++;
				
				while( true )
				{
					numreloc = READ_U32();
				
					if ( numreloc == 0 )
					{
						break;
					}

					hunknum = READ_U32();

					if ( hunknum >= hs->hs_HunkArraySize )
					{
						printf( "\nHunk Numner is illegal\n\nHunk Nr %d is out of range\nFile only has %d hunks", hunknum, hs->hs_HunkArraySize );
						goto bailout;
					}

					TargetMemory = hs->hs_HunkArray[ hunknum ].hi_MemoryAdr;
					TargetNode	 = hs->hs_HunkArray[ hunknum ].hi_HunkNode;

					while( numreloc-- )
					{
						offset = READ_U32() & 0x7fffffff;

						if ( current->hn_MemorySize < offset )
						{
							printf( "\nHunk reloc offset is illegal\n\nOffset %d is out of range\nHunk %d is only %d bytes big", offset, hunknum, hs->hs_HunkArray[ hunknum ].hi_MemorySize );
							goto bailout;
						}

						DestMemory	= (uint32_t *) & current->hn_MemoryBuf[ offset ];
						val32		= *DestMemory;
						val32		= SWAP32( val32 );
						val32	   += TargetMemory;
						*DestMemory	= SWAP32( val32 );

						// This is where the it points to, we need a public label

						hl = Hunk_AddLabel2( hs, TargetNode, val32, LT_Unknown );

						if ( hl == NULL )
						{
							printf( "%s:%04d: Error creating label\n", __FILE__, __LINE__ );
							goto bailout;
						}

						// This is where the code/data points from
						// We need the info, when dealing with Data areas
						Hunk_AddRef( hn, offset );
					}
				}
				break;
			}

			case HUNK_DREL32:
			case HUNK_RELOC32SHORT:
			{
				if ( current == NULL )
				{
					printf( "\nHunk structure sanity check failure\nUnexpected HUNK_DREL32\n" );
					goto bailout;
				}

				ps.bytes = 0;
				ps.pos++;

				while( true )
				{
					numreloc = READ_U16();

					if ( numreloc == 0 )
					{
						break;
					}

					hunknum = READ_U16();

					if ( hunknum >= hs->hs_HunkArraySize )
					{
						printf( "\nHunk Numner is illegal\n\nHunk Nr %d is out of range\nFile only has %d hunks", hunknum, hs->hs_HunkArraySize );
						goto bailout;
					}

					TargetMemory = hs->hs_HunkArray[ hunknum ].hi_MemoryAdr;
					TargetNode	 = hs->hs_HunkArray[ hunknum ].hi_HunkNode;

					while( numreloc-- )
					{
						val16 = READ_U16();
						offset = val16 & 0x7fffffff;

						if ( current->hn_MemorySize < offset )
						{
							printf( "\nHunk reloc offset is illegal\n\nOffset %d is out of range\nHunk %d is only %d bytes big", offset, hunknum, hs->hs_HunkArray[ hunknum ].hi_MemorySize );
							goto bailout;
						}

						DestMemory	= (uint32_t *) & current->hn_MemoryBuf[ offset ];
						val32		= *DestMemory;
						val32		= SWAP32( val32 );
						val32	   += TargetMemory;
						*DestMemory	= SWAP32( val32 );

						// This is where the it points to, we need a public label

						hl = Hunk_AddLabel2( hs, TargetNode, val32, LT_Unknown );

						if ( hl == NULL )
						{
							printf( "%s:%04d: Error creating label\n", __FILE__, __LINE__ );
							goto bailout;
						}

						// This is where the code/data points from
						// We need the info, when dealing with Data areas
						Hunk_AddRef( hn, offset );
					}
				}
				break;
			}

			case HUNK_END:
			{
//	printf( "END hunk\n" );

				if ( current == NULL )
				{
					printf( "\nHunk structure sanity check failure\nUnexpected HUNK_END\n" );
					goto bailout;
				}

				cnt += 1;
				ps.pos += 1;

				current = NULL;

				hn = hs->hs_HunkArray[cnt].hi_HunkNode;
				break;
			}

			case HUNK_DEBUG:
			{
				val32 = PEEK_U32(1);
				ps.pos += val32+2;
				break;
			}

			case HUNK_SYMBOL:
			{
				if ( current == NULL )
				{
					printf( "\nHunk structure sanity check failure\nUnexpected HUNK_SYMBOL\n" );
					goto bailout;
				}

				ps.pos++;

				while( true )
				{
					val32 = READ_U32();		// Name Length

					if ( val32 == 0 )
					{
						break;
					}

					if ( val32 < ( MAX_LabelName / 4 ))
					{
						offset = PEEK_U32( val32 );
						offset += current->hn_MemoryAdr;

						hl = Hunk_AddLabel2( hs, current, offset, LT_Unknown );

						if ( hl == NULL )
						{
							printf( "%s:%04d: Error creating label\n", __FILE__, __LINE__ );
							goto bailout;
						}

						SetLabelName( hs, hl, ps.pos*4, val32*4 );
					}

					ps.pos += val32;	// Skip Name
					ps.pos += 1;		// Skip Offset
				}
				break;
			}

			default:
			{
				printf( "\nUnsupported Hunk Type 0x%08X\n", hunktype );
				goto bailout;
			}
		}
	}

	error = false;

bailout:

	return( error );
}

// --

void Hunk_FreeData( struct HunkStruct *hs )
{
struct HunkLabel *hl;
void *node;

	if ( hs == NULL )
	{
		goto bailout;
	}

	if ( hs->hs_FileBuffer )
	{
		free( hs->hs_FileBuffer );
		hs->hs_FileBuffer = NULL;
	}

	if ( hs->hs_HunkArray )
	{
		for( int cnt=0 ; cnt<hs->hs_HunkArraySize ; cnt++ )
		{
			Hunk_FreeNode( hs->hs_HunkArray[cnt].hi_HunkNode );
			hs->hs_HunkArray[cnt].hi_HunkNode = NULL;
		}

		free( hs->hs_HunkArray );
		hs->hs_HunkArray = NULL;
	}

	while(( node = myRemHead( & hs->hs_SourceList )))
	{
		free( node );
	}

	while(( hl = myRemHead( & hs->hs_ExtLabelList )))
	{
		Hunk_FreeLabel( hl );
	}

	free( hs );

bailout:

	return;
}

// --

static char *myFindFileName( char *name )
{
char *str;
int pos;

	pos = 0;
	str = name;

	while( name[pos] )
	{
		if ( name[pos+1] == 0 )
		{
			break;
		}

		if (( name[pos] == '/' )
		||	( name[pos] == ':' )
		||	( name[pos] == '\\' ))
		{
			str = & name[pos+1];
		}

		pos++;
	}

	return( str );
}

// --

struct HunkStruct *Hunk_LoadFile( char *filename )
{
struct HunkStruct *hs;
MD5Context ctx;
uint32_t *buffer;
size_t fsize;
size_t size;
FILE *file;
int error;
int cnt;

	CurrentAdr = 0x00010000;

	error = true;

	file = NULL;

	hs = calloc( 1, sizeof( struct HunkStruct ));

	if ( hs == NULL )
	{
		printf( "%s:%04d: Error allocating memory (%d Bytes)\n", __FILE__, __LINE__, (int) sizeof( struct HunkStruct ));
		goto bailout;
	}

	hs->hs_FileName = myFindFileName( filename );

	file = fopen( filename, "rb" );

	if ( file == NULL )
	{
		printf( "Error opening file (%s)\n", filename );
		goto bailout;
	}

	fseek( file, 0, SEEK_END );

	size = ftell( file );

	fseek( file, 0, SEEK_SET );

	hs->hs_FileSize = size;

	buffer = calloc( 1, size + 1 );

	if ( buffer == NULL )
	{
		printf( "%s:%04d: Error allocating memory (%d Bytes)\n", __FILE__, __LINE__, (int) size );
		goto bailout;
	}

	hs->hs_FileBuffer = (void *) buffer;

	fsize = fread( buffer, 1, size, file );

	if ( fsize != size )
	{
		printf( "Error reading file\n" );
		goto bailout;
	}

	printf( "Loaded %s (%d Bytes)\n", filename, (int) size );

	// --

	if ( Hunk_Parse( hs ))
	{
		// Error is printed
		goto bailout;
	}

	// --

	md5Init( & ctx );

	md5Update( & ctx, hs->hs_FileBuffer, size );

	md5Finalize( & ctx );

	for( cnt=0 ; cnt<16 ; cnt++ )
	{
		sprintf( & hs->hs_FileMD5[ cnt*2 ], "%02x", ctx.digest[cnt] );
	}

	// --

#if 0

// Check Labels have been inserted in correct order

{
struct HunkLabel *hl;
struct HunkNode *hn;
int cnt;
int i;

	for( cnt=0 ; cnt<hs->hs_HunkArraySize ; cnt++ )
	{
		printf( "\n# Hunk %d\n\n", cnt );

		hn = hs->hs_HunkArray[cnt].hi_HunkNode;

		hl = myGetHead( & hn->hn_Labels );

		i = 1;

		while( hl )
		{
			printf( "%5d : %08x %08x\n", i++, hl->hl_Label_Address, hl->hl_Label_Offset );

			hl = myGetNext( hl );
		}
	}
}
#endif

#if 0

// Check the Spread in Hash

{
struct HunkLabel *hl;
int lcnt;
int ii;

	for( ii=0 ; ii<MAX_LABHASH ; ii++ )
	{
		lcnt = 0;

		hl = hs->hs_LabelHash[ii];

		while( hl )
		{
			lcnt++;
			hl = hl->hl_HashPtr;
		}

		printf( "%3d : Labels %d\n", ii, lcnt );
	}
}
#endif

	error = false;

bailout:

	if (( error ) && ( hs ))
	{
		Hunk_FreeData( hs );
		hs = NULL;
	}

	if ( file )
	{
		fclose( file );
	}

	return( hs );
}

// --
