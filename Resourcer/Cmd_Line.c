 
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

static enum RS4FuncStat ArgFunc_ArgTabs( STR arg )
{
enum RS4FuncStat fs;
S32 val;

	fs = RS4FuncStat_Error;

	if ( sscanf( arg, "%d", & val ) != 1 )
	{
		printf( "%s:%04d: Scanning Argument\n", __FILE__, __LINE__ );
		goto bailout;
	}

	if (( val < 1 ) || ( 15 < val ))
	{
		printf( "ArgTabs %d is out of range (1-15)\n", val );
		goto bailout;
	}

	ArgTabs = val;

	fs = RS4FuncStat_Okay;

bailout:

	return( fs );
}

// --

static enum RS4FuncStat ArgFunc_OpcodeTabs( STR arg )
{
enum RS4FuncStat fs;
S32 val;

	fs = RS4FuncStat_Error;

	if ( sscanf( arg, "%d", & val ) != 1 )
	{
		printf( "%s:%04d: Scanning Argument\n", __FILE__, __LINE__ );
		goto bailout;
	}

	if (( val < 1 ) || ( 15 < val ))
	{
		printf( "OpcodeTabs %d is out of range (1-15)\n", val );
		goto bailout;
	}

	OpcodeTabs = val;

	fs = RS4FuncStat_Okay;

bailout:

	return( fs );
}

// --

static enum RS4FuncStat ArgFunc_LabTabs( STR arg )
{
enum RS4FuncStat fs;
S32 val;

	fs = RS4FuncStat_Error;

	if ( sscanf( arg, "%d", & val ) != 1 )
	{
		printf( "%s:%04d: Scanning Argument\n", __FILE__, __LINE__ );
		goto bailout;
	}

	if (( val < 1 ) || ( 15 < val ))
	{
		printf( "LabTabs %d is out of range (1-15)\n", val );
		goto bailout;
	}

	LabTabs = val;

	fs = RS4FuncStat_Okay;

bailout:

	return( fs );
}

// --

static enum RS4FuncStat ArgFunc_AutoYes( STR arg UNUSED )
{
enum RS4FuncStat fs;

	fs = RS4FuncStat_Okay;

	AutoAnser = ANSER_Yes;

	return( fs );
}

// --

static enum RS4FuncStat ArgFunc_AutoNo( STR arg UNUSED )
{
enum RS4FuncStat fs;

	fs = RS4FuncStat_Okay;

	AutoAnser = ANSER_No;

	return( fs );
}

// --

static enum RS4FuncStat ArgFunc_DebugInfo( STR arg UNUSED )
{
enum RS4FuncStat fs;

	fs = RS4FuncStat_Okay;

	DebugInfo = 1;

	return( fs );
}

/* -- */

static enum RS4FuncStat ArgFunc_Verbose0( STR arg UNUSED )
{
enum RS4FuncStat fs;

	fs = RS4FuncStat_Okay;

	DoVerbose = 0;	// Off

	return( fs );
}

/* -- */

static enum RS4FuncStat ArgFunc_Verbose1( STR arg UNUSED )
{
enum RS4FuncStat fs;

	fs = RS4FuncStat_Okay;

	DoVerbose = 1;	// Normal

	return( fs );
}

/* -- */

static enum RS4FuncStat ArgFunc_Verbose2( STR arg UNUSED )
{
enum RS4FuncStat fs;

	fs = RS4FuncStat_Okay;

	DoVerbose = 2;	// Extra

	return( fs );
}

/* -- */

static enum RS4FuncStat ArgFunc_Verbose3( STR arg UNUSED )
{
enum RS4FuncStat fs;

	fs = RS4FuncStat_Okay;

	DoVerbose = 3;	// even more

	return( fs );
}

/* -- */

static enum RS4FuncStat ArgFunc_Verbose4( STR arg UNUSED )
{
enum RS4FuncStat fs;

	fs = RS4FuncStat_Okay;

	DoVerbose = 4;	// alot

	return( fs );
}

/* -- */

static enum RS4FuncStat ArgFunc_Short( STR arg UNUSED )
{
enum RS4FuncStat fs;

	fs = RS4FuncStat_Okay;

	ShortOpcodes = 1;

	return( fs );
}

// --

static enum RS4FuncStat ArgFunc_Config( STR arg )
{
enum RS4FuncStat fs;

	fs = RS4FuncStat_Error;

	if ( ConfigFile )
	{
		free( ConfigFile );
	}

	ConfigFile = RS4Strdup( arg );

	if ( ConfigFile == NULL )
	{
		printf( "%s:%04d: Error copying string\n", __FILE__, __LINE__ );
		goto bailout;
	}

	if ( DoVerbose > 2 )
	{
		printf( "Config File : %s\n", ConfigFile );
	}

	fs = RS4FuncStat_Okay;

bailout:

	return( fs );
}

// --

static enum RS4FuncStat ArgFunc_Input( STR arg )
{
enum RS4FuncStat fs;

	fs = RS4FuncStat_Error;

	if ( InputFile )
	{
		free( InputFile );
	}

	InputFile = RS4Strdup( arg );

	if ( InputFile == NULL )
	{
		printf( "%s:%04d: Error copying string\n", __FILE__, __LINE__ );
		goto bailout;
	}

	if ( DoVerbose > 2 )
	{
		printf( "Input File : %s\n", InputFile );
	}

	fs = RS4FuncStat_Okay;

bailout:

	return( fs );
}

// --

static enum RS4FuncStat ArgFunc_Output( STR arg )
{
enum RS4FuncStat fs;

	fs = RS4FuncStat_Error;

	if ( OutputFile )
	{
		free( OutputFile );
	}

	OutputFile = RS4Strdup( arg );

	if ( OutputFile == NULL )
	{
		printf( "%s:%04d: Error copying string\n", __FILE__, __LINE__ );
		goto bailout;
	}

	if ( DoVerbose > 2 )
	{
		printf( "Output File : %s\n", OutputFile );
	}

	fs = RS4FuncStat_Okay;

bailout:

	return( fs );
}

/* -- */

#define ARGTYPES	3

struct myArgs
{
	CSTR 		Name1;
	CSTR 		Name2;
	S32					Type;		// 0 = verbose, 1 = config, 2 = Normal
	S32					Options;
	enum RS4FuncStat	(*Function)( STR arg );
};

// -- Early Options before Config file
struct myArgs myOptions[] =
{
// Normal Options
{ "-i", "--input",			2, 1, ArgFunc_Input },
{ "-o", "--output",			2, 1, ArgFunc_Output },
{ "-y", "--yes",			2, 0, ArgFunc_AutoYes },
{ "-n", "--no",				2, 0, ArgFunc_AutoNo },
{ NULL, "--short",			2, 0, ArgFunc_Short },
{ NULL, "--debuginfo",		2, 0, ArgFunc_DebugInfo },
{ NULL, "--labtabs",		2, 1, ArgFunc_LabTabs },
{ NULL, "--opcodetabs",		2, 1, ArgFunc_OpcodeTabs },
{ NULL, "--argtabs",		2, 1, ArgFunc_ArgTabs },

// Config Options
{ "-c", "--config",			1, 1, ArgFunc_Config },

// Verbose Options
{ "-v", "--verbose",		0, 0, ArgFunc_Verbose2 },
{ "-v0", NULL,				0, 0, ArgFunc_Verbose0 },
{ "-v1", NULL,				0, 0, ArgFunc_Verbose1 },
{ "-v2", NULL,				0, 0, ArgFunc_Verbose2 },
{ "-v3", NULL,				0, 0, ArgFunc_Verbose3 },
{ "-v4", NULL,				0, 0, ArgFunc_Verbose4 },
{ NULL,	NULL,				0, 0, NULL }
};

// --

void RS4PrintUsage( void )
{
	printf( "\n" );
	printf( "ReSrc4 v%d.%d (%s)\n", VERSION, REVISION, DATE );
	printf( "\n" );
	printf( "Usage: ReSrc4 [options]\n" );
	printf( "\n" );
	printf( "Options:\n" );
	printf( "\n" );
	printf( "  -i, --input          Input File (Exe File)\n" );
	printf( "  -o, --output         Output File (Asm File)\n" );
	printf( "  -c, --config         Config File (Config File)\n" );
	printf( "  -y, --yes            Auto select yes\n" );
	printf( "  -n, --no             Auto select no\n" );
	printf( "      --short          Output short opcodes ( movea.l -> move.l )\n" );
	printf( "      --debuginfo      Will Append Address and Memory Codes\n" );
	printf( "      --labtabs        Number of Tabs after labels (Default 1)\n" );
	printf( "      --opcodetabs     Number of Tabs after opcodes (Default 2)\n" );
	printf( "      --argtabs        Number of Tabs after arguments (Default 5)\n" );
	printf( "  -v  --verbose        Print extra information\n" );
	printf( "\n" );
}

/* -- */

static S32 _ParseList( struct myArgs *args, S32 argc, STR argv[], S32 type )
{
struct myArgs *arg;
S32 error;
S32 stat;
S32 skip;
S32 pos;
S32 cnt;

	error = true;

	for( cnt=1 ; cnt<argc ; cnt++ )
	{
		pos = 0;

		skip = FALSE;

		while( true )
		{
			arg = & args[pos];

			// End of List?
			if (( ! arg->Name1 ) && ( ! arg->Name2 ))
			{
				break;
			}

			// Found Argument?
			if ((( arg->Name1 ) && ( ! strcmp( arg->Name1, argv[cnt] )))
			||  (( arg->Name2 ) && ( ! strcmp( arg->Name2, argv[cnt] ))))
			{
				skip = TRUE;

				if ( arg->Type == type )
				{
					if ( cnt+arg->Options >= argc )
					{
						printf( "\nMissing argument for %s\n", argv[cnt] );
						RS4PrintUsage();
						goto bailout;
					}					

					stat = arg->Function( argv[cnt+1] );

					if ( stat )
					{
						goto bailout;
					}
				}

				cnt += arg->Options;
				break;
			}

			pos++;
		}

		if ( ! skip )
		{
			printf( "\nUnknown argument : %s\n", argv[cnt] );
			RS4PrintUsage();
			goto bailout;
		}
	}

	error = false;

bailout:

	return( error );
}

enum RS4FuncStat RS4ParseArgs( enum RS4ErrorCode *errcode, S32 argc, STR argv[] )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
S32 cnt;

	ec = RS4ErrStat_Error;
	fs = RS4FuncStat_Error;

	for( cnt=0 ; cnt < ARGTYPES ; cnt++ )
	{
		if ( _ParseList( myOptions, argc, argv, cnt ))
		{
			goto bailout;
		}
	}

	if ( ! InputFile )
	{
		ec = RS4ErrStat_MissingInputFile;
		printf( "Input file not given\n" );
		goto bailout;
	}

	if ( ! OutputFile )
	{
		ec = RS4ErrStat_MissingOutputFile;
		printf( "Output file not given\n" );
		goto bailout;
	}

	if (( InputFile == OutputFile ) || ( ! strcmp( InputFile, OutputFile )))
	{
		ec = RS4ErrStat_SameInOutFile;
		printf( "Input and output filename must not be the same\n" );
		goto bailout;
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
