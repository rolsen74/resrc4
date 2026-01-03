
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

// --

S32 				DebugInfo		= 0;		// Add ' $12345678 1122 3344 ' to source lines
S32 				ShortOpcodes	= 0;		// 0 = movea.l, 1 = move.l
RS4FileHeader *		RS4FileData		= NULL;

STR 				InputFile		= NULL;
STR 				OutputFile		= NULL;
STR 				ConfigFile		= NULL;

S32					DoVerbose		= 1;
S32					AutoAnser		= ANSER_Ask;
S32					Tabs_Label		= 2;
S32					Tabs_Opcode		= 2;
S32					Tabs_Arg		= 5;
S32					Sec_Split		= FALSE;		// Split output file into many
//S32				Sec_xRef		= FALSE;		// add xRef too sections
S32					Sec_xDef		= FALSE;		// add xDef too sections

STR 				LabNames		= "L";		// Label names
STR 				ExtNames		= "Ext_";	// External Label names
STR 				SecNames		= "Sec_";	// Section Label names

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

int main( S32 argc, STR argv[] )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;

	if ( argc < 2 )
	{
		ec = RS4ErrStat_Okay;
		RS4PrintUsage();
		goto bailout;
	}

	ERR_CHK( RS4ParseArgs( & ec, argc, argv ))

	if ( DoVerbose > 0 )
	{
		printf( "11 - Load and Parse Hunk File\n" ); fflush(stdout);
	}

	ERR_CHK( RS4LoadExe( & ec, & RS4FileData, InputFile ))

	if ( ! RS4FileData )
	{
		goto bailout;
	}

	if ( DoVerbose > 0 )
	{
		printf( "22 - Load and Parse Config\n" ); fflush(stdout);
	}

	ERR_CHK( RS4ParseConfig_File( & ec, RS4FileData ))

	if ( DoVerbose > 0 )
	{
		printf( "33 - Trace Memory\n" ); fflush(stdout);
	}

	ERR_CHK( RS4Trace_File( & ec, RS4FileData ))

	if ( DoVerbose > 0 )
	{
		printf( "44 - Label Adjust\n" ); fflush(stdout);
	}

	ERR_CHK( RS4LabelMagic_File( & ec, RS4FileData ))

	if ( DoVerbose > 0 )
	{
		printf( "55 - Build Source\n" ); fflush(stdout);
	}

	ERR_CHK( RS4BuildSource_File( & ec, RS4FileData ))

	if ( DoVerbose > 0 )
	{
		printf( "66 - Save Source\n" ); fflush(stdout);
	}

	ERR_CHK( RS4SaveSource_File( & ec, RS4FileData, OutputFile ))

	if ( DoVerbose > 0 )
	{
		printf( "77 - Done Yay\n" ); fflush(stdout);
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
