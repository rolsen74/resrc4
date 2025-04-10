
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

#include "ReSrc4.h"

// --

int 					DebugInfo		= 0;		// Add ' $12345678 1122 3344 ' to source lines
int 					ShortOpcodes	= 0;		// 0 = movea.l, 1 = move.l
RS4FileHeader *			RS4FileData		= NULL;

char *					InputFile		= NULL;
char *					OutputFile		= NULL;
char *					ConfigFile		= NULL;

int						DoVerbose		= 1;
int						LabTabs			= 1;
int						OpcodeTabs		= 2;
int						ArgTabs			= 5;
int						AutoAnser		= ANSER_Ask;

char *					LabNames		= "L";		// Label names
char *					ExtNames		= "Ext_";	// External Label names
char *					SecNames		= "Sec_";	// Section Label names

static void myPrintErr( enum RS4ErrorCode ec )
{
	if (( ec < RS4ErrStat_Last ) && ( ErrorStrings[ec] ))
	{
		printf( "Error : #%d = %s\n", ec, ErrorStrings[ec] );
	}
	else
	{
		printf( "Error : #%d ??\n", ec );
	}
}

// --

int main( int argc, char *argv[] )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;

	if ( argc < 2 )
	{
		ec = RS4ErrStat_Okay;
		RS4PrintUsage();
		goto bailout;
	}

	fs = RS4ParseArgs( & ec, argc, argv );

	if ( fs != RS4FuncStat_Okay )
	{
		goto bailout;
	}

	if ( DoVerbose > 1 )
	{
		printf( "11 - Load and Parse Hunk File\n" );
	}

	RS4FileData = RS4LoadExe( & ec, InputFile );

	if ( ! RS4FileData )
	{
		goto bailout;
	}

	if ( DoVerbose > 1 )
	{
		printf( "22 - Load and Parse Config\n" );
	}

	fs = RS4ParseConfig_File( & ec, RS4FileData );

	if ( fs != RS4FuncStat_Okay )
	{
		goto bailout;
	}

	if ( DoVerbose > 1 )
	{
		printf( "33 - Trace Memory\n" );
	}

	fs = RS4Trace_File( & ec, RS4FileData );

	if ( fs != RS4FuncStat_Okay )
	{
		goto bailout;
	}

	if ( DoVerbose > 1 )
	{
		printf( "44 - Label Adjust\n" );
	}

	fs = RS4LabelMagic_File( & ec, RS4FileData );

	if ( fs != RS4FuncStat_Okay )
	{
		goto bailout;
	}

	if ( DoVerbose > 1 )
	{
		printf( "55 - Build Source\n" );
	}

	fs = RS4BuildSource_File( & ec, RS4FileData );

	if ( fs != RS4FuncStat_Okay )
	{
		goto bailout;
	}

	if ( DoVerbose > 1 )
	{
		printf( "66 - Save Source\n" );
	}

	fs = RS4SaveSource_File( & ec, RS4FileData, OutputFile );

	if ( fs != RS4FuncStat_Okay )
	{
		goto bailout;
	}

	if ( DoVerbose > 1 )
	{
		printf( "77 - Done Yay\n" );
	}

bailout:

	if ( ec != RS4ErrStat_Okay )
	{
		myPrintErr( ec );
	}

	if ( RS4FileData )
	{
		RS4FreeFile( NULL, RS4FileData );
		RS4FileData = NULL;
	}

	if ( ConfigFile )
	{
		free( ConfigFile );
	}

	if ( OutputFile )
	{
		free( OutputFile );
	}

	if ( InputFile )
	{
		free( InputFile );
	}

	return( 0 );
}
