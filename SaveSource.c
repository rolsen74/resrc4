 
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

#define SaveBufferSize		4096
#define LineBufferSize		1024

static FILE *				SaveHandle		= NULL;
static size_t				SaveUsed		= 0;
static uint8_t *			SaveBuffer		= NULL;
char *						SaveLineBuffer	= NULL;
static size_t				SavedBytes		= 0;

// --

static int SaveFlush( void )
{
size_t written;
int error;

	if (( SaveHandle == NULL ) || ( SaveBuffer == NULL ))
	{
		printf( "%s:%04d: NULL Pointer\n", __FILE__, __LINE__ );
		error = true;
		goto bailout;
	}

	if ( SaveUsed == 0 )
	{
		error = false;
		goto bailout;
	}

	written = fwrite( SaveBuffer, 1, SaveUsed, SaveHandle );

	if ( written != SaveUsed )
	{
		printf( "%s:%04d: Error writting data\n", __FILE__, __LINE__ );
		error = true;
		goto bailout;
	}

	SavedBytes += SaveUsed;

	SaveUsed = 0;

	error = false;

bailout:

	return( error );
}

// --

int SaveWriteString( char *buffer, size_t length )
{
size_t len;
int error;

	error = true;

	if (( SaveHandle == NULL ) || ( SaveBuffer == NULL ))
	{
		printf( "%s:%04d: NULL Pointer\n", __FILE__, __LINE__ );
		error = true;
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
			if ( SaveFlush() )
			{
				printf( "%s:%04d: Save Flush Failed\n", __FILE__, __LINE__ );
				goto bailout;
			}
		}
	}

	error = false;

bailout:

	return( error );
}

// --

static int myExternalLabel( struct HunkStruct *hs )
{
struct HunkLabel *hl;
int error;
int len;

	// --

	hl = myGetHead( & hs->hs_ExtLabelList );

	if ( hl == NULL )
	{
		error = false;
		goto bailout;
	}

	// --

	sprintf( SaveLineBuffer, "\n; External\n" );

	error = SaveWriteString( SaveLineBuffer, strlen( SaveLineBuffer ));

	if ( error )
	{
		printf( "%s:%04d: Error Writting External Data\n", __FILE__, __LINE__ );
		goto bailout;
	}

	// --

	while( hl )
	{
		len = strlen( hl->hl_Label_Name );

		if ( len < 8 )
		{
			sprintf( SaveLineBuffer, "%s:\t\t\tEQU\t$%04X\n", hl->hl_Label_Name, hl->hl_Label_Offset );
		}
		else if ( len < 16 )
		{
			sprintf( SaveLineBuffer, "%s:\t\tEQU\t$%04X\n", hl->hl_Label_Name, hl->hl_Label_Offset );
		}
		else if ( len < 24 )
		{
			sprintf( SaveLineBuffer, "%s:\tEQU\t$%04X\n", hl->hl_Label_Name, hl->hl_Label_Offset );
		}
		else
		{
			sprintf( SaveLineBuffer, "%s: EQU\t$%04X\n", hl->hl_Label_Name, hl->hl_Label_Offset );
		}

		error = SaveWriteString( SaveLineBuffer, strlen( SaveLineBuffer ));

		if ( error )
		{
			printf( "%s:%04d: Error Writting External data\n", __FILE__, __LINE__ );
			goto bailout;
		}

		hl = myGetNext( hl );
	}

	// --

	#if 0
	sprintf( SaveLineBuffer, "\n" );

	error = SaveWriteString( SaveLineBuffer, strlen( SaveLineBuffer ));

	if ( error )
	{
		printf( "%s:%04d: Error writting external label\n", __FILE__, __LINE__ );
		goto bailout;
	}
	#endif

	// --

	error = false;

bailout:

	return( error );
}

// --

static int SaveWrite( struct SourceNode *sn )
{
struct HunkLabel *hl;
char labname[ MAX_LabelName + 2 ];
char adrname[256];
int error;
int len;
int oo;

	if (( SaveHandle == NULL ) || ( SaveBuffer == NULL ))
	{
		printf( "%s:%04d: NULL Pointer\n", __FILE__, __LINE__ );
		error = true;
		goto bailout;
	}

	// -- Label

	hl = sn->sn_Label;

	if (( hl ) && ( hl->hl_Parent == NULL ) && ( hl->hl_Label_Name[0] ))
	{
		sprintf( labname, "%s:", hl->hl_Label_Name );
	}
	else
	{
		labname[0] = 0;
	}

	sprintf( SaveLineBuffer, "%s", labname );

	// --

	if (( *sn->sn_Opcode ) || ( *sn->sn_Argument ) || (( DebugInfo ) && ( sn->sn_Address )))
	{
		len = strlen( labname );

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

	strcat( SaveLineBuffer, sn->sn_Opcode );

	if (( *sn->sn_Argument ) || (( DebugInfo ) && ( sn->sn_Address )))
	{
		len = strlen( sn->sn_Opcode );

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

		strcat( SaveLineBuffer, sn->sn_Argument );
	}

	// --

	if (( DebugInfo ) && ( sn->sn_Address ))
	{
		len = strlen( sn->sn_Argument );

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

		if ( sn->sn_Comment )
		{
			sprintf( adrname, "; $%08x %s", sn->sn_Address, sn->sn_Comment );
		}
		else
		{
			sprintf( adrname, "; $%08x", sn->sn_Address );
		}

		strcat( SaveLineBuffer, adrname );
	}

	// --

	strcat( SaveLineBuffer, "\n" );

	error = SaveWriteString( SaveLineBuffer, strlen( SaveLineBuffer ));

	if ( error )
	{
		printf( "%s:%04d: Error writting string\n", __FILE__, __LINE__ );
		goto bailout;
	}

	// --

bailout:

	return( error );
}

// --

struct mySaveStruct
{
	char *		Name;
	int			(*Func)();
};

static struct mySaveStruct mySave[] =
{
	{ "Amigaguide",			Save_Lib_Amigaguide },
	{ "ASL",				Save_Lib_Asl },
	{ "Battclock",			Save_Lib_Battclock },
	{ "Battmem",			Save_Lib_Battmem },
	{ "Bullet",				Save_Lib_Bullet },
	{ "Cardres",			Save_Lib_Cardres },
	{ "CIA",				Save_Lib_Cia },
	{ "Colorwheel",			Save_Lib_Colorwheel },
	{ "Commodities",		Save_Lib_Commodities },
//	{ "Console",			Save_Lib_Console },
	{ "Datatypes",			Save_Lib_Datatypes },
	{ "Disk",				Save_Lib_Disk },
	{ "Diskfont",			Save_Lib_Diskfont },
	{ "DOS",				Save_Lib_Dos },
	{ "DTClass",			Save_Lib_DTClass },
	{ "Exec",				Save_Lib_Exec },
	{ "Expansion",			Save_Lib_Expansion },
	{ "Gadtools",			Save_Lib_Gadtools },
	{ "Graphics",			Save_Lib_Graphics },
	{ "Icon",				Save_Lib_Icon },
	{ "IFFParse",			Save_Lib_IFFParse },
//	{ "Input",				Save_Lib_Input },
	{ "Intuition",			Save_Lib_Intuition },
	{ "Keymap",				Save_Lib_Keymap },
	{ "Layers",				Save_Lib_Layers },
	{ "Locale",				Save_Lib_Locale },
	{ "Lowlevel",			Save_Lib_Lowlevel },
	{ "Mathffp",			Save_Lib_Mathffp },
	{ "Mathieeedoubbase",	Save_Lib_Mathieeedoubbas },
	{ "Mathieeedoubtrans",	Save_Lib_Mathieeedoubtrans },
	{ "Mathieeesingbas",	Save_Lib_Mathieeesingbas },
	{ "Mathieeesingtrans",	Save_Lib_Mathieeesingtrans },
	{ "Mathtrans",			Save_Lib_Mathtrans },
	{ "Misc",				Save_Lib_Misc },
	{ "Nonvolatile",		Save_Lib_Nonvolatile },
	{ "Potgo",				Save_Lib_Potgo },
//	{ "Ramdrive",			Save_Lib_Ramdrive },
	{ "Rexxsyslib",			Save_Lib_Rexxsyslib },
//	{ "Timer",				Save_Lib_Timer },
	{ "Translator",			Save_Lib_Translator },
	{ "Utility",			Save_Lib_Utility },
	{ "Workbench",			Save_Lib_Workbench },
	{ NULL, NULL }
};

int SaveSource( struct HunkStruct *hs, char *filename )
{
struct SourceNode *sn;
int error;
int pos;

// printf( "SaveSource\n" );

	error = true;

	SaveBuffer	= NULL;
	SaveLineBuffer	= NULL;
	SaveUsed	= 0;
	SaveHandle	= fopen( filename, "wb" );

	if ( SaveHandle == NULL )
	{
		printf( "Can't create '%s' save file\n", filename );
		goto bailout;
	}

	SaveBuffer = malloc( SaveBufferSize );

	if ( SaveBuffer == NULL )
	{
		printf( "%s:%04d: Error allocating memory (%d Bytes)\n", __FILE__, __LINE__, SaveBufferSize );
		goto bailout;
	}

	SaveLineBuffer = malloc( LineBufferSize );

	if ( SaveLineBuffer == NULL )
	{
		printf( "%s:%04d: Error allocating memory (%d Bytes)\n", __FILE__, __LINE__, LineBufferSize );
		goto bailout;
	}

	// --

	sprintf( SaveLineBuffer,
		"\n"
		";\n"
		"; Disassembled with ReSrc4 v%d.%d (%s)\n"
		"; Written by Rene W. Olsen (c) 2014-%d\n"
		";\n"
		"; Filename: %s (%d bytes)\n"
		"; MD5 Sum: $%s\n"
		";\n", 
		VERSION,
		REVISION,
		DATE,
		YEAR,
		hs->hs_FileName,
		hs->hs_FileSize,
		hs->hs_FileMD5
	);

	if ( SaveWriteString( SaveLineBuffer, strlen( SaveLineBuffer )))
	{
		printf( "%s:%04d: Error writting string\n", __FILE__, __LINE__ );
		goto bailout;
	}

	// --

	if ( myExternalLabel( hs ))
	{
		printf( "%s:%04d: Error Writting External Data\n", __FILE__, __LINE__ );
		goto bailout;
	}

	// --

	pos = 0;

	while( mySave[pos].Name )
	{
		if ( mySave[pos].Func() )
		{
			printf( "%s:%04d: Error Writting %s lvo's\n", __FILE__, __LINE__, mySave[pos].Name );
			goto bailout;
		}
		else
		{
			pos++;
		}
	}

	// --

	sn = myGetHead( & hs->hs_SourceList );

	while( sn )
	{
		if ( SaveWrite( sn ))
		{
			printf( "%s:%04d: Save Node Failed\n", __FILE__, __LINE__ );
			goto bailout;
		}

		sn = myGetNext( sn );
	}

	if ( SaveFlush() )
	{
		printf( "%s:%04d: Save Flush Failed\n", __FILE__, __LINE__ );
		goto bailout;
	}

	error = false;

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
		if ( error )
		{
			fclose( SaveHandle );
			remove( filename );
		}
		else
		{
			fclose( SaveHandle );

			printf( "Saved %s (%ld bytes)\n", filename, SavedBytes );
		}

		SaveHandle = NULL;
	}

	return( error );
}

// --
