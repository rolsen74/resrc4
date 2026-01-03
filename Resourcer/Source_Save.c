 
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

#include "Resourcer/ReSrc4.h"

// --

#define SaveBufferSize		1024 * 16
#define LineBufferSize		1024 * 1

struct Intern
{
	enum RS4ErrorCode		ec;
	enum RS4FuncStat		fs;
	RS4FileHeader *			fh;
	STR						FileName;
	U64						SaveUsed;
	U64						SavedBytes;
	MEM 					SaveBuffer;
	FILE *					SaveHandle;
	STR 					SaveLineBuffer;
};

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

static S32 __Buf_Flush( struct Intern *in )
{
U64 written;
S32 error;

	error = TRUE;

	if (( ! in->SaveHandle ) || ( ! in->SaveBuffer ))
	{
		in->ec = RS4ErrStat_Internal;

		#ifdef DEBUG
		printf( "%s:%04d: NULL Pointer\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	if ( ! in->SaveUsed )
	{
		in->ec = RS4ErrStat_Internal;

		#ifdef DEBUG
		printf( "%s:%04d: Not Used\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	written = fwrite( in->SaveBuffer, 1, in->SaveUsed, in->SaveHandle );

	if ( written != in->SaveUsed )
	{
		in->ec = RS4ErrStat_FileIO;

		#ifdef DEBUG
		printf( "%s:%04d: Error writting data\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	in->SavedBytes += in->SaveUsed;
	in->SaveUsed = 0;

	error = FALSE;

bailout:

	return( error );
}

// --

S32 __Buf_WriteString( struct Intern *in, STR buffer, U64 length )
{
S32 error;
U64 len;

	error = TRUE;

	if (( ! in->SaveHandle ) || ( ! in->SaveBuffer ))
	{
		in->ec = RS4ErrStat_Internal;

		#ifdef DEBUG
		printf( "%s:%04d: NULL Pointer\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	while( length > 0 )
	{
		len = MIN( length, SaveBufferSize - in->SaveUsed );

		if ( len <= 0 )
		{
			break;
		}

		memcpy( & in->SaveBuffer[ in->SaveUsed ], buffer, len );

		in->SaveUsed += len;
		buffer += len;
		length -= len;

		if ( SaveBufferSize <= in->SaveUsed )
		{
			if ( __Buf_Flush( in ))
			{
				#ifdef DEBUG
				printf( "%s:%04d: Save Flush Failed\n", __FILE__, __LINE__ );
				#endif

				goto bailout;
			}
		}
	}

	error = FALSE;

bailout:

	return( error );
}

// --

static S32 __Line_Code( struct Intern *in, RS4Source *rs )
{
struct _SrcCode *sc;
RS4Label *rl;
char labname[64];
char adrname[64];
S32 error;
S32 len;
S32 oo;

	error = TRUE;

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

	sprintf( in->SaveLineBuffer, "%s", labname );

	// --

	if (( *sc->rs_Opcode ) || ( *sc->rs_Argument ) || (( DebugInfo ) && ( sc->rs_Address )))
	{
		_GetStringInfo( labname, NULL, & len );

		oo = Tabs_Label * 8;

		if ( len >= oo )
		{
			strcat( in->SaveLineBuffer, "\n" );
			len = 0;
		}

		while( len < oo )
		{
			strcat( in->SaveLineBuffer, "\t" );
			oo -= 8;
		}
	}

	// --

	strcat( in->SaveLineBuffer, sc->rs_Opcode );

	if (( *sc->rs_Argument ) || (( DebugInfo ) && ( sc->rs_Address )))
	{
		_GetStringInfo( sc->rs_Opcode, NULL, & len );

		oo = Tabs_Opcode * 8;

		if ( len >= oo )
		{
			strcat( in->SaveLineBuffer, " " );
		}

		while( len < oo )
		{
			strcat( in->SaveLineBuffer, "\t" );
			oo -= 8;
		}

		strcat( in->SaveLineBuffer, sc->rs_Argument );
	}

	// --

	if (( DebugInfo ) && ( sc->rs_Address ))
	{
		_GetStringInfo( sc->rs_Argument, NULL, & len );

		oo = Tabs_Arg * 8;

		if ( len >= oo )
		{
			strcat( in->SaveLineBuffer, " " );
		}

		while( len < oo )
		{
			strcat( in->SaveLineBuffer, "\t" );
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

		strcat( in->SaveLineBuffer, adrname );
	}

	// --

	strcat( in->SaveLineBuffer, "\n" );

	if ( __Buf_WriteString( in, in->SaveLineBuffer, strlen( in->SaveLineBuffer )))
	{
		#ifdef DEBUG
		printf( "%s:%04d: Error : __Line : String :\n", __FILE__, __LINE__ );
		#endif
 
		goto bailout;
	}

	// --

	error = FALSE;

bailout:

	return( error );
}

// --

static S32 __Line_String( struct Intern *in, RS4Source *rs )
{
struct _SrcString *ss;
S32 error;

	error = TRUE;

	// --

	ss = & rs->rs_Data.rs_String;

	if ( __Buf_WriteString( in, ss->rs_String, strlen( ss->rs_String )))
	{
		#ifdef DEBUG
		printf( "%s:%04d: Error : __Line : String :\n", __FILE__, __LINE__ );
		#endif
 
		goto bailout;
	}

	// --

	error = FALSE;

bailout:

	return( error );
}

// --

static S32 __Line( struct Intern *in, RS4Source *rs )
{
S32 error;

	error = TRUE;

	switch( rs->rs_Type )
	{
		case RS4SourceType_Code:
		{
			if ( __Line_Code( in, rs ))
			{
				#ifdef DEBUG
				printf( "%s:%04d: Error : __Line : Code :\n", __FILE__, __LINE__ );
				#endif

				goto bailout;
			}
			break;
		}

		case RS4SourceType_String:
		{
			if ( __Line_String( in, rs ))
			{
				#ifdef DEBUG
				printf( "%s:%04d: Error : __Line : String :\n", __FILE__, __LINE__ );
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

	error = FALSE;

bailout:

	return( error );
}

// --

static S32 __Header( struct Intern *in )
{
RS4FileHeader *fh;
S32 error;
STR ftype;

	error = TRUE;

	fh = in->fh;

	// --

	while( TRUE )
	{
		#ifdef SUPPORT_FHR

		if ( fh->rfh_FileType == RS4FileType_FHR )
		{
			ftype = "FHR";
			break;
		}

		#endif

		#ifdef SUPPORT_HUNK

		if ( fh->rfh_FileType == RS4FileType_Hunk )
		{
			ftype = "Hunk";
			break;
		}

		#endif

		ftype = "???";
		break;
	}

	sprintf( in->SaveLineBuffer,
		"\n"
		";\n"
		"; Disassembled with ReSrc4 v%d.%d (%s)\n"
		";\n"
		"; Size .. : %" PRId64 " bytes\n"
		"; Type .. : %s\n"
		"; Name .. : '%s'\n"
		"; MD5 ... : $%s\n"
		";\n", 
		VERSION,
		REVISION,
		DATE,
		fh->rfh_FileSize,
		ftype,
		fh->rfh_FileName,
		fh->rfh_FileMD5
	);

	if ( __Buf_WriteString( in, in->SaveLineBuffer, strlen( in->SaveLineBuffer )))
	{
		#ifdef DEBUG
		printf( "%s:%04d: Error : __Header : 1 :\n", __FILE__, __LINE__ );
		#endif
		goto bailout;
	}

	// --

	error = FALSE;

bailout:

	return( error );
}

// --
// One huge file

static S32 __Single( struct Intern *in )
{
RS4FileSection *fs;
RS4FileHeader *fh;
RS4Source *rs;
RS4Label *rl;
S32 error;
S32 secnr;
S32 len;

	error = TRUE;

	fh = in->fh;

	// --

	in->SavedBytes = 0;
	in->SaveHandle = fopen( in->FileName, "wb" );

	if ( ! in->SaveHandle )
	{
		#ifdef DEBUG
		printf( "%s:%04d: Error : __Single : 1 :\n", __FILE__, __LINE__ );
		#endif
		in->ec = RS4ErrStat_FileIO;
		printf( "Can't create '%s' save file\n", in->FileName );
		goto bailout;
	}

	if ( __Header( in ))
	{
		#ifdef DEBUG
		printf( "%s:%04d: Error : __Single : 2 :\n", __FILE__, __LINE__ );
		#endif
		goto bailout;
	}

	// --

	rl = RS4GetHead( & fh->rfh_ExtLabelList );

	if ( rl )
	{
		sprintf( in->SaveLineBuffer, "\n; External\n" );

		if ( __Buf_WriteString( in, in->SaveLineBuffer, strlen( in->SaveLineBuffer )))
		{
			#ifdef DEBUG
			printf( "%s:%04d: Error : __Single : 3 :\n", __FILE__, __LINE__ );
			#endif
			goto bailout;
		}

		// --

		rl = RS4GetHead( & fh->rfh_ExtLabelList );

		while( rl )
		{
			len = strlen( rl->rl_Name );

			if ( len < 8 )
			{
				sprintf( in->SaveLineBuffer, "%s:\t\t\tEQU\t$%08" PRIX64 "\n", rl->rl_Name, rl->rl_Offset );
			}
			else if ( len < 16 )
			{
				sprintf( in->SaveLineBuffer, "%s:\t\tEQU\t$%08" PRIX64 "\n", rl->rl_Name, rl->rl_Offset );
			}
			else if ( len < 24 )
			{
				sprintf( in->SaveLineBuffer, "%s:\tEQU\t$%08" PRIX64 "\n", rl->rl_Name, rl->rl_Offset );
			}
			else
			{
				sprintf( in->SaveLineBuffer, "%s: EQU\t$%08" PRIX64 "\n", rl->rl_Name, rl->rl_Offset );
			}

			if ( __Buf_WriteString( in, in->SaveLineBuffer, strlen( in->SaveLineBuffer )))
			{
				#ifdef DEBUG
				printf( "%s:%04d: Error : __Single : 3 :\n", __FILE__, __LINE__ );
				#endif
				goto bailout;
			}

			rl = RS4GetNext( rl );
		}
	}

	// --

	#ifdef SUPPORT_AMIGAOS3
	if ( AmigaOS3_SaveLibFunc( in, in->SaveLineBuffer ))
	{
		#ifdef DEBUG
		printf( "%s:%04d: Error : __Single : 4 :\n", __FILE__, __LINE__ );
		#endif
		goto bailout;
	}
	#endif

	// --

	for( secnr=0 ; secnr < fh->rfh_SecArraySize ; secnr++ )
	{
		fs = fh->rfh_SecArray[secnr].rsi_Section;

		rs = RS4GetHead( & fs->rfs_SourceList );

		while( rs )
		{
			if ( __Line( in, rs ))
			{
				#ifdef DEBUG
				printf( "%s:%04d: Error : __Single : 3 :\n", __FILE__, __LINE__ );
				#endif

				goto bailout;
			}

			// Next Source (Line)
			rs = RS4GetNext( rs );
		}
	}

	// --

	if ( __Buf_Flush( in ))
	{
		#ifdef DEBUG
		printf( "%s:%04d: Error : __Single : 4 :\n", __FILE__, __LINE__ );
		#endif
		goto bailout;
	}

	if ( DoVerbose > 0 )
	{
		printf( "Saved '%s' (%" PRIu64 " bytes)\n", in->FileName, in->SavedBytes );
	}

	// Let parent close file

	error = FALSE;

bailout:

	return( error );
}

// --
// Split Section

static S32 __Split_Sec( struct Intern *in, RS4FileSection *fs, S32 secnr )
{
RS4Source *rs;
STR secname;
S32 error;
STR name;
int stat;

	error = TRUE;

	/**/ if ( fs->rfs_SecType == RS4ST_Code )
	{
		secname = "_Code";
	}
	else if ( fs->rfs_SecType == RS4ST_Data )
	{
		secname = "_Data";
	}
	else if ( fs->rfs_SecType == RS4ST_BSS )
	{
		secname = "_BSS";
	}
	else
	{
		secname = "";
	}

	stat = asprintf( & name, "%s_SEC_%02d%s.s", in->FileName, secnr, secname );

	if (( stat < 0 ) || ( ! ConfigFile ))
	{
		in->ec = RS4ErrStat_OutOfMemory;

		#ifdef DEBUG
		printf( "%s:%04d: Error allocating memory\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	in->SavedBytes = 0;
	in->SaveHandle = fopen( name, "wb" );

	if ( ! in->SaveHandle )
	{
		in->ec = RS4ErrStat_FileIO;

		#ifdef DEBUG
		printf( "%s:%04d: Error : __Single : 1 :\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	if ( ! in->SaveHandle )
	{
		in->ec = RS4ErrStat_FileIO;

		#ifdef DEBUG
		printf( "%s:%04d: Error : __Single : 1 :\n", __FILE__, __LINE__ );
		#endif

		printf( "Can't create '%s' save file\n", name );
		goto bailout;
	}

	if ( __Header( in ))
	{
		#ifdef DEBUG
		printf( "%s:%04d: Error : __Single : 2 :\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	// --

	rs = RS4GetHead( & fs->rfs_SourceList );

	while( rs )
	{
		if ( __Line( in, rs ))
		{
			#ifdef DEBUG
			printf( "%s:%04d: Error : __Single : 3 :\n", __FILE__, __LINE__ );
			#endif

			goto bailout;
		}

		// Next Source (Line)
		rs = RS4GetNext( rs );
	}

	// --

	if ( __Buf_Flush( in ))
	{
		#ifdef DEBUG
		printf( "%s:%04d: Error : __Single : 4 :\n", __FILE__, __LINE__ );
		#endif
		goto bailout;
	}

	fclose( in->SaveHandle );
	in->SaveHandle = NULL;

	if ( DoVerbose > 0 )
	{
		printf( "Saved '%s' (%" PRIu64 " bytes)\n", name, in->SavedBytes );
	}

	// --

	// Free mem
	free( name );
	name = NULL;

	error = FALSE;

bailout:

	return( error );
}

// --
// Split files

static S32 __Split( struct Intern *in )
{
RS4FileSection *fs;
RS4FileHeader *fh;
RS4Label *rl;
S32 secnr;
S32 error;
STR name;
S32 stat;
S32 len;

	error = TRUE;

	name = NULL;

	fh = in->fh;

	// --
	// Handle Includes

	stat = asprintf( & name, "%s.i", in->FileName );

	if (( stat < 0 ) || ( ! name ))
	{
		in->ec = RS4ErrStat_OutOfMemory;

		#ifdef DEBUG
		printf( "%s:%04d: Error allocating memory\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	in->SavedBytes = 0;
	in->SaveHandle = fopen( name, "wb" );

	if ( ! in->SaveHandle )
	{
		in->ec = RS4ErrStat_FileIO;

		#ifdef DEBUG
		printf( "%s:%04d: Error : __Single : 1 :\n", __FILE__, __LINE__ );
		#endif

		printf( "Can't create '%s' save file\n", name );
		goto bailout;
	}

	if ( __Header( in ))
	{
		#ifdef DEBUG
		printf( "%s:%04d: Error : __Single : 2 :\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	// --

	rl = RS4GetHead( & fh->rfh_ExtLabelList );

	if ( rl )
	{
		sprintf( in->SaveLineBuffer, "\n; External\n" );

		if ( __Buf_WriteString( in, in->SaveLineBuffer, strlen( in->SaveLineBuffer )))
		{
			#ifdef DEBUG
			printf( "%s:%04d: Error : __Single : 3 :\n", __FILE__, __LINE__ );
			#endif
			goto bailout;
		}

		// --

		rl = RS4GetHead( & fh->rfh_ExtLabelList );

		while( rl )
		{
			len = strlen( rl->rl_Name );

			if ( len < 8 )
			{
				sprintf( in->SaveLineBuffer, "%s:\t\t\tEQU\t$%08" PRIX64 "\n", rl->rl_Name, rl->rl_Offset );
			}
			else if ( len < 16 )
			{
				sprintf( in->SaveLineBuffer, "%s:\t\tEQU\t$%08" PRIX64 "\n", rl->rl_Name, rl->rl_Offset );
			}
			else if ( len < 24 )
			{
				sprintf( in->SaveLineBuffer, "%s:\tEQU\t$%08" PRIX64 "\n", rl->rl_Name, rl->rl_Offset );
			}
			else
			{
				sprintf( in->SaveLineBuffer, "%s: EQU\t$%08" PRIX64 "\n", rl->rl_Name, rl->rl_Offset );
			}

			if ( __Buf_WriteString( in, in->SaveLineBuffer, strlen( in->SaveLineBuffer )))
			{
				#ifdef DEBUG
				printf( "%s:%04d: Error : __Single : 3 :\n", __FILE__, __LINE__ );
				#endif
				goto bailout;
			}

			rl = RS4GetNext( rl );
		}
	}

	// --

	#ifdef SUPPORT_AMIGAOS3
	if ( AmigaOS3_SaveLibFunc( in, in->SaveLineBuffer ))
	{
		#ifdef DEBUG
		printf( "%s:%04d: Error : __Single : 4 :\n", __FILE__, __LINE__ );
		#endif
		goto bailout;
	}
	#endif

	// --
	// Close Include file

	if ( __Buf_Flush( in ))
	{
		#ifdef DEBUG
		printf( "%s:%04d: Error : __Single : 5 :\n", __FILE__, __LINE__ );
		#endif
		goto bailout;
	}

	fclose( in->SaveHandle );
	in->SaveHandle = NULL;

	if ( DoVerbose > 0 )
	{
		printf( "Saved '%s' (%" PRIu64 " bytes)\n", name, in->SavedBytes );
	}

	free( name );
	name = NULL;

	// --
	// Handle Sections

	for( secnr=0 ; secnr < fh->rfh_SecArraySize ; secnr++ )
	{
		fs = fh->rfh_SecArray[secnr].rsi_Section;

		if ( __Split_Sec( in, fs, secnr ))
		{
			#ifdef DEBUG
			printf( "%s:%04d: Error : __Single : 2 :\n", __FILE__, __LINE__ );
			#endif

			goto bailout;
		}
	}

	error = FALSE;

bailout:

	if ( name )
	{
		free( name );
	}

	return( error );
}

// --

enum RS4FuncStat RS4SaveSource_File( enum RS4ErrorCode *errcode, RS4FileHeader *fh, STR filename )
{
struct Intern in;

	memset( & in, 0, sizeof( in ));

	in.FileName = filename;
	in.ec = RS4ErrStat_Okay;
	in.fs = RS4FuncStat_Okay;
	in.fh = fh;

	// --

	in.SaveBuffer = malloc( SaveBufferSize );

	if ( ! in.SaveBuffer )
	{
		in.ec = RS4ErrStat_OutOfMemory;

		#ifdef DEBUG
		printf( "%s:%04d: Error allocating memory (%d Bytes)\n", __FILE__, __LINE__, SaveBufferSize );
		#endif

		goto bailout;
	}

	in.SaveLineBuffer = malloc( LineBufferSize );

	if ( ! in.SaveLineBuffer )
	{
		in.ec = RS4ErrStat_OutOfMemory;

		#ifdef DEBUG
		printf( "%s:%04d: Error allocating memory (%d Bytes)\n", __FILE__, __LINE__, LineBufferSize );
		#endif

		goto bailout;
	}

	// --

	if ( Sec_Split )
	{
		if ( __Split( & in ))
		{
			#ifdef DEBUG
			printf( "%s:%04d: Error : __Split :\n", __FILE__, __LINE__ );
			#endif
		}
	}
	else
	{
		if ( __Single( & in ))
		{
			#ifdef DEBUG
			printf( "%s:%04d: Error : __Single :\n", __FILE__, __LINE__ );
			#endif
		}
	}

	// --

bailout:

	if ( in.SaveHandle )
	{
		fclose( in.SaveHandle );
	}

	if ( in.SaveLineBuffer )
	{
		free( in.SaveLineBuffer );
	}

	if ( in.SaveBuffer )
	{
		free( in.SaveBuffer );
	}

	if ( errcode )
	{
		*errcode = in.ec;
	}

	return( in.fs );
}

// --
