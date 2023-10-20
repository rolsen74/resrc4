 
/*
 * Copyright (c) 2014-2023 Rene W. Olsen < renewolsen @ gmail . com >
 * All rights reserved.
 *
 */

// --

#include "ReSrc4.h"

// --

// --

static char *CONFIG_HEADER =	
	"[## ReSrc4 v1 : Configfile ##]";

static char CONFIG_BODY_FMT[] =
	"\n" // Header Newline
	"\n"
	";\n"
	"; Amiga Hunk file MD5 check sum is a mandatory entry\n"
	";\n"
	"; MD5: $xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"
	";\n"
	"\n"
	"MD5: $%s\n"
	"\n"
	"\n"
	";\n"
	"; MemWrite writes to memory before start of Tracing\n"
	";\n"
	"; MemWrite: $12345678 $xx ..\n"
	";\n"
	"\n"
	"\n"
	";\n"
	"; The Code command starts a Trace at the Address given.\n"
	";\n"
	"; Code: $12345678\n"
	";\n"
	"\n"
	"\n"
	";\n"
	"; Relative16 is normal a JumpTable used by C Compilers\n"
	";\n"
	";             TableAddress RelativeAddress Entries\n"
	"; Relative16: $12345678    $12345678       x\n"
	";\n"
	"\n"
	"\n"
	";\n"
	"; LabelName creates a label at address with given name.\n"
	";\n"
	"; The name can be set as \"\", and the resourcer will give\n"
	"; it a general name.\n"
	";\n"
	"; LabelName: $12345678 \"Name\"\n"
	";\n"
	"\n"
	"\n"
	;

// --

static const int8_t HexData[256] =
{
//	 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// 0
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// 1
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// 2
	 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,	// 3
	-1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// 4
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// 5
	-1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// 6
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// 7
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// 8
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// 9
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// A
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// B
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// C
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// D
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// E
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	// F
};

// --

static int myReadAddress( char *buf, int *p, int32_t *a )
{
int32_t adr;
int error;
int off;
int pos;
int hex;
int cnt;
int val;

	error = true;

	pos = *p;

	while(( buf[pos] == 9 ) || ( buf[pos] == 32 ))
	{
		pos++;
	}

	if ( buf[pos] != '$' )
	{
		goto bailout;
	}

	pos++;

	// Need atlease one hex digt
	adr = 0;
	off = buf[ pos ];
	hex = HexData[ off ];

	if ( hex == -1 )
	{
		goto bailout;
	}

	for( cnt=0 ; cnt<8 ; cnt++ )
	{
		val = buf[ pos ];
		hex = HexData[ val ];

		if ( hex == -1 )
		{
			break;
		}

		adr = adr << 4;
		adr = adr | hex;
		pos++;
	}

	*a = adr;
	*p = pos;

	error = false;

bailout:

	return( error );
}

// --

static int myReadNumber( char *buf, int *p, int32_t *n )
{
int error;
int pos;
int val;
int c;

	error = true;

	pos = *p;

	while(( buf[pos] == 9 ) || ( buf[pos] == 32 ))
	{
		pos++;
	}

	// Need atlease one digt
	if (( buf[pos] < '0' ) || ( buf[pos] >= '9' ))
	{
		goto bailout;
	}

	val = 0;

	while( true )
	{
		c = buf[pos];

		if (( c < '0' ) || ( c >= '9' ))
		{
			break;
		}

		val = ( val * 10 ) + ( c - '0' );
		pos++;
	}

	*n = val;
	*p = pos;

	error = false;

bailout:

	return( error );
}

// --

static int myReadString( char *buf, int *p, char *str, int max )
{
int error;
int pos;
int cnt;
int c;

	error = true;

	pos = *p;

	while(( buf[pos] == 9 ) || ( buf[pos] == 32 ))
	{
		pos++;
	}

	if ( buf[pos] != '\"' )
	{
		printf( "Missing String \" Indicator\n" );
		goto bailout;
	}

	pos++;

	cnt = 0;

	while( true )
	{
		c = buf[pos];

		if (( c == 0 )
		||	( c == 10 )
		||	( c == '\"' ))
		{
			break;
		}

		pos++;

		if ( cnt >= max )
		{
			printf( "String to long\n" );
			goto bailout;
		}

		str[cnt++] = c;
	}

	if ( buf[pos] != '\"' )
	{
		printf( "String \" Terminator\n" );
		goto bailout;
	}

	pos++;

	str[cnt] = 0;

	*p = pos;

	error = false;

bailout:

	return( error );
}

// --

static int CMD_LABELTYPE( struct HunkStruct *hs, char *buf, int linenr )
{
struct HunkLabel *hl;
int32_t adr;
int type;
int err;
int pos;

	err = true;
	pos = 10;
	
	while(( buf[pos] == 9 ) || ( buf[pos] == 32 ))
	{
		pos++;
	}

	if ( myReadAddress( buf, & pos, & adr ))
	{
		printf( "Line %d : LabelType : Error reading address\n", linenr );
		goto bailout;
	}

	hl = Hunk_AddLabel( hs, adr, LT_Unknown );

	if ( hl == NULL )
	{
		printf( "Line %d : LabelType : Error adding label at $%08x\n", linenr, adr );
		goto bailout;
	}

	while(( buf[pos] == 9 ) || ( buf[pos] == 32 ))
	{
		pos++;
	}

	if (( buf[pos] == 0 )
	||	( buf[pos] == 10 )
	||	( buf[pos] == 13 )
	||	( buf[pos] == ';' ))
	{
		printf( "Line %d : LabelType : Error Missing Command at $%08x\n", linenr, adr );
		goto bailout;
	}

	/**/ if ( ! strncasecmp( & buf[ pos ], "Pointer", 7 ))
	{
		pos += 7;

		while(( buf[pos] == 9 ) || ( buf[pos] == 32 ))
		{
			pos++;
		}

		if (( buf[pos] == 0 )
		||	( buf[pos] == 10 )
		||	( buf[pos] == ';' ))
		{
			printf( "Line %d : LabelType : Error Missing Command Argument at $%08x\n", linenr, adr );
			goto bailout;
		}

		/**/ if ( ! strncasecmp( & buf[ pos ], "DiskFontBase", 12 ))
		{
			pos += 12;
			type = LPT_Library + LIBT_DiskFontBase;
		}
		else if ( ! strncasecmp( & buf[ pos ], "DosBase", 7 ))
		{
			pos += 7;
			type = LPT_Library + LIBT_DosBase;
		}
		else if ( ! strncasecmp( & buf[ pos ], "ExecBase", 8 ))
		{
			pos += 8;
			type = LPT_Library + LIBT_ExecBase;
		}
		else if ( ! strncasecmp( & buf[ pos ], "GfxBase", 7 ))
		{
			pos += 7;
			type = LPT_Library + LIBT_GraphicsBase;
		}
		else if ( ! strncasecmp( & buf[ pos ], "IntuitionBase", 13 ))
		{
			pos += 13;
			type = LPT_Library + LIBT_IntuitionBase;
		}
		else
		{
			printf( "Line %d : LabelType : Error Unsupported Command Argument at $%08x\n", linenr, adr );
			goto bailout;
		}

		hl = Hunk_AddLabel( hs, adr, LT_Pointer );

		if ( hl == NULL )
		{
			printf( "Line %d : LabelType : Error adding label at $%08x\n", linenr, adr );
			goto bailout;
		}

		hl->hl_Label_SubType = type;
	}
	else
	{
		printf( "Line %d : LabelType : Error Unsupported Command at $%08x\n", linenr, adr );
		goto bailout;
	}

	while(( buf[pos] == 9 ) || ( buf[pos] == 32 ))
	{
		pos++;
	}

	if (( buf[pos] )
	&&	( buf[pos] != 10 )
	&&	( buf[pos] != ';' ))
	{
		printf( "Line %d : LabelName : Error junk at end of line\n", linenr );
		goto bailout;
	}

	err = false;

bailout:

	return( err );
}

// --

static int CMD_LABELNAME( struct HunkStruct *hs, char *buf, int linenr )
{
struct HunkLabel *hl;
int32_t adr;
int err;
int pos;

printf( "Label Name\n" );

	err = true;
	pos = 10;
	
	while(( buf[pos] == 9 ) || ( buf[pos] == 32 ))
	{
		pos++;
	}

	if ( myReadAddress( buf, & pos, & adr ))
	{
		printf( "Line %d : LabelName : Error reading address\n", linenr );
		goto bailout;
	}

printf( "adr %08x\n", adr );

	hl = Hunk_AddLabel( hs, adr, LT_Unknown );

	if ( hl == NULL )
	{
		printf( "Line %d : LabelName : Error adding label at $%08x\n", linenr, adr );
		goto bailout;
	}

	if ( myReadString( buf, & pos, hl->hl_Label_Name, MAX_LabelName ))
	{
		printf( "Line %d : LabelName : Error reading string\n", linenr );
		goto bailout;
	}

printf( "str %s\n", hl->hl_Label_Name );

	while(( buf[pos] == 9 ) || ( buf[pos] == 32 ))
	{
		pos++;
	}

	if (( buf[pos] )
	&&	( buf[pos] != 10 )
	&&	( buf[pos] != ';' ))
	{
		printf( "Line %d : LabelName : Error junk at end of line\n", linenr );
		goto bailout;
	}

	err = false;

bailout:

printf( "done\n" );

	return( err );
}

// --

static int CMD_RELATIVE16( struct HunkStruct *hs, char *buf, int linenr )
{
int relative_adr;
int tabel_adr;
int entries;
int err;
int pos;

	err = true;
	pos = 11;
	
	while(( buf[pos] == 9 ) || ( buf[pos] == 32 ))
	{
		pos++;
	}

	if ( myReadAddress( buf, & pos, & tabel_adr ))
	{
		printf( "Line %d : Relative16 : Error reading address\n", linenr );
		goto bailout;
	}

	if ( myReadAddress( buf, & pos, & relative_adr ))
	{
		printf( "Line %d : Relative16 : Error reading address\n", linenr );
		goto bailout;
	}

	if ( myReadNumber( buf, & pos, & entries ))
	{
		printf( "Line %d : Relative16 : Error reading number\n", linenr );
		goto bailout;
	}

	if ( JumpTable_Rel16( hs, tabel_adr, relative_adr, entries ))
	{
		printf( "Line %d : Relative16 : Error adding JumpTabel\n", linenr );
		goto bailout;
	}

	while(( buf[pos] == 9 ) || ( buf[pos] == 32 ))
	{
		pos++;
	}

	if (( buf[pos] )
	&&	( buf[pos] != 10 )
	&&	( buf[pos] != ';' ))
	{
		printf( "Line %d : Relative16 : Error junk at end of line\n", linenr );
		goto bailout;
	}

	err = false;

bailout:

	return( err );
}

// --

static int CMD_MEMWRITE( struct HunkStruct *hs, char *buf, int linenr )
{
struct HunkNode *hn;
uint8_t *mem;
uint32_t adr;
uint8_t chr;
int off;
int max;
int pos;
int cnt;
int hex;
int val;
int err;
int len;

	err = true;
	pos = 9;
	
	 // skip cmd

	while(( buf[pos] == 9 ) || ( buf[pos] == 32 ))
	{
		pos++;
	}

	if ( buf[pos] != '$' )
	{
		printf( "Line %d : MemWrite : Address Argument must start with a $\n", linenr );
		goto bailout;
	}

	pos++;

	adr = 0;
	off = buf[ pos ];
	hex = HexData[ off ];

	if ( hex == -1 )
	{
		printf( "Line %d : MemWrite : Must be atleast one hex number after $\n", linenr );
		goto bailout;
	}

	for( cnt=0 ; cnt<8 ; cnt++ )
	{
		val = buf[ pos ];
		hex = HexData[ val ];

		if ( hex == -1 )
		{
			break;
		}

		adr = adr << 4;
		adr = adr | hex;
		pos++;
	}

	hn = Hunk_FindNode( hs, adr );

	if ( hn == NULL )
	{
		printf( "Line %d : MemWrite : Address $%08x not found\n", linenr, adr );
		goto bailout;
	}

	mem = hn->hn_MemoryBuf;
	max = hn->hn_MemorySize;
	off = adr - hn->hn_MemoryAdr;

	// --

	if (( buf[pos] != 9 ) && ( buf[pos] != 32 ))
	{
		printf( "Line %d : MemWrite : Expected Space/Tab after address\n", linenr );
		goto bailout;
	}

	while(( buf[pos] == 9 ) || ( buf[pos] == 32 ))
	{
		pos++;
	}

	if ( buf[pos] != '$' )
	{
		printf( "Line %d : MemWrite : Buffer Argument must start with a $\n", linenr );
		goto bailout;
	}

	pos++;

	// Count mem bytes

	len = pos;
	chr = 0;
	int oo = 1;

	while( true )
	{
		val = buf[ len ];
		hex = HexData[ val ];

		if ( hex == -1 )
		{
			break;
		}

		if ( max <= off )
		{
			printf( "Line %d : MemWrite : Writting past end of memory block\n", linenr );
			goto bailout;
		}

		chr = ( chr << 4 ) + ( hex );

		oo++;

		if ( oo & 1 )
		{
// printf( "Write %02x to %08x\n", chr, hn->hn_MemoryAdr + off );
			mem[off++] = chr;
		}

		len++;
	}

	len -= pos;

	if ( len == 0 )
	{
		printf( "Line %d : MemWrite : Missing Memory Data\n", linenr );
		goto bailout;
	}

	if ( len & 1 )
	{
		printf( "Line %d : MemWrite : Memory Data, incomplete hex data\n", linenr );
		goto bailout;
	}

	pos += len;

	// --

	while(( buf[pos] == 9 ) || ( buf[pos] == 32 ))
	{
		pos++;
	}

	if (( buf[pos] )
	&&	( buf[pos] != 10 )
	&&	( buf[pos] != 13 )
	&&	( buf[pos] != ';' ))
	{
		printf( "Line %d : MemWrite : Invalid data at end of Code Argument\n", linenr );
		goto bailout;
	}

// printf( "Patched Address : %08x\n", adr );

	err = false;

bailout:

	return( err );
}

// --

static int CMD_CODE( struct HunkStruct *hs, char *buf, int linenr )
{
int32_t adr;
int err;
int pos;

	err = true;
	pos = 5;
	
	while(( buf[pos] == 9 ) || ( buf[pos] == 32 ))
	{
		pos++;
	}

	if ( myReadAddress( buf, & pos, & adr ))
	{
		printf( "Line %d : Code : Error reading address\n", linenr );
		goto bailout;
	}

	if ( Trace_AddBrance( hs, NULL, adr ))
	{
		printf( "Line %d : Code : Error adding code brance\n", linenr );
		goto bailout;
	}

	while(( buf[pos] == 9 ) || ( buf[pos] == 32 ))
	{
		pos++;
	}

	if (( buf[pos] )
	&&	( buf[pos] != 10 )
	&&	( buf[pos] != ';' ))
	{
		printf( "Line %d : Code : Error junk at end of line\n", linenr );
		goto bailout;
	}

	err = false;

bailout:

	return( err );
}

// --

static int CMD_MD5( char *buf, char *md5, int linenr )
{
int stat;
int pos;
int cnt;
int err;
int p;

	err = true;
	pos = 4;
	
	 // skip cmd

	while(( buf[pos] == 9 ) || ( buf[pos] == 32 ))
	{
		pos++;
	}

	if ( buf[pos] != '$' )
	{
		printf( "Line %d : MD5 : Address Argument must start with a $\n", linenr );
		goto bailout;
	}

	pos++;

	p = pos;

	for( cnt=0 ; cnt<32 ; cnt++ )
	{
		stat = ((( buf[p] >= '0' ) && ( buf[p] <= '9' ))
			||	(( buf[p] >= 'a' ) && ( buf[p] <= 'f' ))
			||	(( buf[p] >= 'A' ) && ( buf[p] <= 'F' )));

		if ( ! stat )
		{
			printf( "Line %d : MD5 : Invalid string\n", linenr );
			goto bailout;
		}

		p++;
	}

	while(( buf[p] == 9 ) || ( buf[p] == 32 ))
	{
		p++;
	}

	if (( buf[p] )
	&&	( buf[p] != 10 )
	&&	( buf[p] != 13 )
	&&	( buf[p] != ';' ))
	{
		printf( "Line %d : MD5 : Invalid data at end of string\n", linenr );
		goto bailout;
	}

	memcpy( md5, & buf[pos], 32 );

	err = false;

bailout:

	return( err );
}

// --

static int DoCreateConfigFile( struct HunkStruct *hs )
{
FILE *file;
char buf[1024];
int total;
int error;
int fsize;
int len;

	total = 0;
	error = true;

	file = fopen( ConfigFile, "wb" );

	if ( file == NULL )
	{
		printf( "Error opening '%s' config file\n", ConfigFile );
		goto bailout;
	}

	len = strlen( CONFIG_HEADER );
	fsize = fwrite( CONFIG_HEADER, 1, len, file );

	if ( fsize != len )
	{
		printf( "Error writting data\n" );
		goto bailout;
	}

	total += fsize;

	snprintf( buf, 1024, CONFIG_BODY_FMT, hs->hs_FileMD5 );
//	memcpy( & CONFIG_BODY[7], hs->hs_FileMD5, 32 );

	len = strlen( buf );
	fsize = fwrite( buf, 1, len, file );

	if ( fsize != len )
	{
		printf( "Error writting data\n" );
		goto bailout;
	}

	total += fsize;

	printf( "Created '%s' config file (%d bytes)\n", ConfigFile, total );

	error = false;

bailout:

	if ( file )
	{
		fclose( file );

		if ( error )
		{
			remove( ConfigFile );
		}
	}

	if ( error )
	{
		printf( "Error creating config file\n" );
	}

	return( error );
}

// --

static int myParseConfig( struct HunkStruct *hs, char *buffer, int size )
{
char md5[34];
int linenr;
int error;
int pos;
int len;

	error = true;

	len = strlen( CONFIG_HEADER );

	if ( strncasecmp( buffer, CONFIG_HEADER, len ))
	{
		printf( "Invalid Config Header\n" );
		goto bailout;
	}

	memset( md5, 'x', 32 );
	md5[32] = 0;

	pos = 0;

	linenr = 1;

	while(( buffer[pos] ) && ( pos < size ))
	{
		// -- Next Line

		while(( buffer[pos] ) && ( buffer[pos] != 10 ))
		{
			pos++;
		}

		while(( buffer[pos] ) && ( buffer[pos] == 10 ))
		{
			linenr++;
			pos++;
		}

		// -- Skip Space and Tab

		while(( buffer[pos] == 9 ) || ( buffer[pos] == 32 ))
		{
			pos++;
		}

		// -- 

		if (( buffer[pos] != 10 )
		&&	( buffer[pos] != 13 )
		&&	( buffer[pos] != ';' ))
		{
			// Get Command Length

			len = pos;

			while(( buffer[len] ) && ( buffer[len] != 9 ) && ( buffer[len] != 32 ))
			{
				len++;
			}

			len -= pos;

			/**/ if (( len == 4 ) && ( ! strncasecmp( & buffer[pos], "MD5:", 4 )))
			{
				if ( CMD_MD5( & buffer[pos], md5, linenr ))
				{
					goto bailout;
				}
			}
			else if (( len == 5 ) && ( ! strncasecmp( & buffer[pos], "Code:", 5 )))
			{
				if ( CMD_CODE( hs, & buffer[pos], linenr ))
				{
					goto bailout;
				}
			}
			else if (( len == 9 ) && ( ! strncasecmp( & buffer[pos], "MemWrite:", 9 )))
			{
				if ( CMD_MEMWRITE( hs, & buffer[pos], linenr ))
				{
					goto bailout;
				}
			}
			else if (( len == 11 ) && ( ! strncasecmp( & buffer[pos], "Relative16:", 11 )))
			{
				if ( CMD_RELATIVE16( hs, & buffer[pos], linenr ))
				{
					goto bailout;
				}
			}
			else if (( len == 10 ) && ( ! strncasecmp( & buffer[pos], "LabelName:", 10 )))
			{
				if ( CMD_LABELNAME( hs, & buffer[pos], linenr ))
				{
					goto bailout;
				}
			}
			else if (( len == 10 ) && ( ! strncasecmp( & buffer[pos], "LabelType:", 10 )))
			{
				if ( CMD_LABELTYPE( hs, & buffer[pos], linenr ))
				{
					goto bailout;
				}
			}
		}

		// --
	}

	// --

	if ( strncasecmp( hs->hs_FileMD5, md5, 32 ))
	{
		printf( "Input file MD5 sum do not match the Config file sum\n" );
		printf( "Cfg  : %s\n", md5 );
		printf( "File : %s\n", hs->hs_FileMD5 );
		goto bailout;
	}

	error = false;

bailout:

	return( error );
}

// --

int HandleConfigFile( struct HunkStruct *hs )
{
char response[10];
char *buffer;
FILE *file;
int error;
int fsize;
int size;
int stat;
int len;

	file = NULL;

	error = true;

	buffer = NULL;

	if ( ConfigFile == NULL )
	{
		len = strlen( InputFile );

		if ((( len > 4 ) && ( ! strncasecmp( & InputFile[ len - 4 ], ".exe", 4 )))
		||  (( len > 4 ) && ( ! strncasecmp( & InputFile[ len - 4 ], ".bin", 4 ))))
		{
			ConfigFile = strdup( InputFile );

			if ( ConfigFile == NULL )
			{
				goto bailout;
			}

			ConfigFile[ len - 3 ] = 'c';
			ConfigFile[ len - 2 ] = 'f';
			ConfigFile[ len - 1 ] = 'g';
		}
		else
		{
			stat = asprintf( & ConfigFile, "%s.cfg", InputFile );

			if (( stat < 0 ) || ( ConfigFile == NULL ))
			{
				goto bailout;
			}
		}
	}

	// -- Load

	file = fopen( ConfigFile, "rb" );

	if ( file == NULL )
	{
        printf( "The config file '%s' does not exist.\n", ConfigFile );
        printf( "Do you want to create it? (Yes/[NO]): ");

		if ( fgets( response, sizeof( response ), stdin ) == NULL )
		{
			printf( "Error reading user input\n" );
			goto bailout;
		}

		if (( response[0] != 'Y' ) && ( response[0] != 'y' )) 
		{
			// Not an error
			error = false;
			goto bailout;
		}

		// Create an Empty Config File
		if ( ! DoCreateConfigFile( hs ))
		{
			error = false;
		}
		goto bailout;			
	}

	fseek( file, 0, SEEK_END );

	size = ftell( file );

	fseek( file, 0, SEEK_SET );

	buffer = calloc( 1, size + 1 );

	if ( buffer == NULL )
	{
		printf( "%s:%04d: Error allocating memory (%d Bytes)\n", __FILE__, __LINE__, size );
		goto bailout;
	}

	fsize = fread( buffer, 1, size, file );

	if ( fsize != size )
	{
		printf( "Error reading file\n" );
		goto bailout;
	}

	printf( "Loaded %s (%d Bytes)\n", ConfigFile, size );

	// --

	if ( myParseConfig( hs, buffer, size ))
	{
		goto bailout;
	}

	error = false;

bailout:

	if ( buffer )
	{
		free( buffer );
	}

	if ( file )
	{
		fclose( file );
	}

	return( error );
}

// --
