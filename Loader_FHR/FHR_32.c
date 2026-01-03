
/*
** Copyright (c) 2025 by Rene W. Olsen
**
** SPDX-License-Identifier: GPL-3.0-or-later
**
** This software is released under the GNU General Public License, version 3.
** For the full text of the license, please visit:
** https://www.gnu.org/licenses/gpl-3.0.html
**
** You can also find a copy of the license in the LICENSE file included with this software.
**
*/

// --

#include "Resourcer/ReSrc4.h"

// --

struct ParseStruct
{
	RS4FileSection *sec;	// Current section
	U8 *	mem;	// Buffer ptr
	S64		pos;	// Current pos
	S64		max;	// Bytes left .. Section len or File Size
};

// --

static U32 my_RawRead_U32( struct ParseStruct *ps, U64 off )
{
MEM mem;
U32 val;

	mem = (PTR) & ps->sec->rfs_MemoryBuf[ off ];

	val = 0;
	val |= (U32) *mem++ << 24;
	val |= (U32) *mem++ << 16;
	val |= (U32) *mem++ <<  8;
	val |= (U32) *mem++ <<  0;

	return( val );
}

// --

static void my_RawWrite_U32( struct ParseStruct *ps, U64 off, U32 val )
{
MEM mem;

	mem = & ps->sec->rfs_MemoryBuf[ off ];

	*mem++ = ( val >> 24 ) & 0xff ;
	*mem++ = ( val >> 16 ) & 0xff ;
	*mem++ = ( val >>  8 ) & 0xff ;
	*mem++ = ( val >>  0 ) & 0xff ;
}

// --

static void my_Read_U8( enum RS4ErrorCode *ecptr, struct ParseStruct *ps, U8 *valptr )
{
enum RS4ErrorCode ec;
U8 *mem;
U8 val;

	/**/ if ( ps->pos < 0 )
	{
		printf( "Error Reading 8 : Pos %" PRId64 "\n", ps->pos );

		ec = RS4ErrStat_Error;
	}
	else
	{
		mem = (PTR) & ps->mem[ ps->pos ];
		val = *mem;

		// todo check for pos overflow?
		ps->pos += 1;
		ps->max -= 1;

		ec = RS4ErrStat_Okay;
	}

	if ( valptr )	*valptr	= ( ec == RS4ErrStat_Okay ) ? val : 0;
	if ( ecptr )	*ecptr	= ec;
}

// --

static void my_Read_U32_Swap( enum RS4ErrorCode *ecptr, struct ParseStruct *ps, U32 *valptr )
{
enum RS4ErrorCode ec;
U32 *mem;
U32 val;

	/**/ if ( ps->pos < 0 )
	{
		printf( "Error Reading 32 Swap : Pos %" PRId64 "\n", ps->pos );

		ec = RS4ErrStat_Error;
	}
	else
	{
		mem = (PTR) & ps->mem[ ps->pos ];
		val = *mem;
		val = SWAP32( val );

		// todo check for pos overflow?
		ps->pos += 4;
		ps->max -= 4;

		ec = RS4ErrStat_Okay;
	}

	if ( valptr )	*valptr	= ( ec == RS4ErrStat_Okay ) ? val : 0;
	if ( ecptr )	*ecptr	= ec;
}

// --

static void my_Read_U32_Big( enum RS4ErrorCode *ecptr, struct ParseStruct *ps, U32 *valptr )
{
enum RS4ErrorCode ec;
U32 val;

	if ( ps->pos < 0 )
	{
		printf( "Error Reading 32 Big : Pos %" PRId64 "\n", ps->pos );

		ec = RS4ErrStat_Error;
	}
	else
	{
		val =	(( (U32) ps->mem[ ps->pos + 0 ] << 24 ) |
				 ( (U32) ps->mem[ ps->pos + 1 ] << 16 ) |
				 ( (U32) ps->mem[ ps->pos + 2 ] <<  8 ) |
				 ( (U32) ps->mem[ ps->pos + 3 ] <<  0 ) );

		// todo check for pos overflow?
		ps->pos += 4;
		ps->max -= 4;

		ec = RS4ErrStat_Okay;
	}

	if ( valptr )	*valptr	= ( ec == RS4ErrStat_Okay ) ? val : 0;
	if ( ecptr )	*ecptr	= ec;
}

// --

static void my_Read_S32_ID( enum RS4ErrorCode *ecptr, struct ParseStruct *ps, U32 *idptr, S32 *lenptr )
{
enum RS4ErrorCode ec;
U32 len;
U32 ID;

//	ps.ps_SecOffset = ps->pos;

	// -- Read ID -- Big Endian Format --

	my_Read_U32_Big( & ec, ps, & ID );

	if ( ec != RS4ErrStat_Okay )
	{
		printf( "%s:%04d: Error : ec %d\n", __FILE__, __LINE__, ec );
		goto bailout;
	}

	// -- Read Section Length ( execpt for END0 and BSS0 )

	if (( ID != FHR_END0 ) && ( ID != FHR_BSS0 ))
	{
		// -- Read length with Endian swap

		my_Read_U32_Swap( & ec, ps, & len );

		if ( ec != RS4ErrStat_Okay )
		{
			printf( "%s:%04d: Error : ec %d\n", __FILE__, __LINE__, ec );
			goto bailout;
		}
	}
	else
	{
		len = 0;
		ec = RS4ErrStat_Okay;
	}	

bailout:

	// -- Set Return Values

	if ( lenptr )	*lenptr = ( ec == RS4ErrStat_Okay ) ? (S32) len : 0;
	if ( idptr )	*idptr	= ( ec == RS4ErrStat_Okay ) ? ID : 0;
	if ( ecptr )	*ecptr	= ec;
}

// --

static void my_Read_Reloc( enum RS4ErrorCode *ecptr, struct ParseStruct *ps, U64 *valptr )
{
enum RS4ErrorCode ec;
U64 val;
S32 c;
U8 v8;

	my_Read_U8( & ec, ps, & v8 );

	if ( ec != RS4ErrStat_Okay )
	{
		printf( "%s:%04d: Error : ec %d\n", __FILE__, __LINE__, ec );
		goto bailout;
	}

	c = v8;

	if ( c < 0x80 )
	{
		val = c;
	}
	else
	{
		val = 0;

		for( int mask=1 ; mask<256 ; mask *= 2 )
		{
			val = val << 8;

			if ( c & mask )
			{
				my_Read_U8( & ec, ps, & v8 );

				if ( ec != RS4ErrStat_Okay )
				{
					printf( "%s:%04d: Error : ec %d\n", __FILE__, __LINE__, ec );
					goto bailout;
				}
			
				val += v8;
			}
		}
	}

	ec = RS4ErrStat_Okay;

bailout:

	if ( valptr )
	{
		*valptr = ( ec == RS4ErrStat_Okay ) ? val : 0 ;
	}

	if ( ecptr )
	{
		*ecptr = ec;
	}
}

// --

static void my_Parse_Reloc_Exe( enum RS4ErrorCode *ecptr, struct ParseStruct *ps, RS4FileHeader *fh )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
S64 reloff;
U64 addval;
U64 type;
U64 num;
U64 sec;
U64 off;
U32 tmp;

	while( TRUE )
	{
		reloff = ps->pos;

		// Number of Relocs
		my_Read_Reloc( & ec, ps, & num );
	
		if ( ec != RS4ErrStat_Okay )
		{
			goto bailout;
		}

//		if ( display )
		{
			printf( "\n  File  Offset .... : $%08" PRIX64 "  : %" PRId64 "\n", reloff, reloff );
			printf( "  Reloc Num ....... : $%08" PRIX64 "  : %" PRId64 "\n", num, num );
		}

		if ( ! num )
		{
			break;
		}

		// Target Section
		my_Read_Reloc( & ec, ps, & sec );

		if ( ec != RS4ErrStat_Okay )
		{
			goto bailout;
		}

//		if ( display )
		{
			printf( "  Reloc Target .... : $%08" PRIX64 "  : SecNr #%" PRId64 "\n", sec, sec );
		}

		if ( sec >= (U64) fh->rfh_SecArraySize )
		{
			printf( "%s:%04d: Invalid Section Number .. Out of range (%" PRId64 ")\n", __FILE__, __LINE__, sec );
			ec = RS4ErrStat_Error;
			goto bailout;
		}

		// Reloc Type
		my_Read_Reloc( & ec, ps, & type );

		if ( ec != RS4ErrStat_Okay )
		{
			goto bailout;
		}

//		if ( display )
//		{
//			char *txt[] = { "Reloc", "xDef", "xRef", "??" }; 
//			printf( "  Reloc Type ...... : $%08" PRIx64 "  : %" PRId64 " : %s\n", type, type, MAXSTRING( type, 3 ));
//		}

		/**/ if ( type == FHR_RELTYPE_RELOC )
		{
			addval = fh->rfh_SecArray[sec].rsi_MemoryAdr;

			while( num-- )
			{
				// Reloc Offset
				my_Read_Reloc( & ec, ps, & off );

				if ( ec != RS4ErrStat_Okay )
				{
					goto bailout;
				}
		
//				if ( display )
				{
					printf( "  Reloc Offset .... : $%08" PRIX64 "  : %" PRId64 "\n", off, off );
				}

				tmp = my_RawRead_U32( ps, off );
				tmp += addval;
				my_RawWrite_U32( ps, off, tmp );

				ERR_CHK( RS4AddRef_Sec( & ec, NULL, ps->sec, off ))
				ERR_CHK( RS4AddLabel_File( & ec, NULL, fh, tmp, RS4LabelType_Unset, __FILE__ ))
			}
		}
		else
		{
			printf( "%s:%04d: Unsupported Reloc Type (%" PRId64 ")\n", __FILE__, __LINE__, type );
			ec = RS4ErrStat_Error;
			goto bailout;
		}
	}

	ec = RS4ErrStat_Okay;

bailout:

	if ( ecptr )
	{
		*ecptr = ec;
	}
}

// --

enum RS4FuncStat FHR_ParseFile_32( enum RS4ErrorCode *errcode, RS4FileHeader *fh, struct FHR_Header *h )
{
struct ParseStruct ps;
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
//RS4FileSection *current;
RS4FileSection *sec;
RS4FileSecInfo *rsi;
S32 sections;
S64 curpos;
S64 curmax;
S32 secnr;
S32 len32;
S32 cnt;
U32 id;

	fs = RS4FuncStat_Error;
	ec = RS4ErrStat_Error;

	// --

	memset( & ps, 0, sizeof( ps ));
	ps.mem = (PTR) fh->rfh_FileBuffer;
	ps.pos = sizeof( struct FHR_Header );
	ps.max = fh->rfh_FileSize - ps.pos;

	// --

	sections = SWAP16( h->FHR_Sections );

	fh->rfh_SecArray = calloc( 1, sections * sizeof( RS4FileSecInfo ));

	if ( ! fh->rfh_SecArray )
	{
		ec = RS4ErrStat_OutOfMemory;

		#ifdef DEBUG
		printf( "%s:%04d: Error allocating memory (%d Bytes)\n", __FILE__, __LINE__, (S32) ( sections * sizeof( RS4FileSecInfo ) ));
		#endif

		goto bailout;
	}

	fh->rfh_SecArraySize = sections;

	// --
	// Parse Section Info


	for( cnt=0 ; cnt<sections ; cnt++ )
	{
		// Read 'struct SEC_Header_32'
		U32	Size;
		my_Read_U32_Swap( & ec, & ps, & Size );

//		U16	Align	= READ_U16();
		ps.pos	+= 2;	// Skip
		ps.max	-= 2;	// Skip

//		U16	Flags	= READ_U16();
		ps.pos	+= 2;	// Skip
		ps.max	-= 2;	// Skip

		// --

		ERR_CHK( RS4AllocSection( & ec, & sec, cnt, Size ))

		if ( ! sec )
		{
			// ec allready set

			#ifdef DEBUG
			printf( "%s:%04d: Error allocating memory (%d Bytes)\n", __FILE__, __LINE__, (S32) ( sections * sizeof( RS4FileSection ) ));
			#endif

			goto bailout;
		}

		sec->rfs_File 		= fh;

		rsi = & fh->rfh_SecArray[cnt];
		rsi->rsi_Section	= sec;
		rsi->rsi_MemoryAdr	= sec->rfs_MemoryAdr;
		rsi->rsi_MemorySize	= Size;
	}

	// trace start adr .. hunk use First, not zero
	fh->rfh_StartAdr = fh->rfh_SecArray[0].rsi_MemoryAdr;

	// --
	// Parse Sections

//	current = NULL;
	secnr = 0;
	sec = fh->rfh_SecArray[secnr].rsi_Section;

	while( TRUE )
	{
//		printf( "> Pos %ld : Max %ld : SecNr %d : Sec %p : Cur %p\n", ps.pos, ps.max, secnr, sec, current );

		// --
		if (( fh->rfh_FileSize == ps.pos ) && ( ps.max == 0 ))
		{
			// Normal exit
			break;
		}

		if ( fh->rfh_FileSize < ps.pos )
		{
			ec = RS4ErrStat_Error;
			printf( "%s:%04d: Error : Overrun : ec %d\n", __FILE__, __LINE__, ec );
			goto bailout;
		}

		if ( ps.max < 0 )
		{
			ec = RS4ErrStat_Error;
			printf( "%s:%04d: Error : Overrun : ec %d\n", __FILE__, __LINE__, ec );
			goto bailout;
		}

		// --
		my_Read_S32_ID( & ec, & ps, & id, & len32 );

		if ( ec != RS4ErrStat_Okay )
		{
			printf( "%s:%04d: Error : ec %d\n", __FILE__, __LINE__, ec );
			goto bailout;
		}

		curpos = ps.pos;
		curmax = ps.max;

		switch( id )
		{
			case FHR_COD0:
			{
				if ( ps.sec )
				{
					ec = RS4ErrStat_FileStructure;
					printf( "\nFHR structure sanity check failure\nMissing FHR_END0\n" );
					goto bailout;
				}

				ps.sec = sec;
				sec->rfs_SecType	= RS4ST_Code;
				sec->rfs_DataSize	= sec->rfs_MemorySize;
				sec->rfs_FileOffset	= ps.pos + 2;	// +2 skip CRC

				if ( sec->rfs_DataSize > 0 )
				{
					memcpy(
						sec->rfs_MemoryBuf,
						& fh->rfh_FileBuffer[ sec->rfs_FileOffset ],
						sec->rfs_DataSize
					);
				}

				ps.pos = sec->rfs_FileOffset + sec->rfs_DataSize;
				my_Parse_Reloc_Exe( & ec, & ps, fh );

				if ( ec != RS4ErrStat_Okay )
				{
					printf( "\nFHR structure sanity check failure\nError decoding COD0 Relocs\n" );
					goto bailout;
				}

				if ( ps.pos != curpos + len32 )
				{
					printf( "\nFHR structure sanity check failure\nWrong COD0 Length\n" );
					goto bailout;
				}
				break;
			}

			case FHR_DAT0:
			{
				if ( ps.sec )
				{
					ec = RS4ErrStat_FileStructure;
					printf( "\nFHR structure sanity check failure\nMissing FHR_END0\n" );
					goto bailout;
				}

				ps.sec = sec;
				sec->rfs_SecType	= RS4ST_Data;
				sec->rfs_DataSize	= sec->rfs_MemorySize;
				sec->rfs_FileOffset	= ps.pos + 2;	// +2 skip CRC

				if ( sec->rfs_DataSize > 0 )
				{
					memcpy(
						sec->rfs_MemoryBuf,
						& fh->rfh_FileBuffer[ sec->rfs_FileOffset ],
						sec->rfs_DataSize
					);
				}

				ps.pos = sec->rfs_FileOffset + sec->rfs_DataSize;
				my_Parse_Reloc_Exe( & ec, & ps, fh );

				if ( ec != RS4ErrStat_Okay )
				{
					printf( "\nFHR structure sanity check failure\nError decoding DAT0 Relocs\n" );
					goto bailout;
				}

				if ( ps.pos != curpos + len32 )
				{
					printf( "\nFHR structure sanity check failure\nWrong DAT0 Length\n" );
					goto bailout;
				}
				break;
			}

			case FHR_BSS0:
			{
				if ( ps.sec )
				{
					ec = RS4ErrStat_FileStructure;
					printf( "\nFHR structure sanity check failure\nMissing FHR_END0\n" );
					goto bailout;
				}

				ps.sec = sec;
				sec->rfs_SecType	= RS4ST_BSS;
				sec->rfs_DataSize	= sec->rfs_MemorySize;
				sec->rfs_FileOffset	= ps.pos + 8;
				break;
			}

			case FHR_END0:
			{
				if ( ! ps.sec )
				{
					ec = RS4ErrStat_FileStructure;
					printf( "\nFHR structure sanity check failure\nUnexpected FHR_END0\n" );
					goto bailout;
				}

				secnr++;

				if ( secnr > sections )
				{
					ec = RS4ErrStat_FileStructure;
					printf( "\nFHR structure sanity check failure\nUnexpected FHR_END0\n" );
					goto bailout;
				}

				ps.sec = NULL;

				sec = fh->rfh_SecArray[secnr].rsi_Section;
				break;
			}

			default:
			{
				printf( "Unsupported ID : $%08x\n", id );
				break;
			}
		}

		ps.pos = curpos + len32;
		ps.max = curmax - len32;
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
