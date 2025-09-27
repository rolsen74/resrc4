 
/*
 * Copyright (c) 2014-2025 Rene W. Olsen < renewolsen @ gmail . com >
 *
 * This software is released under the GNU General Public License, version 3.
 * For the full text of the license, please visit:
 * https://www.gnu.org/licenses/gpl-3.0.html
 *
 * You can also find a copy of the license in the LICENSE file included with this software.
 */

// --

#include "Resourcer/ReSrc4.h"

// --

#define SaveBufferSize		4096
#define LineBufferSize		1024

static FILE *				SaveHandle		= NULL;
static U64				SaveUsed		= 0;
static MEM 					SaveBuffer		= NULL;
STR 						SaveLineBuffer	= NULL;
static U64				SavedBytes		= 0;

// --

static enum RS4FuncStat RS4SaveFlush( enum RS4ErrorCode *errcode )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
U64 written;

	ec = RS4ErrStat_Error;
	fs = RS4FuncStat_Error;

	if (( ! SaveHandle ) || ( ! SaveBuffer ))
	{
		ec = RS4ErrStat_Internal;

		#ifdef DEBUG
		printf( "%s:%04d: NULL Pointer\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	if ( ! SaveUsed )
	{
		ec = RS4ErrStat_Internal;

		#ifdef DEBUG
		printf( "%s:%04d: Not Used\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	written = fwrite( SaveBuffer, 1, SaveUsed, SaveHandle );

	if ( written != SaveUsed )
	{
		ec = RS4ErrStat_FileIO;

		#ifdef DEBUG
		printf( "%s:%04d: Error writting data\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	SavedBytes += SaveUsed;

	SaveUsed = 0;

	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fs );
}

// --

enum RS4FuncStat RS4SaveWriteString( enum RS4ErrorCode *errcode, STR buffer, U64 length )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
U64 len;

	ec = RS4ErrStat_Error;
	fs = RS4FuncStat_Error;

	if (( ! SaveHandle ) || ( ! SaveBuffer ))
	{
		ec = RS4ErrStat_Internal;

		#ifdef DEBUG
		printf( "%s:%04d: NULL Pointer\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	while( length )
	{
		len = MIN( length, SaveBufferSize - SaveUsed );

		if ( len <= 0 )
		{
			break;
		}

		memcpy( & SaveBuffer[ SaveUsed ], buffer, len );

		SaveUsed += len;
		buffer += len;
		length -= len;

		if ( SaveBufferSize <= SaveUsed )
		{
			fs = RS4SaveFlush( & ec );

			if ( fs != RS4FuncStat_Okay )
			{
				// ec allready set

				#ifdef DEBUG
				printf( "%s:%04d: Save Flush Failed\n", __FILE__, __LINE__ );
				#endif

				goto bailout;
			}
		}
	}

	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fs );
}

// --

enum RS4FuncStat RS4ExternalLabel( enum RS4ErrorCode *errcode, RS4FileHeader *fh )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Label *rl;
S32 len;

	ec = RS4ErrStat_Error;
	fs = RS4FuncStat_Error;
	rl = RS4GetHead( & fh->rfh_ExtLabelList );

	if ( ! rl )
	{
		// Not an error
		ec = RS4ErrStat_Okay;
		fs = RS4FuncStat_Okay;
		goto bailout;
	}

	// --

	sprintf( SaveLineBuffer, "\n; External\n" );

	fs = RS4SaveWriteString( & ec, SaveLineBuffer, strlen( SaveLineBuffer ));

	if ( fs != RS4FuncStat_Okay )
	{
		// ec allready set

		#ifdef DEBUG
		printf( "%s:%04d: Error Writting External Data\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	// --

	while( rl )
	{
		len = strlen( rl->rl_Name );

		if ( len < 8 )
		{
			sprintf( SaveLineBuffer, "%s:\t\t\tEQU\t$%08" PRIX64 "\n", rl->rl_Name, rl->rl_Offset );
		}
		else if ( len < 16 )
		{
			sprintf( SaveLineBuffer, "%s:\t\tEQU\t$%08" PRIX64 "\n", rl->rl_Name, rl->rl_Offset );
		}
		else if ( len < 24 )
		{
			sprintf( SaveLineBuffer, "%s:\tEQU\t$%08" PRIX64 "\n", rl->rl_Name, rl->rl_Offset );
		}
		else
		{
			sprintf( SaveLineBuffer, "%s: EQU\t$%08" PRIX64 "\n", rl->rl_Name, rl->rl_Offset );
		}

		fs = RS4SaveWriteString( & ec, SaveLineBuffer, strlen( SaveLineBuffer ));

		if ( fs != RS4FuncStat_Okay )
		{
			// ec allready set

			#ifdef DEBUG
			printf( "%s:%04d: Error Writting External data\n", __FILE__, __LINE__ );
			#endif

			goto bailout;
		}

		rl = RS4GetNext( rl );
	}

	// --

	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

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

#define TAB_SIZE 8

static void _GetStringInfo( CSTR str, S32 *lenptr, S32 *posptr )
{
S32 pos;
S32 len;
S32 chr;

	pos = 0;
	len = 0;

	while( TRUE )
	{
		chr = *str++;

		if ( ! chr )
		{
			break;
		}

		len++;

		/**/ if ( chr == 9 )
		{
			pos = ( pos + TAB_SIZE ) - ( pos % TAB_SIZE );
		}
		else if ( chr == 10 )
		{
			pos = 0;
		}
		else
		{
			pos++;
		}
	}

	if ( lenptr )
	{
		*lenptr = len;
	}

	if ( posptr )
	{
		*posptr = pos;
	}
}

// --

static enum RS4FuncStat RS4SaveWrite_Code( enum RS4ErrorCode *errcode, RS4Source *rs )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
struct _SrcCode *sc;
CHR labname[ MAX_LabelName + 2 ];
CHR adrname[256];
RS4Label *rl;
S32 len;
S32 oo;

	ec = RS4ErrStat_Error;
	fs = RS4FuncStat_Error;

	if (( ! SaveHandle ) || ( ! SaveBuffer ))
	{
		ec = RS4ErrStat_Internal;

		#ifdef DEBUG
		printf( "%s:%04d: NULL Pointer\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	#ifdef DEBUG
	if ( rs->rs_Type != RS4SourceType_Code )
	{
		fs = RS4FuncStat_Error;
		ec = RS4ErrStat_Internal;

		#ifdef DEBUG
		printf( "%s:%04d: NULL Pointer\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}
	#endif

	sc = & rs->rs_Data.rs_Code;

	// -- Label

	rl = sc->rs_Label;

	if (( rl ) && ( ! rl->rl_Parent ) && ( rl->rl_Name[0] ))
	{
		sprintf( labname, "%s:", rl->rl_Name );
	}
	else
	{
		labname[0] = 0;
	}

	sprintf( SaveLineBuffer, "%s", labname );

	// --

	if (( *sc->rs_Opcode ) || ( *sc->rs_Argument ) || (( DebugInfo ) && ( sc->rs_Address )))
	{
		_GetStringInfo( labname, NULL, & len );

		oo = LabTabs * 8;

		if ( len >= oo )
		{
			strcat( SaveLineBuffer, "\n" );
			len = 0;
		}

		while( len < oo )
		{
			strcat( SaveLineBuffer, "\t" );
			oo -= 8;
		}
	}

	// --

	strcat( SaveLineBuffer, sc->rs_Opcode );

	if (( *sc->rs_Argument ) || (( DebugInfo ) && ( sc->rs_Address )))
	{
		_GetStringInfo( sc->rs_Opcode, NULL, & len );

		oo = OpcodeTabs * 8;

		if ( len >= oo )
		{
			strcat( SaveLineBuffer, " " );
		}

		while( len < oo )
		{
			strcat( SaveLineBuffer, "\t" );
			oo -= 8;
		}

		strcat( SaveLineBuffer, sc->rs_Argument );
	}

	// --

	if (( DebugInfo ) && ( sc->rs_Address ))
	{
		_GetStringInfo( sc->rs_Argument, NULL, & len );

		oo = ArgTabs * 8;

		if ( len >= oo )
		{
			strcat( SaveLineBuffer, " " );
		}

		while( len < oo )
		{
			strcat( SaveLineBuffer, "\t" );
			oo -= 8;
		}

		if ( sc->rs_Comment )
		{
			sprintf( adrname, "; $%08" PRIx64 " %s", sc->rs_Address, sc->rs_Comment );
		}
		else
		{
			sprintf( adrname, "; $%08" PRIx64 "", sc->rs_Address );
		}

		strcat( SaveLineBuffer, adrname );
	}

	// --

	strcat( SaveLineBuffer, "\n" );

	fs = RS4SaveWriteString( & ec, SaveLineBuffer, strlen( SaveLineBuffer ));

	if ( fs != RS4FuncStat_Okay )
	{
		// ec allready set

		#ifdef DEBUG
		printf( "%s:%04d: Error writting string\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	// --

	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fs );
}

// --

static enum RS4FuncStat RS4SaveWrite_String( enum RS4ErrorCode *errcode, RS4Source *rs )
{
struct _SrcString *ss;
enum RS4ErrorCode ec;
enum RS4FuncStat fs;

	ec = RS4ErrStat_Error;
	fs = RS4FuncStat_Error;

	if (( ! SaveHandle ) || ( ! SaveBuffer ))
	{
		ec = RS4ErrStat_Internal;

		#ifdef DEBUG
		printf( "%s:%04d: NULL Pointer\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	#ifdef DEBUG
	if ( rs->rs_Type != RS4SourceType_String )
	{
		fs = RS4FuncStat_Error;
		ec = RS4ErrStat_Internal;

		#ifdef DEBUG
		printf( "%s:%04d: NULL Pointer\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}
	#endif

	// --

	ss = & rs->rs_Data.rs_String;

	fs = RS4SaveWriteString( & ec, ss->rs_String, strlen( ss->rs_String ));

	if ( fs != RS4FuncStat_Okay )
	{
		// ec allready set

		#ifdef DEBUG
		printf( "%s:%04d: Error writting string\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	// --

	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fs );
}

// --

S32 SaveWriteString( STR buffer UNUSED, U64 length UNUSED )
{
	return( 0 );
}

enum RS4FuncStat RS4SaveSource_File( enum RS4ErrorCode *errcode, RS4FileHeader *fh, STR filename )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Source *rs;

	ec = RS4ErrStat_Error;
	fs = RS4FuncStat_Error;

	// --

	#ifdef DEBUG

	if (( ! fh ) || ( ! filename ))
	{
		ec = RS4ErrStat_Internal;

		#ifdef DEBUG
		printf( "%s:%04d: Error NULL Pointer\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	#endif

	// --

	SaveUsed		= 0;
	SaveHandle		= fopen( filename, "wb" );
	SaveBuffer		= NULL;
	SaveLineBuffer	= NULL;

	// --

	if ( ! SaveHandle )
	{
		ec = RS4ErrStat_FileIO;
		printf( "Can't create '%s' save file\n", filename );
		goto bailout;
	}

	SaveBuffer = malloc( SaveBufferSize );

	if ( ! SaveBuffer )
	{
		ec = RS4ErrStat_OutOfMemory;

		#ifdef DEBUG
		printf( "%s:%04d: Error allocating memory (%d Bytes)\n", __FILE__, __LINE__, SaveBufferSize );
		#endif

		goto bailout;
	}

	SaveLineBuffer = malloc( LineBufferSize );

	if ( ! SaveLineBuffer )
	{
		ec = RS4ErrStat_OutOfMemory;

		#ifdef DEBUG
		printf( "%s:%04d: Error allocating memory (%d Bytes)\n", __FILE__, __LINE__, LineBufferSize );
		#endif

		goto bailout;
	}

	// --

	sprintf( SaveLineBuffer,
		"\n"
		";\n"
		"; Disassembled with ReSrc4 v%d.%d (%s)\n"
		"; Written by Rene W. Olsen (c) 2014-%d\n"
		";\n"
		"; Filename: %s (%" PRId64 " bytes)\n"
		"; MD5 Sum: $%s\n"
		";\n", 
		VERSION,
		REVISION,
		DATE,
		YEAR,
		fh->rfh_FileName,
		fh->rfh_FileSize,
		fh->rfh_FileMD5
	);

	fs = RS4SaveWriteString( & ec, SaveLineBuffer, strlen( SaveLineBuffer ));

	if ( fs != RS4FuncStat_Okay )
	{
		// ec allready set

		#ifdef DEBUF
		printf( "%s:%04d: Error writting string\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	// --

	fs = RS4ExternalLabel( & ec, fh );

	if ( fs != RS4FuncStat_Okay )
	{
		// ec allready set

		#ifdef DEBUF
		printf( "%s:%04d: Error Writting External Data\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	// --

	fs = AmigaOS3_SaveLibFunc( & ec );

	if ( fs != RS4FuncStat_Okay )
	{
		// ec allready set

		#ifdef DEBUG
		printf( "%s:%04d: Error Writting lvo's\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	// --

	rs = RS4GetHead( & fh->rfh_SourceList );

	while( rs )
	{
		switch( rs->rs_Type )
		{
			case RS4SourceType_Code:
			{
				fs = RS4SaveWrite_Code( & ec, rs );

				if ( fs != RS4FuncStat_Okay )
				{
					// ec allready set

					#ifdef DEBUG
					printf( "%s:%04d: Save Node Failed\n", __FILE__, __LINE__ );
					#endif

					goto bailout;
				}
				break;
			}

			case RS4SourceType_String:
			{
				fs = RS4SaveWrite_String( & ec, rs );

				if ( fs != RS4FuncStat_Okay )
				{
					// ec allready set

					#ifdef DEBUG
					printf( "%s:%04d: Save Node Failed\n", __FILE__, __LINE__ );
					#endif

					goto bailout;
				}
				break;
			}

			default:
			{
				#ifdef DEBUG
				printf( "%s:%04d: Error unknown type (%d)\n", __FILE__, __LINE__, rs->rs_Type );
				#endif
				goto bailout;
			}
		}

		rs = RS4GetNext( rs );
	}

	// --

	fs = RS4SaveFlush( & ec );

	if ( fs != RS4FuncStat_Okay )
	{
		// ec allready set

		#ifdef DEBUF
		printf( "%s:%04d: Save Flush Failed\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	// --

	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

bailout:

	if ( SaveLineBuffer )
	{
		free( SaveLineBuffer );
		SaveLineBuffer = NULL;
	}

	if ( SaveBuffer )
	{
		free( SaveBuffer );
		SaveBuffer = NULL;
	}

	if ( SaveHandle )
	{
		if ( fs != RS4FuncStat_Okay )
		{
			fclose( SaveHandle );
			remove( filename );
		}
		else
		{
			fclose( SaveHandle );

			if ( DoVerbose > 0 )
			{
				printf( "Saved %s (%d bytes)\n", filename, (S32) SavedBytes );
			}
		}

		SaveHandle = NULL;
	}

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fs );
}

// --
