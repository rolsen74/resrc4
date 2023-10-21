
/*
 * Copyright (c) 2014-2023 by Rene W. Olsen < renewolsen @ gmail . com >
 * All rights reserved.
 *
 */

// --

#include "ReSrc4.h"

// --

int 					DebugInfo		= 0;		// Add ' $12345678 1122 3344 ' to source lines
int 					ShortOpcodes	= 0;		// 0 = movea.l, 1 = move.l
struct HunkStruct *		HunkData		= NULL;

char *					InputFile		= NULL;
char *					OutputFile		= NULL;
char *					ConfigFile		= NULL;

int						LabTabs			= 1;
int						OpcodeTabs		= 2;
int						ArgTabs			= 5;

char *					LabNames		= "L";		// Label names
char *					ExtNames		= "Ext_";	// External Label names
char *					SecNames		= "Sec_";	// Section Label names

// --

char *myStrdup( char *string ) 
{
char *str;
int len;

	str = NULL;

	if ( string == NULL ) 
	{
		goto bailout;
	}

	len = strlen( string );

	str = malloc( len + 1 );

	if ( str == NULL ) 
	{
		goto bailout;
	}

	strcpy( str, string );

bailout:

	return( str );
}

// --

static int ArgFunc_ArgTabs( char *arg )
{
int error;
int val;

	error = true;

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

	error = false;

bailout:

	return( error );
}

// --

static int ArgFunc_OpcodeTabs( char *arg )
{
int error;
int val;

	error = true;

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

	error = false;

bailout:

	return( error );
}

// --

static int ArgFunc_LabTabs( char *arg )
{
int error;
int val;

	error = true;

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

	error = false;

bailout:

	return( error );
}

// --

static int ArgFunc_DebugInfo( char *arg )
{
	DebugInfo = 1;

	return( false );
}

// --

static int ArgFunc_Short( char *arg )
{
	ShortOpcodes = 1;

	return( false );
}

// --

static int ArgFunc_Config( char *arg )
{
int error;

	error = true;

	if ( ConfigFile )
	{
		free( ConfigFile );
	}

	ConfigFile = myStrdup( arg );

	if ( ConfigFile == NULL )
	{
		printf( "%s:%04d: Error copying string\n", __FILE__, __LINE__ );
		goto bailout;
	}

	error = false;

bailout:

	return( error );
}

// --

static int ArgFunc_Input( char *arg )
{
int error;

	error = true;

	if ( InputFile )
	{
		free( InputFile );
	}

	InputFile = myStrdup( arg );

	if ( InputFile == NULL )
	{
		printf( "%s:%04d: Error copying string\n", __FILE__, __LINE__ );
		goto bailout;
	}

	error = false;

bailout:

	return( error );
}

// --

static int ArgFunc_Output( char *arg )
{
int error;

	error = true;

	if ( OutputFile )
	{
		free( OutputFile );
	}

	OutputFile = myStrdup( arg );

	if ( OutputFile == NULL )
	{
		printf( "%s:%04d: Error copying string\n", __FILE__, __LINE__ );
		goto bailout;
	}

	error = false;

bailout:

	return( error );
}

// --

struct myArgs
{
	char *	Name1;
	char *	Name2;
	int		Options;
	int		(*Function)( char *arg );
};

// -- Early Options before Config file
struct myArgs myOptions[] =
{
{ "-i", "--input",		1, ArgFunc_Input },
{ "-o", "--output",		1, ArgFunc_Output },
{ "-c", "--config",		1, ArgFunc_Config },
{ NULL, "--short",		0, ArgFunc_Short },
{ NULL, "--debuginfo",	0, ArgFunc_DebugInfo },
{ NULL, "--labtabs",	1, ArgFunc_LabTabs },
{ NULL, "--opcodetabs",	1, ArgFunc_OpcodeTabs },
{ NULL, "--argtabs",	1, ArgFunc_ArgTabs },
{ NULL, NULL, 0, NULL }
};

// --

static void myPrintUsage( void )
{
	printf( "\n" );
	printf( "ReSrc4 v%d.%d (%s)\n", VERSION, REVISION, DATE );
	printf( "\n" );
	printf( "Usage: ReSrc4 [options]\n" );
	printf( "\n" );
	printf( "Options:\n" );
	printf( "\n" );
	printf( "  -i, --input          Input File (Amiga Hunk File)\n" );
	printf( "  -o, --output         Output File (Asm Source File)\n" );
	printf( "  -c, --config         Config File (ReSrc4 Config File)\n" );
	printf( "      --short          Output short opcodes ( movea.l -> move.l )\n" );
	printf( "      --debuginfo      Will Append Address and Memory Codes\n" );
	printf( "      --labtabs        Number of Tabs after labels (Default 1)\n" );
	printf( "      --opcodetabs     Number of Tabs after opcodes (Default 2)\n" );
	printf( "      --argtabs        Number of Tabs after arguments (Default 5)\n" );
	printf( "\n" );
}

// --

static int myParseArgs( int argc, char *argv[] )
{
int error;
int stat;
int pos;
int cnt;

	error = true;

	for( cnt=1 ; cnt<argc ; cnt++ )
	{
		pos = 0;

		while( true )
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
					myPrintUsage();
					goto bailout;
				}					

				stat = myOptions[pos].Function( argv[cnt+1] );

				if ( stat )
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
			myPrintUsage();
			goto bailout;
		}
	}

	error = false;

bailout:

	return( error );
}

// --

int main( int argc, char *argv[] )
{
	if ( argc < 2 )
	{
		myPrintUsage();
		goto bailout;
	}

	if ( myParseArgs( argc, argv ))
	{
		goto bailout;
	}

	if ( InputFile == NULL )
	{
		printf( "Input file not given\n" );
		goto bailout;
	}

	if ( OutputFile == NULL )
	{
		printf( "Output file not given\n" );
		goto bailout;
	}

printf( "11 - Load and Parse Hunk File\n" );

	HunkData = Hunk_LoadFile( InputFile );

	if ( HunkData == NULL )
	{
printf( "Load error\n" );
		goto bailout;
	}

printf( "22 - Load and Parse Config\n" );

	if ( HandleConfigFile( HunkData ))
	{
printf( "Config error\n" );
		goto bailout;
	}

printf( "33 - Trace Memory\n" );

	if ( TraceHunk( HunkData ))
	{
printf( "Trace error\n" );
		goto bailout;
	}

printf( "44 - Label Adjust\n" );

	if ( LabelMagic( HunkData ))
	{
printf( "Label error\n" );
		goto bailout;
	}

printf( "55 - Build Source\n" );

	if ( BuildSource( HunkData ))
	{
printf( "Build error\n" );
		goto bailout;
	}

printf( "66 - Save Source\n" );

	if ( SaveSource( HunkData, OutputFile ))
	{
printf( "Save error\n" );
		goto bailout;
	}

printf( "77 - Done Yay\n" );

bailout:

	if ( HunkData )
	{
		Hunk_FreeData( HunkData );
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
