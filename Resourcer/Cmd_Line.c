 
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

static enum RS4FuncStat ArgFunc_ArgTabs( char *arg )
{
enum RS4FuncStat fs;
int val;

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

static enum RS4FuncStat ArgFunc_OpcodeTabs( char *arg )
{
enum RS4FuncStat fs;
int val;

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

static enum RS4FuncStat ArgFunc_LabTabs( char *arg )
{
enum RS4FuncStat fs;
int val;

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

static enum RS4FuncStat ArgFunc_AutoYes( char *arg UNUSED )
{
enum RS4FuncStat fs;

	fs = RS4FuncStat_Okay;

	AutoAnser = ANSER_Yes;

	return( fs );
}

// --

static enum RS4FuncStat ArgFunc_AutoNo( char *arg UNUSED )
{
enum RS4FuncStat fs;

	fs = RS4FuncStat_Okay;

	AutoAnser = ANSER_No;

	return( fs );
}

// --

static enum RS4FuncStat ArgFunc_DebugInfo( char *arg UNUSED )
{
enum RS4FuncStat fs;

	fs = RS4FuncStat_Okay;

	DebugInfo = 1;

	return( fs );
}

// --

static enum RS4FuncStat ArgFunc_Verbose( char *arg UNUSED )
{
enum RS4FuncStat fs;

	fs = RS4FuncStat_Okay;

	Verbose = 1;

	return( fs );
}

// --

static enum RS4FuncStat ArgFunc_Verbose2( char *arg UNUSED )
{
enum RS4FuncStat fs;

	fs = RS4FuncStat_Okay;

	Verbose = 2;

	return( fs );
}

// --

static enum RS4FuncStat ArgFunc_Short( char *arg UNUSED )
{
enum RS4FuncStat fs;

	fs = RS4FuncStat_Okay;

	ShortOpcodes = 1;

	return( fs );
}

// --

static enum RS4FuncStat ArgFunc_Config( char *arg )
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

	fs = RS4FuncStat_Okay;

bailout:

	return( fs );
}

// --

static enum RS4FuncStat ArgFunc_Input( char *arg )
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

	fs = RS4FuncStat_Okay;

bailout:

	return( fs );
}

// --

static enum RS4FuncStat ArgFunc_Output( char *arg )
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

	fs = RS4FuncStat_Okay;

bailout:

	return( fs );
}

// --

struct myArgs
{
	char *	Name1;
	char *	Name2;
	int		Options;
	enum RS4FuncStat (*Function)( char *arg );
};

// -- Early Options before Config file
struct myArgs myOptions[] =
{
{ "-i", "--input",		1, ArgFunc_Input },
{ "-o", "--output",		1, ArgFunc_Output },
{ "-c", "--config",		1, ArgFunc_Config },
{ "-y", "--yes",		0, ArgFunc_AutoYes },
{ "-n", "--no",			0, ArgFunc_AutoNo },
{ NULL, "--short",		0, ArgFunc_Short },
{ NULL, "--debuginfo",	0, ArgFunc_DebugInfo },
{ "-v", "--verbose",	0, ArgFunc_Verbose },
{ "-v1",NULL,			0, ArgFunc_Verbose },
{ "-v2","--verbose2",	0, ArgFunc_Verbose2 },
{ NULL, "--labtabs",	1, ArgFunc_LabTabs },
{ NULL, "--opcodetabs",	1, ArgFunc_OpcodeTabs },
{ NULL, "--argtabs",	1, ArgFunc_ArgTabs },
{ NULL,	NULL, 0, NULL }
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
	printf( " -v2  --verbose2       Print even more information\n" );
	printf( "\n" );
}

// --

enum RS4FuncStat RS4ParseArgs( enum RS4ErrorCode *errcode, int argc, char *argv[] )
{
enum RS4ErrorCode ec;
enum RS4FuncStat cmdstat;
enum RS4FuncStat fs;
int pos;
int cnt;

	fs = RS4FuncStat_Error;
	ec = RS4ErrStat_Error;

	for( cnt=1 ; cnt<argc ; cnt++ )
	{
		pos = 0;

		while( TRUE )
		{
			// End of List?
			if (( myOptions[pos].Name1 == NULL )
			&&	( myOptions[pos].Name2 == NULL ))
			{
				break;
			}

			// Found Argument?
			if ((( myOptions[pos].Name1 ) && ( strcmp( myOptions[pos].Name1, argv[cnt] ) == 0 ))
			||  (( myOptions[pos].Name2 ) && ( strcmp( myOptions[pos].Name2, argv[cnt] ) == 0 )))
			{
				if ( cnt+myOptions[pos].Options >= argc )
				{
					printf( "\nMissing argument for %s\n", argv[cnt] );
					RS4PrintUsage();
					goto bailout;
				}					

				cmdstat = myOptions[pos].Function( argv[cnt+1] );

				if ( cmdstat != RS4FuncStat_Okay )
				{
					goto bailout;
				}

				cnt += myOptions[pos].Options;
				break;
			}

			pos++;
		}

		if (( myOptions[pos].Name1 == NULL )
		&&	( myOptions[pos].Name2 == NULL ))
		{
			printf( "\nUnknown argument : %s\n", argv[cnt] );
			RS4PrintUsage();
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
		printf( "\nInput and output filename must not be the same\n\n" );
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
