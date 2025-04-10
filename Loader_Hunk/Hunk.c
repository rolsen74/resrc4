
/*
 * Copyright (c) 2014-2025 Rene W. Olsen < renewolsen @ gmail . com >
 * All rights reserved.
 *
 */

// --

#include "Resourcer/ReSrc4.h"
#include "Resourcer/MD5.h"

// --

#define DODEBUG(x)

// --
// -- Intern Struct

struct HunkParseStruct
{
	uint32_t *	mem;
	int32_t		pos;
	int			bytes;
	uint32_t	cache;
};

// --

enum RS4FuncStat RS4BuildLabelString( enum RS4ErrorCode *errcode, RS4Label *rl, char *buf )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Label *parent;
int off;

	ec = RS4ErrStat_Error;
	fs = RS4FuncStat_Error;

	#ifdef DEBUG
	if ( ! rl )
	{
		printf( "%s:%04d: NULL Pointer\n", __FILE__, __LINE__ );
		goto bailout;
	}
	#endif

	parent = rl->rl_Parent;

	if ( parent )
	{
		off = rl->rl_Offset - parent->rl_Offset;

		if ( off < 0 )
		{
			sprintf( buf, "%s%d", parent->rl_Name, off );
		}
		else
		{
			sprintf( buf, "%s+%d", parent->rl_Name, off );
		}
	}
	else
	{
		sprintf( buf, "%s", rl->rl_Name );
	}

	// --

	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

	// --

	#ifdef DEBUG
bailout:
	#endif

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fs );
}

// --

void BuildLabelString( RS4Label *rl, char *buf )
{
RS4Label *parent;
int off;

	parent = rl->rl_Parent;

	if ( parent )
	{
		off = rl->rl_Offset - parent->rl_Offset;

		if ( off < 0 )
		{
			sprintf( buf, "%s%d", parent->rl_Name, off );
		}
		else
		{
			sprintf( buf, "%s+%d", parent->rl_Name, off );
		}
	}
	else
	{
		sprintf( buf, "%s", rl->rl_Name );
	}
}

// --

enum RS4FuncStat RS4BuildLabelString2( enum RS4ErrorCode *errcode, RS4Trace *rt, char *buf, int64_t adr, int64_t val )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Label *rl;
RS4Ref *isRef;

	ec	= RS4ErrStat_Error;
	fs	= RS4FuncStat_Error;

	isRef = RS4FindRef_Sec( rt->rt_Section, adr );

	if ( isRef )
	{
		isRef->rr_Handled = TRUE;

		// if there is a Ref then the a label have been added
		rl = RS4FindLabel_File( rt->rt_File, val );

		if ( ! rl )
		{
			ec = RS4ErrStat_Internal;

			#ifdef DEBUG
			printf( "%s:%04d: Error finding label at $%08" PRIx64 "\n", __FILE__, __LINE__, adr );
			#endif

			goto bailout;
		}

		if ( rl->rl_Name[0] )
		{
			fs = RS4BuildLabelString( & ec, rl, buf );

			if ( fs != RS4FuncStat_Okay )
			{
				// ec allready set

				#ifdef DEBUG
				printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
				#endif

				goto bailout;
			}
		}
		else
		{
			// Not an external as it has a Refs 
			sprintf( buf, "$%08" PRIx64 "", val );
		}
	}
	else
	{
		sprintf( buf, "$%08" PRIx64 "", val );
	}

	// --

	fs	= RS4FuncStat_Okay;
	ec	= RS4ErrStat_Okay;

	// --

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fs );
}

// --

static int DupCount = 0;

static enum RS4FuncStat RS4SetLabelName( enum RS4ErrorCode *errcode, RS4FileHeader *fh, RS4Label *rl, int32_t pos, int max )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4FileSection *sec;
RS4Label *cur;
char name[MAX_LabelName+16];
uint8_t *buf;
int cnt;
int len;
int c;

	// -- Get Name and add NUL

	ec	= RS4ErrStat_Error;
	fs	= RS4FuncStat_Error;
	buf = fh->rfh_FileBuffer;

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

	for( cnt=0 ; cnt<fh->rfh_SecArraySize ; cnt++ )
	{
		sec = fh->rfh_SecArray[cnt].rsi_Section;
		cur = RS4GetHead( & sec->rfs_SecLabels );

		while( cur )
		{
			if (( cur->rl_Name[0] ) && ( ! memcmp( cur->rl_Name, name, len )))
			{
				break;
			}
			else
			{
				cur = RS4GetNext( cur );
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
		memcpy( rl->rl_Name, name, len+1 );
	}
	else
	{
		printf( "Skipping label name: %s\n", name );
	}

	// --

	fs	= RS4FuncStat_Okay;
	ec	= RS4ErrStat_Okay;

	// --

//bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fs );
}

// --

RS4Label *RS4AddExtLabel( enum RS4ErrorCode *errcode, RS4FileHeader *fh, int64_t addr )
{
enum RS4ErrorCode ec;
RS4Label *new;
RS4Label *rl;

	ec = RS4ErrStat_Error;
	rl = RS4GetTail( & fh->rfh_ExtLabelList );

	while( rl )
	{
		if ( rl->rl_Address <= addr )
		{

			break;
		}
		else
		{
			rl = RS4GetPrev( rl );
		}
	}

	// Is there already a label with this address?

	if (( rl ) && ( rl->rl_Address == addr ))
	{
		new = rl;
		goto bailout;
	}

	// Create new label

	new = calloc( 1, sizeof( RS4Label ));

	if ( ! new )
	{
		ec	= RS4ErrStat_OutOfMemory;

		#ifdef DEBUG
		printf( "%s:%04d: Error allocating Memory (%d Bytes)\n", __FILE__, __LINE__, (int) sizeof( RS4Label ));
		#endif

		goto bailout;
	}

//	new->hl_HunkNode	= NULL;
//	new->rl_Type		= type;
	new->rl_Address		= addr;
	new->rl_Offset		= addr;

	// Insert node at correct place in the sorted list

	if ( rl )
	{
		RS4AddAfter( & fh->rfh_ExtLabelList, rl, new );
	}
	else
	{
		RS4AddHead( & fh->rfh_ExtLabelList, new );
	}

	// --

	ec	= RS4ErrStat_Okay;

	// --

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return(	new );
}

// --

static uint32_t myReadU32( struct HunkParseStruct *ps )
{
uint32_t val;
uint32_t pos;

	pos = ps->pos;
	val = ps->mem[ pos ];
	ps->pos++;

	return( SWAP32( val ));
}

// --

static uint32_t myPeekU32( struct HunkParseStruct *hps, int offset )
{
uint32_t val;
uint32_t pos;

	pos = hps->pos;
	pos += offset;
	val = hps->mem[ pos ];

	return( SWAP32( val ));
}

// --

static uint16_t myReadU16( struct HunkParseStruct *hps )
{
uint16_t val;

	if ( hps->bytes == 0 )
	{
		hps->cache = SWAP32( hps->mem[ hps->pos ] );
		hps->bytes = 4;
		hps->pos++;
	}

	/**/ if ( hps->bytes == 4 )
	{
		val = hps->cache >> 16;
		hps->bytes = 2;
	}
	else if ( hps->bytes == 2 )
	{
		val = hps->cache;
		hps->bytes = 0;
	}
	else
	{
		val = 0; // Make gcc happy
	}

// printf( "ps->bytes %d : val %04x\n", ps->bytes, val );

	return( val );
}

// --

static uint32_t Hunk_KnownHunk( uint32_t type )
{
	switch( type )
	{
		case HUNK_CODE:				// 0x000003E9
		case HUNK_DATA:				// 0x000003EA
		case HUNK_BSS:				// 0x000003EB
		case HUNK_RELOC32:			// 0x000003EC
		case HUNK_RELOC16:			// 0x000003ED
		case HUNK_RELOC8:			// 0x000003EE
		case HUNK_SYMBOL:			// 0x000003F0
		case HUNK_DEBUG:			// 0x000003F1
		case HUNK_END:				// 0x000003F2
		case HUNK_HEADER:			// 0x000003F3
		case HUNK_DREL32:			// 0x000003F7
		case HUNK_RELOC32SHORT:		// 0x000003FC
		case HUNK_RELRELOC32:		// 0x000003FD
		{
			return( type );
		}

		default:
		{
			return( 0 );
		}
	}
}

// --

#define READ_U32()		myReadU32( & hps )
#define PEEK_U32(x)		myPeekU32( & hps, x )
#define READ_U16()		myReadU16( & hps )

enum RS4FuncStat Hunk_ParseFile( enum RS4ErrorCode *errcode, RS4FileHeader *fh )
{
struct HunkParseStruct hps;
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4FileSection *TargetSec;
RS4FileSection *current;
RS4FileSection *sec;
RS4Label *rl;
RS4Ref *rr;
uint32_t *DestMemory;
uint32_t TargetAdr;
uint32_t numreloc;
uint32_t hunktype;
uint32_t val32;
uint32_t size;
uint16_t val16;
int64_t offset;
int sections;
int hunknum;
int relcnt;
int first;
int cnt;

	fs = RS4FuncStat_Error;
	ec = RS4ErrStat_Error;

	// --

	memset( & hps, 0, sizeof( hps ));

	hps.mem = (void *) fh->rfh_FileBuffer;

	// --

	if ( READ_U32() != HUNK_HEADER )
	{
		printf( "This is not an Amiga Executable Hunk file\n" );
		goto bailout;
	}

	// -- Skip String(s)

	while( TRUE )
	{
		val32 = READ_U32();

		if ( ! val32 )
		{
			break;
		}

		hps.pos += val32;
	}

	// --

	sections = READ_U32();

	fh->rfh_SecArray = calloc( 1, sections * sizeof( RS4FileSecInfo ));

	if ( ! fh->rfh_SecArray )
	{
		ec = RS4ErrStat_OutOfMemory;

		#ifdef DEBUG
		printf( "%s:%04d: Error allocating memory (%d Bytes)\n", __FILE__, __LINE__, (int) ( sections * sizeof( RS4FileSecInfo ) ));
		#endif

		goto bailout;
	}

	fh->rfh_SecArraySize = sections;

	// --

	first = READ_U32();
	fh->rfh_HunkData.hfh_HunkFirst = first;
	fh->rfh_HunkData.hfh_HunkLast  = READ_U32();
	fh->rfh_SecFirst = first;

	// --

	// We start at Zero to get all Hunks loaded
	// the system starts at 'hunkfirst'
	for( cnt=0 ; cnt<sections ; cnt++ )
	{
		val32 = READ_U32();
		size = (( val32 & 0x7fffffff ) << 2 );

		sec = RS4AllocSection( & ec, cnt, size );

		if ( ! sec )
		{
			// ec allready set

			#ifdef DEBUG
			printf( "%s:%04d: Error allocating memory (%d Bytes)\n", __FILE__, __LINE__, (int) ( sections * sizeof( RS4FileSection ) ));
			#endif

			goto bailout;
		}

		sec->rfs_File = fh;

		switch( val32 >> 30 )
		{
			case 0:
			{
				sec->rfs_HunkData.hfs_MemoryType = HUNK_MEMTYPE_Public;
				break;
			}

			case 1:
			{
				sec->rfs_HunkData.hfs_MemoryType = HUNK_MEMTYPE_Chip;
				break;
			}

			case 2:
			{
				sec->rfs_HunkData.hfs_MemoryType = HUNK_MEMTYPE_Fast;
				break;
			}

			default:
			{
				// should I quit here?
				sec->rfs_HunkData.hfs_MemoryType = HUNK_MEMTYPE_Unknown;
				break;
			}
		}

		fh->rfh_SecArray[cnt].rsi_Section		= sec;
		fh->rfh_SecArray[cnt].rsi_MemoryAdr		= sec->rfs_MemoryAdr;
		fh->rfh_SecArray[cnt].rsi_MemorySize	= size;
	}

	// --

	fh->rfh_StartAdr = fh->rfh_SecArray[first].rsi_MemoryAdr;

	// --

	cnt = 0;
	sec = fh->rfh_SecArray[cnt].rsi_Section;

	current = NULL;

	while( fh->rfh_FileSize > hps.pos * 4 )
	{
		val32 = PEEK_U32(0);
		hunktype = val32 & 0x3fffffff;

		switch( hunktype )
		{
			case HUNK_CODE:
			case HUNK_DATA:
			{
				if ( DoVerbose > 1 )
				{
					printf( "\nHunk Number ....... : #%d\n", cnt );

					if ( hunktype == HUNK_CODE )
					{
						printf( "Hunk ID ........... : [$%04X] HUNK_CODE\n", hunktype );
					}
					else
					{
						printf( "Hunk ID ........... : [$%04X] HUNK_DATA\n", hunktype );
					}

					if ( DoVerbose > 2 )
					{
						printf( "File Offset ....... : $%08x\n", hps.pos*4 );
					}
				}

				if ( current )
				{
					ec = RS4ErrStat_FileStructure;
					printf( "\nHunk structure sanity check failure\nMissing HUNK_END\n" );
					goto bailout;
				}

				current = sec;

				val32 = PEEK_U32(1);
				size = val32;

				if ( DoVerbose > 2 )
				{
					printf( "Hunk Size ......... : $%08x (%d)\n", size*4, size*4 );
				}

				sec->rfs_DataSize	= size * 4;
				sec->rfs_FileOffset	= ( hps.pos * 4 ) + 8;
				sec->rfs_SecType	= ( hunktype == HUNK_CODE ) ? RS4ST_Code : RS4ST_Data ;

				if ( sec->rfs_DataSize > sec->rfs_MemorySize )
				{
					ec = RS4ErrStat_FileStructure;
					printf( "\nHunk structure sanity check failure\nHunk Data size bigger that memory size\n" );
					goto bailout;
				}

				if ( size )
				{
					memcpy(
						sec->rfs_MemoryBuf,
						& fh->rfh_FileBuffer[ ( hps.pos * 4 ) + 8 ],
						size * 4
					);
				}

				hps.pos += size + 2;
				break;
			}

			case HUNK_BSS:
			{
				if ( DoVerbose > 1 )
				{
					printf( "\nHunk Number ....... : #%d\n", cnt );
					printf( "Hunk ID ........... : [$%04X] HUNK_BSS\n", hunktype );

					if ( DoVerbose > 2 )
					{
						printf( "File Offset ....... : $%08x\n", hps.pos*4 );
					}
				}

				if ( current )
				{
					ec = RS4ErrStat_FileStructure;
					printf( "\nHunk structure sanity check failure\nMissing HUNK_END\n" );
					goto bailout;
				}

				current = sec;

				sec->rfs_DataSize	= 0;
				sec->rfs_FileOffset	= ( hps.pos * 4 ) + 8;
				sec->rfs_SecType	= RS4ST_BSS;

				hps.pos += 2;
				break;
			}

			case HUNK_RELOC32:
			{
				if ( DoVerbose > 1 )
				{
					printf( "Hunk ID ........... : [$%04X] HUNK_RELOC32\n", hunktype );

					if ( DoVerbose > 2 )
					{
						printf( "File Offset ....... : $%08x\n", hps.pos*4 );
					}
				}

				if ( ! current )
				{
					ec = RS4ErrStat_FileStructure;
					printf( "\nHunk structure sanity check failure\nUnexpected HUNK_RELOC32\n" );
					goto bailout;
				}

				hps.pos++;
				
				while( TRUE )
				{
					numreloc = READ_U32();

					if ( DoVerbose > 2 )
					{
						printf( " Relocs ........... : #%d\n", numreloc );
					}

					if ( ! numreloc )
					{
						break;
					}

					hunknum = READ_U32();

					if ( DoVerbose > 2 )
					{
						printf( " Reloc Target ..... : #%d\n", hunknum );
					}

					if ( hunknum >= sections )
					{
						ec = RS4ErrStat_FileStructure;
						printf( "\nHunk Numner is illegal\n\nHunk Nr %d is out of range\nFile only has %d hunks", hunknum, sections );
						goto bailout;
					}

					relcnt = 1;
					TargetAdr = fh->rfh_SecArray[ hunknum ].rsi_MemoryAdr;
					TargetSec = fh->rfh_SecArray[ hunknum ].rsi_Section;

					while( numreloc-- )
					{
						offset = READ_U32(); // & 0x7fffffff;

						if ( DoVerbose > 2 )
						{
							printf( " Reloc Offset .1... : $%08" PRIx64 " #%d\n", offset, relcnt++ );
						}

						if ( current->rfs_MemorySize < offset )
						{
							ec = RS4ErrStat_FileStructure;
							printf( "\nHunk reloc offset is illegal\n\nOffset %" PRId64 " is out of range\nHunk %d is only %" PRId64 " bytes big", offset, hunknum, fh->rfh_SecArray[ hunknum ].rsi_MemorySize );
							goto bailout;
						}

						DestMemory	= (uint32_t *) & current->rfs_MemoryBuf[ offset ];
						val32		= *DestMemory;
						val32		= SWAP32( val32 );
						val32	   += TargetAdr;
						*DestMemory	= SWAP32( val32 );

						// This is where the it points to, we need a public label
						rl = RS4AddLabel_Sec( & ec, TargetSec, val32, RS4LabelType_Unset );

						if ( ! rl )
						{
							// ec allready set
							fs = RS4FuncStat_Error;
							ec = RS4ErrStat_Error;

							#ifdef DEBUG
							printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
							#endif

							goto bailout;
						}

						// This is where the code/data points from
						// We need the info, when dealing with Data areas
						rr = RS4AddRef_Sec( & ec, current, offset );

						if ( ! rr )
						{
							fs = RS4FuncStat_Error;
							// ec allready set

							#ifdef DEBUG
							printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
							#endif

							goto bailout;
						}
					}
				}
				break;
			}

			case HUNK_END:
			{
				if ( DoVerbose > 1 )
				{
					printf( "Hunk ID ........... : [$%04X] HUNK_END\n", hunktype );

					if ( DoVerbose > 2 )
					{
						printf( "File Offset ....... : $%08x\n", hps.pos*4 );
					}
				}

				if ( ! current )
				{
					// I found a very old Amiga Hunk File that
					// has two HUNK_END in a row. Lets peek ahead
					// and see if there are something we recognize

					val32 = PEEK_U32(1);

					hunktype = Hunk_KnownHunk( val32 );

					if (( ! hunktype ) || ( hunktype == HUNK_HEADER ))
					{
						ec = RS4ErrStat_FileStructure;
						printf( "\nHunk structure sanity check failure\nUnexpected HUNK_END\n" );
						goto bailout;
					}
				}
				else
				{
					cnt += 1;
					current = NULL;
					sec = fh->rfh_SecArray[cnt].rsi_Section;
				}

				hps.pos += 1;
				break;
			}

			case HUNK_DEBUG:
			{
				if ( DoVerbose > 1 )
				{
					printf( "Hunk ID ........... : [$%04X] HUNK_DEBUG\n", hunktype );

					if ( DoVerbose > 2 )
					{
						printf( "File Offset ....... : $%08x\n", hps.pos*4 );
					}
				}

				val32 = PEEK_U32(1);
				hps.pos += val32+2;

				if ( DoVerbose > 2 )
				{
					printf( "Hunk Size ......... : %d\n", val32*4 );
				}
				break;
			}

			case HUNK_SYMBOL:
			{
				if ( DoVerbose > 1 )
				{
					printf( "Hunk ID ........... : [$%04X] HUNK_SYMBOL\n", hunktype );

					if ( DoVerbose > 2 )
					{
						printf( "File Offset ....... : $%08x\n", hps.pos*4 );
					}
				}

				if ( ! current )
				{
					ec = RS4ErrStat_FileStructure;
					printf( "\nHunk structure sanity check failure\nUnexpected HUNK_SYMBOL\n" );
					goto bailout;
				}

				hps.pos++;

				while( TRUE )
				{
					val32 = READ_U32();		// Name Length

					if ( ! val32 )
					{
						break;
					}

					if ( val32 < ( MAX_LabelName / 4 ))
					{
						offset = PEEK_U32( val32 );
						offset += current->rfs_MemoryAdr;

						// Create Label in current Section (and set name right after)
						rl = RS4AddLabel_Sec( & ec, current, offset, RS4LabelType_Unset );

						if ( ! rl )
						{
							// ec allready set
							fs = RS4FuncStat_Error;
							ec = RS4ErrStat_Error;

							#ifdef DEBUG
							printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
							#endif

							goto bailout;
						}

						fs = RS4SetLabelName( & ec, fh, rl, hps.pos*4, val32*4 );

						if ( fs != RS4FuncStat_Okay )
						{
							// ec allready set

							#ifdef DEBUG
							printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
							#endif

							goto bailout;
						}
					}

					hps.pos += val32;	// Skip Name
					hps.pos += 1;		// Skip Offset
				}
				break;
			}

			case HUNK_DREL32:
			case HUNK_RELOC32SHORT:
			{
				if ( DoVerbose > 1 )
				{
					if ( hunktype == HUNK_DREL32 )
					{
						printf( "Hunk ID ........... : [$%04X] HUNK_DREL32\n", hunktype );
					}
					else
					{
						printf( "Hunk ID ........... : [$%04X] HUNK_RELOC32SHORT\n", hunktype );
					}

					if ( DoVerbose > 2 )
					{
						printf( "File Offset ....... : $%08x\n", hps.pos*4 );
					}
				}

				if ( ! current )
				{
					ec = RS4ErrStat_FileStructure;
					printf( "\nHunk structure sanity check failure\nUnexpected HUNK_DREL32\n" );
					goto bailout;
				}

				hps.bytes = 0;
				hps.pos++;

				while( TRUE )
				{
					numreloc = READ_U16();

					if ( DoVerbose > 2 )
					{
						printf( " Relocs ........... : #%d\n", numreloc );
					}

					if ( ! numreloc )
					{
						break;
					}

					hunknum = READ_U16();

					if ( DoVerbose > 2 )
					{
						printf( " Reloc Target ..... : #%d\n", hunknum );
					}

					if ( hunknum >= sections )
					{
						ec = RS4ErrStat_FileStructure;
						printf( "\nHunk Numner is illegal\n\nHunk Nr %d is out of range\nFile only has %d hunks", hunknum, sections );
						goto bailout;
					}

					relcnt = 1;	
					TargetAdr	= fh->rfh_SecArray[ hunknum ].rsi_MemoryAdr;
					TargetSec	= fh->rfh_SecArray[ hunknum ].rsi_Section;

					while( numreloc-- )
					{
						val16 = READ_U16();
						offset = val16; // & 0x7fff;

						if ( DoVerbose > 2 )
						{
							printf( " Reloc Offset .2... : $%08" PRIx64 " #%d\n", offset, relcnt++ );
						}

						if ( current->rfs_MemorySize < offset )
						{
							ec = RS4ErrStat_FileStructure;
							printf( "\nHunk reloc offset is illegal\n\nOffset %" PRId64 " is out of range\nHunk %d is only %" PRId64 " bytes big", offset, hunknum, fh->rfh_SecArray[ hunknum ].rsi_MemorySize );
							goto bailout;
						}

						DestMemory	= (uint32_t *) & current->rfs_MemoryBuf[ offset ];
						val32		= *DestMemory;
						val32		= SWAP32( val32 );
						val32	   += TargetAdr;
						*DestMemory	= SWAP32( val32 );

						// This is where the it points to, we need a public label
						rl = RS4AddLabel_Sec( & ec, TargetSec, val32, RS4LabelType_Unset );

						if ( ! rl )
						{
							// ec allready set
							fs = RS4FuncStat_Error;
							ec = RS4ErrStat_Error;

							#ifdef DEBUG
							printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
							#endif

							goto bailout;
						}

						// This is where the code/data points from
						// We need the info, when dealing with Data areas
						rr = RS4AddRef_Sec( & ec, current, offset );

						if ( ! rr )
						{
							fs = RS4FuncStat_Error;
							// ec allready set

							#ifdef DEBUG
							printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
							#endif

							goto bailout;
						}
					}
				}
				break;
			}

			case HUNK_RELRELOC32:
			{
				if ( DoVerbose > 1 )
				{
					printf( "Hunk ID ........... : [$%04X] HUNK_RELRELOC32\n", hunktype );

					if ( DoVerbose > 2 )
					{
						printf( "File Offset ....... : $%08x\n", hps.pos*4 );
					}
				}

				if ( ! current )
				{
					ec = RS4ErrStat_FileStructure;
					printf( "\nHunk structure sanity check failure\nUnexpected HUNK_DREL32\n" );
					goto bailout;
				}

				hps.bytes = 0;
				hps.pos++;

				while( TRUE )
				{
					numreloc = READ_U16();

					if ( DoVerbose > 2 )
					{
						printf( " Relocs ........... : #%d\n", numreloc );
					}

					if ( ! numreloc )
					{
						break;
					}

					hunknum = READ_U16();

					if ( DoVerbose > 2 )
					{
						printf( " Reloc Target ..... : #%d\n", hunknum );
					}

					if ( hunknum >= sections )
					{
						ec = RS4ErrStat_FileStructure;
						printf( "\nHunk Numner is illegal\n\nHunk Nr %d is out of range\nFile only has %d hunks", hunknum, sections );
						goto bailout;
					}

					relcnt = 1;	
					TargetAdr	= fh->rfh_SecArray[ hunknum ].rsi_MemoryAdr;
					TargetSec	= fh->rfh_SecArray[ hunknum ].rsi_Section;

					while( numreloc-- )
					{
						val16 = READ_U16();
						offset = val16; // & 0x7fff;

						if ( DoVerbose > 2 )
						{
							printf( " Reloc Offset .3... : $%08" PRIx64 " #%d\n", offset, relcnt++ );
						}

						if ( current->rfs_MemorySize < offset )
						{
							ec = RS4ErrStat_FileStructure;
							printf( "\nHunk reloc offset is illegal\n\nOffset %" PRId64 " is out of range\nHunk %d is only %" PRId64 " bytes big", offset, hunknum, fh->rfh_SecArray[ hunknum ].rsi_MemorySize );
							goto bailout;
						}

						DestMemory	= (uint32_t *) & current->rfs_MemoryBuf[ offset ];

						val32		= *DestMemory;
						val32		= SWAP32( val32 );
						val32		+= TargetAdr;

						*DestMemory	= SWAP32( ( val32 - current->rfs_MemoryAdr - offset ) );

						// Possible Section jump
						rl = RS4AddLabel_File( & ec, fh, val32, RS4LabelType_Unset );

						if ( ! rl )
						{
							// ec allready set
							fs = RS4FuncStat_Error;
							ec = RS4ErrStat_Error;

							#ifdef DEBUG
							printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
							#endif

							goto bailout;
						}

						// This is where the code/data points from
						// We need the info, when dealing with Data areas
						rr = RS4AddRef_Sec( & ec, current, offset );

						if ( ! rr )
						{
							fs = RS4FuncStat_Error;
							// ec allready set

							#ifdef DEBUG
							printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
							#endif

							goto bailout;
						}
					}
				}
				break;
			}

			default:
			{
				ec = RS4ErrStat_FileStructure;

				if ( DoVerbose > 0 )
				{
					char txt[] = { "'....'" };
					int v1 = (( hunktype & 0xff000000 ) >> 24 );
					int v2 = (( hunktype & 0x00ff0000 ) >> 16 );
					int v3 = (( hunktype & 0x0000ff00 ) >>  8 );
					int v4 = (( hunktype & 0x000000ff ) >>  0 );
					txt[1] = (( v1 > 0x1f ) && ( v1 < 127 )) ? v1 : '.';
					txt[2] = (( v2 > 0x1f ) && ( v2 < 127 )) ? v2 : '.';
					txt[3] = (( v3 > 0x1f ) && ( v3 < 127 )) ? v3 : '.';
					txt[4] = (( v4 > 0x1f ) && ( v4 < 127 )) ? v4 : '.';
					printf( "File Offset ....... : $%08x\n", hps.pos*4 );
					printf( "Unsupported Hunk Type 0x%08X : %s\n", hunktype, txt );
				}
				else
				{
					printf( "\nUnsupported Hunk Type 0x%08X\n", hunktype );
				}
				goto bailout;
			}
		}
	}

	if ( DoVerbose > 1 )
	{
		printf( "\n" );
	}

	// --

	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fs );
}

// --
