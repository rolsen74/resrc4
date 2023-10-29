
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

void Mark_NulString( struct HunkLabel *hl )
{
struct HunkNode *hn;
uint8_t *type;
uint8_t *mem;
int32_t off;
int chr;
int len;

	hn	= hl->hl_HunkNode;
	off	= hl->hl_Label_Offset;
	mem = hn->hn_MemoryBuf;
	type= hn->hn_MemoryType;

// printf( "Offset: %d\n", off );

	while( true )
	{
		chr = mem[off];

		type[off++] = MT_Data;

		if ( chr == 0 )
		{
			break;
		}
	}

	len = off - hl->hl_Label_Offset;

// printf( "Length: %d\n", len );

	hl->hl_Label_Type = LT_String;
	hl->hl_Label_Size = len;
}

// --

void Mark_D1_NulString( struct M68kStruct *ms )
{
struct HunkLabel *hl;

	if ( ms->ms_Registers[REG_D1].mr_Type != RT_Label )
	{
		goto bailout;
	}

	hl = ms->ms_Registers[REG_D1].mr_LabelNode;

	if ( hl == NULL )
	{
		goto bailout;
	}

	Mark_NulString( hl );

bailout:

	return;
}

// --

void Mark_D1D2_NulString( struct M68kStruct *ms )
{
struct HunkLabel *hl;

	if ( ms->ms_Registers[REG_D1].mr_Type != RT_Label )
	{
		goto next;
	}

	hl = ms->ms_Registers[REG_D1].mr_LabelNode;

	if ( hl == NULL )
	{
		goto next;
	}

	Mark_NulString( hl );

next:

	if ( ms->ms_Registers[REG_D2].mr_Type != RT_Label )
	{
		goto bailout;
	}

	hl = ms->ms_Registers[REG_D2].mr_LabelNode;

	if ( hl == NULL )
	{
		goto bailout;
	}

	Mark_NulString( hl );

bailout:

	return;
}

// --

void Mark_D2_NulString( struct M68kStruct *ms )
{
struct HunkLabel *hl;

	if ( ms->ms_Registers[REG_D2].mr_Type != RT_Label )
	{
		goto bailout;
	}

	hl = ms->ms_Registers[REG_D2].mr_LabelNode;

	if ( hl == NULL )
	{
		goto bailout;
	}

	Mark_NulString( hl );

bailout:

	return;
}

// --

void Mark_D3_NulString( struct M68kStruct *ms )
{
struct HunkLabel *hl;

	if ( ms->ms_Registers[REG_D3].mr_Type != RT_Label )
	{
		goto bailout;
	}

	hl = ms->ms_Registers[REG_D3].mr_LabelNode;

	if ( hl == NULL )
	{
		goto bailout;
	}

	Mark_NulString( hl );

bailout:

	return;
}

// --

void Mark_A0_NulString( struct M68kStruct *ms )
{
struct HunkLabel *hl;

	// A1 = String name
	if ( ms->ms_Registers[REG_A0].mr_Type != RT_Label )
	{
		goto bailout;
	}

	hl = ms->ms_Registers[REG_A0].mr_LabelNode;

	if ( hl == NULL )
	{
		goto bailout;
	}

	Mark_NulString( hl );

bailout:

	return;
}

// --

void Mark_A1_NulString( struct M68kStruct *ms )
{
struct HunkLabel *hl;

	if ( ms->ms_Registers[REG_A1].mr_Type != RT_Label )
	{
		goto bailout;
	}

	hl = ms->ms_Registers[REG_A1].mr_LabelNode;

	if ( hl == NULL )
	{
		goto bailout;
	}

	Mark_NulString( hl );

bailout:

	return;
}

// --

