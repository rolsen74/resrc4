
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

struct Header BranceList;

// --

#define MAX_LOGS	16

struct LogNode
{
	uint32_t		Addr;
	const char *	Opcode;
	int				Used;
};

struct LogNode		LogData[MAX_LOGS];
int					LogPos; 

// --

static void Log_Clear( void )
{
int cnt;

	for( cnt=0 ; cnt<MAX_LOGS ; cnt++ )
	{
		LogData[cnt].Used = 0;
	}
}

// --

static void Log_AddNode( struct M68kStruct *ms )
{
	LogData[ LogPos ].Opcode	= ms->ms_Str_Opcode;
	LogData[ LogPos ].Addr		= ms->ms_MemoryAdr;
	LogData[ LogPos ].Used		= 1;

	LogPos++;

	if ( LogPos >= MAX_LOGS )
	{
		LogPos = 0;
	}
}

// --

static void Log_Dump( void )
{
int cnt;
int pos;

	pos = LogPos;

	for( cnt=0 ; cnt<MAX_LOGS ; cnt++ )
	{
		pos++;

		if ( pos >= MAX_LOGS )
		{
			pos = 0;
		}

		if ( LogData[pos].Used )
		{
			printf( "LOG : %08x : %s\n", LogData[pos].Addr, LogData[pos].Opcode );
		}
	}

	printf( "\n" );
}

// --

static int Text_ScanHunk( struct HunkNode *hn )
{
struct HunkLabel *next;
struct HunkLabel *hl;
uint8_t *type;
uint8_t *buf;
int error;
int size;
int max;
int len;
int cnt;
int c;
int i;

	error = true;

	buf = hn->hn_MemoryBuf;
	size = hn->hn_MemorySize;
	type = hn->hn_MemoryType;

	hl = myGetHead( & hn->hn_Labels );

	while( hl )
	{
		if (( hl->hl_Label_Offset >= 0 ) 
		&&	( hl->hl_Label_Offset < size )
		&&	( hl->hl_Label_Type == LT_Unknown )
		&&	( buf[  hl->hl_Label_Offset ] != 0 )
		&&	( type[ hl->hl_Label_Offset ] == MT_Unset ))
		{
			len = hl->hl_Label_Offset;

			max = size - len;

			next = myGetNext( hl );

			if ( next )
			{
				max = MIN( max, next->hl_Label_Offset - hl->hl_Label_Offset );
			}

			for( cnt=0 ; cnt<max ; cnt++ )
			{
				c = buf[len];

				if ((( c < 127 ) && ( 31 < c )) 
				||	( c == 9 )
				||	( c == 10 ))
//				||	( c == 13 ))
				{
					len++;
				} 
				else
				{
					break;
				}
			}

			for( i=0 ; i<4 ; i++ )
			{
				// NUL Term and max 3 bytes to align
				if (( buf[len] == 0 ) && ( cnt < max ))
				{
					cnt++;
					len++;
				}
				else
				{
					break;
				}
			}

			len -= hl->hl_Label_Offset;

			if (( len > 3 ) && ( len == max ))
			{
				hl->hl_Label_Type = LT_String;
				hl->hl_Label_Size = len;

				memset( & type[ hl->hl_Label_Offset ], MT_Data, len );
			}
		}

		hl = myGetNext( hl );
	}

	error = false;

// bailout:

	return( error );
}

// --

static int Text_Scan( struct HunkStruct *hs )
{
struct HunkNode *hn;
int error;
int cnt;

	error = true;

	for( cnt=0 ; cnt<hs->hs_HunkArraySize ; cnt++ )
	{
		hn = hs->hs_HunkArray[cnt].hi_HunkNode;

		if ( hn->hn_HunkType != HUNK_BSS )
		{
			if ( Text_ScanHunk( hn ))
			{
				printf( "%s:%04d: Error Text_Scan() failed\n", __FILE__, __LINE__ );
				goto bailout;
			}
		}
	}

	error = false;

bailout:

	return( error );
}

// --

static int Trace_ScanHunkJumps( struct HunkStruct *hs, struct M68kStruct *ms, struct HunkNode *hn )
{
uint8_t *mem;
int error;
int size;
int pos;

// printf( "--- Jump Hunk %d Scan Start\n", hn->hn_HunkNr );

	error = true;

	mem		= hn->hn_MemoryBuf;
	size	= hn->hn_MemorySize;
	pos		= 0;

	if ( size < 2 )
	{
		error = false;
		goto bailout;
	}

	size -= 2; // Avoid buffer overrun

	ms->ms_HunkNode = hn;

	while( pos < size )
	{
		//	Jmp	L02BB(PC,D0.w)	; $0002047a 4efb 0004
		if (( mem[pos] == 0x4e ) && ( mem[pos+1] == 0xfb ))
		{
			ms->ms_MemoryBuf = & mem[pos];
			ms->ms_MemoryAdr = hn->hn_MemoryAdr + pos;
			ms->ms_MemoryType = & hn->hn_MemoryType[ pos ];

			if ( Trace_JumpTable( hs, ms ))
			{
				printf( "%s:%04d: Error Trace_ScanHunkJumps failed\n", __FILE__, __LINE__ );
				goto bailout;
			}
		}

		pos++;
	}

	error = false;

bailout:

// printf( "--- Jump Hunk Scan Ended\n" );

	return( error );
}

// --

static int Trace_ScanJumps( struct HunkStruct *hs )
{
struct M68kStruct ms;
struct HunkNode *hn;
int error;
int cnt;

// printf( "--- Jump Scan Start\n" );

	error = true;

	memset( & ms, 0, sizeof( ms ));

	ms.ms_HunkStruct = hs;

	for( cnt=0 ; cnt<hs->hs_HunkArraySize ; cnt++ )
	{
		hn = hs->hs_HunkArray[cnt].hi_HunkNode;

		if ( hn->hn_HunkType == HUNK_CODE )
		{
			if ( Trace_ScanHunkJumps( hs, & ms, hn ))
			{
				printf( "%s:%04d: Error Trace_ScanJumps failed\n", __FILE__, __LINE__ );
				goto bailout;
			}
		}
	}

	error = false;

bailout:

// printf( "--- Jump Scan Ended\n" );

	return( error );
}

// --

int Trace_AddBrance( struct HunkStruct *hs, struct M68kStruct *ms, int32_t addr )
{
struct BranceNode *bn;
struct HunkNode *hn;
int error;

	hn = Hunk_FindNode( hs, addr );

	if ( hn == NULL )
	{
		printf( "Warning: Brancing outside of Hunk memory not supported ($%08x)\n", addr );
		error = false;
		goto bailout;
	}

	bn = calloc( 1, sizeof( struct BranceNode ));

	if ( bn == NULL )
	{
		printf( "%s:%04d: Error allocating memory (%d Bytes)\n", __FILE__, __LINE__, (int) sizeof( struct BranceNode ));
		error = true;
		goto bailout;
	}

	bn->bn_HunkNode		= hn;
	bn->bn_HunkOffset	= addr - hn->hn_MemoryAdr;
	bn->bn_HunkAddress	= addr;

	if ( ms )
	{
		memcpy( & bn->bn_Registers, & ms->ms_Registers[0], 16 * sizeof( struct M68kRegister ));
	}

	myAddTail( & BranceList, bn );

	error = false;

bailout:

	return( error );
}

// --

static int myTraceBrance( struct HunkStruct *hs, struct BranceNode *bn, struct M68kStruct *ms )
{
struct HunkLabel *hl;
struct HunkNode *hn;
uint32_t size;
uint32_t pos;
uint8_t *type;
int error;

	error	= true;

	hn		= bn->bn_HunkNode;
	pos		= bn->bn_HunkOffset;

	type	= hn->hn_MemoryType;
	size	= hn->hn_MemorySize;

	ms->ms_HunkStruct = hs;
	ms->ms_HunkNode = hn;

	memcpy( & ms->ms_Registers[0], & bn->bn_Registers[0], sizeof( struct M68kRegister ) * 16 );

	if ( type[ pos ] == MT_Unset )
	{
		hl = Hunk_FindLabel( ms->ms_HunkStruct, hn->hn_MemoryAdr + pos );

		if ( hl )
		{
			memcpy( & hl->hl_Registers[0], & bn->bn_Registers[0], sizeof( struct M68kRegister ) * 16 );
		}
	}

	while( true )
	{
		if ( pos >= size )
		{
			break;
		}

		if ( type[ pos ] != MT_Unset )
		{
			break;
		}

		ms->ms_MemoryAdr	= hn->hn_MemoryAdr + pos;
		ms->ms_MemoryBuf	= & hn->hn_MemoryBuf[ pos ];
		ms->ms_MemoryType	= & hn->hn_MemoryType[ pos ];

		M68k_Decode( ms );

		if ( ms->ms_DecodeStatus == MSTAT_UnknownCmd )
		{
			printf( "\n" );
			printf( "Unknown Opcode : $%08x\n", ms->ms_Opcode );
			printf( "Address ...... : $%08x\n", ms->ms_MemoryAdr );
			printf( "Hunk Nr ...... : %d\n", hn->hn_HunkNr );
			printf( "Hunk Type .... : $%04x\n", hn->hn_HunkType );
			printf( "Hunk Offset .. : $%08x (%d)\n", pos, pos );
			printf( "File Offset .. : $%08x (%d)\n", hn->hn_FileOffset+pos, hn->hn_FileOffset+pos );
			printf( "\n" );
			Log_Dump();
			break;
		}

		#if 0
		else
		{
			char name[256];
			char adr[16];
			sprintf( adr, "$%08x", hn->hn_MemoryAdr + pos );
			memset( & name[0], 32, 12+8+8 );
			memcpy( & name[0], adr, strlen( adr ));
			memcpy( & name[16], ms->ms_Str_Opcode, strlen( ms->ms_Str_Opcode ));
			strcpy( & name[28], ms->ms_Buf_Argument );
			printf( "%s\n", name );
		}
		#endif

		if ( ms->ms_DecodeStatus == MSTAT_Error )
		{
			break;
		}

		Log_AddNode( ms );

		memset( & type[ pos ], MT_Code, ms->ms_OpcodeSize );

		if ( ms->ms_JumpTable )
		{
			if ( Trace_JumpTable( hs, ms ))
			{
				printf( "%s:%04d: Jump Table Error\n", __FILE__, __LINE__ );
				goto bailout;
			}
		}

		if ( ms->ms_JmpRegister.mr_Type != RT_Unknown )
		{
			switch( ms->ms_JmpRegister.mr_Type )
			{
				case RT_Label:
				{
					if ( ms->ms_JmpRegister.mr_LabelNode == NULL )
					{
						printf( "%s:%04d: Error NULL Pointer\n", __FILE__, __LINE__ );
						goto bailout;
					}

					if ( Trace_AddBrance( hs, ms, ms->ms_JmpRegister.mr_LabelNode->hl_Label_Address ))
					{
						printf( "%s:%04d: Error adding brance\n", __FILE__, __LINE__ );
						goto bailout;
					}
					break;
				}

				case RT_Address:
				{
					if ( Trace_AddBrance( hs, ms, ms->ms_JmpRegister.mr_Address ))
					{
						printf( "%s:%04d: Error adding brance\n", __FILE__, __LINE__ );
						goto bailout;
					}
					break;
				}

				default:
				{
					break;
				}
			}
		}

		if ( ms->ms_ClearRegMask )
		{
			for( int cnt=0 ; cnt<16 ; cnt++ )
			{
				if ( ms->ms_ClearRegMask & ( 1 << cnt ))
				{
					ms->ms_Registers[cnt].mr_Type = RT_Unknown;
				}
			}

			ms->ms_ClearRegMask = 0;
		}

		if ( ms->ms_LastOpcode == true )
		{
			break;
		}

		pos += ms->ms_OpcodeSize;
	}

	error = false;

bailout:

	return( error );
}

// --

int TraceHunk( struct HunkStruct *hs )
{
struct BranceNode *bn;
struct M68kStruct ms;
char argbuf[256];
int error;
int err;

	error = true;

	// --

	hs->hs_PassNr = 1;

	// --

	memset( & ms, 0, sizeof( ms ));

	ms.ms_Buf_Argument = argbuf;

	Log_Clear();

	// --
	// Set our start Trace Addresse

	Trace_AddBrance( hs, NULL, hs->hs_HunkArray[ hs->hs_HunkFirst ].hi_MemoryAdr );

    // --

	if ( Trace_ScanJumps( hs ))
	{
		printf( "%s:%04d: Error scanning jump tables\n", __FILE__, __LINE__ );
		goto bailout;
	}

    // --

	bn = myRemHead( & BranceList );

    while( bn )
    {
		err = myTraceBrance( hs, bn, & ms );

		free( bn );

		if ( err  )
		{
			printf( "%s:%04d: Error tracing brance\n", __FILE__, __LINE__ );
			goto bailout;
		}

		bn = myRemHead( & BranceList );
    }

	// --

	if ( Text_Scan( hs ))
	{
		printf( "%s:%04d: Error scanning for text\n", __FILE__, __LINE__ );
		goto bailout;
	}

	// --
	error = false;

bailout:

	while(( bn = myRemHead( & BranceList )))
	{
		free( bn );
	}

	return( error );
}

// --
