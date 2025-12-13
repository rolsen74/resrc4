
/*
** Copyright (c) 2014-2025 Rene W. Olsen
**
** SPDX-License-Identifier: GPL-3.0-or-later
**
** This software is released under the GNU General Public License, version 3.
** For the full text of the license, please visit:
** https://www.gnu.org/licenses/gpl-3.0.html
**
** You can also find a copy of the license in the LICENSE file included with this software.
*/

// --

#include "ReSrc4.h"
#include "Resourcer/MD5.h"

// --

void RS4FreeFile( enum RS4ErrorCode *errcode, RS4FileHeader *fh )
{
enum RS4ErrorCode ec;

	ec = RS4ErrStat_Error;

	if ( ! fh )
	{
		goto bailout;
	}

	if ( fh->rfh_ID != RS4ID_FileHeader )
	{
		ec = RS4ErrStat_InvalidStructID;

		#ifdef DEBUG
		printf( "%s:%04d: Error Invalid Struct ID\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	fh->rfh_ID = 0;

	if ( fh->rfh_FileBuffer )
	{
		free( fh->rfh_FileBuffer );
		fh->rfh_FileBuffer = NULL;
		fh->rfh_FileSize = 0;
	}

	free( fh );

	ec = RS4ErrStat_Okay;

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}
}

// --

RS4FileHeader *RS4LoadFile( enum RS4ErrorCode *errcode, STR filename )
{
enum RS4ErrorCode ec;
RS4FileHeader *fh;
MD5Context ctx;
FILE *file;
U64 fsize;
U64 size;
PTR mem;
S32 err;
S32 cnt;

	err = TRUE;

	ec = RS4ErrStat_Error;

	fh = calloc( 1, sizeof( RS4FileHeader ));

	if ( ! fh )
	{
		ec = RS4ErrStat_OutOfMemory;

		#ifdef DEBUG
		printf( "%s:%04d: Error allocating memory (%d Bytes)\n", __FILE__, __LINE__, (S32) sizeof( RS4FileHeader ));
		#endif

		goto bailout;
	}

	fh->rfh_ID = RS4ID_FileHeader;

	// -- Open File

	file = fopen( filename, "rb" );

	if ( ! file )
	{
		/**/ if ( errno == ENOENT )
		{
			ec = RS4ErrStat_FileNotFound;
		}
		else if ( errno == EISDIR )
		{
			ec = RS4ErrStat_FileIsADir;
		}
		else
		{
			ec = RS4ErrStat_OpeningFile;
		}

		#ifdef DEBUG
		printf( "Debug: Error opening file '%s'\n", FindFileName( filename ));
		#endif

		goto bailout;
	}

	// -- Get File Size

	if ( fseek( file, 0, SEEK_END ))
	{
		ec = RS4ErrStat_FileIO;

		#ifdef DEBUG
		printf( "Error doing file Seek '%s'\n", filename );
		#endif

		goto bailout;
	}

	size = ftell( file );

	if ( size == -1UL )
	{
		ec = RS4ErrStat_FileIO;

		#ifdef DEBUG
		printf( "Error doing file Tell '%s'\n", filename );
		#endif

		goto bailout;
	}

	if ( fseek( file, 0, SEEK_SET ))
	{
		ec = RS4ErrStat_FileIO;

		#ifdef DEBUG
		printf( "Error doing file Seek '%s'\n", filename );
		#endif

		goto bailout;
	}

	fh->rfh_FileSize = size;

	// -- Alloc File Buffer

	mem = calloc( 1, size + 1 );

	if ( ! mem )
	{
		ec = RS4ErrStat_OutOfMemory;

		#ifdef DEBUG
		printf( "%s:%04d: Error allocating memory (%d Bytes)\n", __FILE__, __LINE__, (S32) size );
		#endif

		goto bailout;
	}

	fh->rfh_FileBuffer = mem;

	// -- Load File

	fsize = fread( mem, 1, size, file );

	if ( fsize != size )
	{
		ec = RS4ErrStat_FileIO;

		#ifdef DEBUG
		printf( "Error doing file Read '%s'\n", filename );
		#endif

		goto bailout;
	}

	fh->rfh_FileSize = size;

	// -- Copy file name+path

	fh->rfh_FileName = RS4Strdup( filename );

	if ( ! fh->rfh_FileName )
	{
		ec = RS4ErrStat_OutOfMemory;

		#ifdef DEBUG
		printf( "%s:%04d: Error '%s'\n", __FILE__, __LINE__, filename );
		#endif

		goto bailout;
	}

	// -- NUL terminate file, in case we have an ASCII file.
	// It makes parsing the file so much easier/safer

	((STR )mem)[size] = 0;

	// -- Calculate MD5 hash

	md5Init( & ctx );

	md5Update( & ctx, mem, size );

	md5Finalize( & ctx );

	for( cnt=0 ; cnt<16 ; cnt++ )
	{
		sprintf( & fh->rfh_FileMD5[ cnt*2 ], "%02x", ctx.digest[cnt] );
	}

	// --

	err = FALSE;
	ec = RS4ErrStat_Okay;

bailout:

	if (( err ) && ( fh ))
	{
		RS4FreeFile( NULL, fh );
		fh = NULL;
	}

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fh );
}

// --

enum RS4FileType RS4IdentifyFile( enum RS4ErrorCode *errcode, RS4FileHeader *file )
{
enum RS4ErrorCode ec;
enum RS4FileType type;
U32 fid;

	ec = RS4ErrStat_Error;

	type = RS4FileType_Unknown;

	if (( ! file ) || ( file->rfh_ID != RS4ID_FileHeader ))
	{
		ec = RS4ErrStat_InvalidStructID;
		type = RS4FileType_Error;
		goto bailout;
	}

	// todo check for other file type, (in a smart way)

	#ifdef SUPPORT_HUNK

	if ( type == RS4FileType_Unknown )
	{
		fid =	( file->rfh_FileBuffer[0] << 24 ) +
				( file->rfh_FileBuffer[1] << 16 ) +
				( file->rfh_FileBuffer[2] <<  8 ) +
				( file->rfh_FileBuffer[3] <<  0 ) ;

		/**/ if ( fid == HUNK_HEADER )
		{
			type = RS4FileType_Hunk;
		}
	}

	#endif

	#ifdef SUPPORT_FHR

	if ( type == RS4FileType_Unknown )
	{
		struct FHR_Header *h = (PTR) file->rfh_FileBuffer;

		if (( SWAP32( h->FHR_ID )			== FHR_HEADER )
		&&	( SWAP16( h->FHR_OSType )		== FHR_OS_AmigaOS3 )
		&&	( SWAP16( h->FHR_CPUType )		== FHR_CPU_M68k )
		&&	( SWAP16( h->FHR_Encryption )	== FHR_ENC_None )
		&&	( h->FHR_Size	== FHR_SIZE_32 )
		&&	( h->FHR_Type	== FHR_TYPE_Exe )
		&&	( h->FHR_Pack	== FHR_PACK_None )
		&&	( h->FHR_Endian == FHR_ENDIAN_Big ))
		{
			type = RS4FileType_FHR;
		}
	}

	#endif

	ec = RS4ErrStat_Okay;

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	return( type );
}

// --

RS4FileHeader *RS4LoadExe( enum RS4ErrorCode *errcode, STR filename )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4FileHeader *file;
S32 type;
S32 err;

	ec = RS4ErrStat_Error;

	err = TRUE;

	file = RS4LoadFile( & ec, filename );

	if ( ! file )
	{
		// ec already set

		#ifdef DEBUG
		printf( "%s:%04d: Error loading file '%s'\n", __FILE__, __LINE__, filename );
		#endif

		goto bailout;
	}

	if ( DoVerbose > 0 )
	{
		printf( "Loaded '%s' (%" PRId64 " bytes)\n", filename, file->rfh_FileSize );
	}

	type = RS4IdentifyFile( & ec, file );

	if ( type == RS4FileType_Error )
	{
		// ec already set

		#ifdef DEBUG
		printf( "%s:%04d: Error identifying file '%s'\n", __FILE__, __LINE__, filename );
		#endif

		goto bailout;
	}

	if ( type == RS4FileType_Unknown )
	{
		ec = RS4ErrStat_UnsupportedFileType;

		#ifdef DEBUG
		printf( "%s:%04d: Error unsupported file type '%s'\n", __FILE__, __LINE__, filename );
		#endif

		goto bailout;

	}

	file->rfh_FileType = type;

	// Set First Virtual Address (for Sections)
	RS4CurrentVirtAdr = 0x00010000;

	switch( type )
	{
		#ifdef SUPPORT_FHR
		case RS4FileType_FHR:
		{
			fs = FHR_ParseFile( & ec, file );
			break;
		}
		#endif

		#ifdef SUPPORT_HUNK
		case RS4FileType_Hunk:
		{
			fs = Hunk_ParseFile( & ec, file );
			break;
		}
		#endif

		default:
		{
			ec = RS4ErrStat_UnsupportedFileType;

			#ifdef DEBUG
			printf( "Unknown file type\n" );
			#endif

			goto bailout;
		}
	}

	if ( fs != RS4FuncStat_Okay )
	{
		printf( "Error loading file\n" );
		goto bailout;
	}

	err = FALSE;

	ec = RS4ErrStat_Okay;

bailout:

	if (( err ) && ( file ))
	{
		RS4FreeFile( NULL, file );
		file = NULL;
	}

	if ( errcode )
	{
		*errcode = ec;
	}

	return( file );
}

// --
