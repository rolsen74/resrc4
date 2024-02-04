
/*
 * Copyright (c) 2014-2024 Rene W. Olsen < renewolsen @ gmail . com >
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

static int Global_IFFParse_Used = false;

struct lvostruct
{
	int16_t	Used;
	int16_t	Offset;
	char *	Name;
	void 	(*Func)( struct M68kStruct *ms );
};

// --


// --

static struct lvostruct LVOS[] =
{
	{ 0, -30, "_LVOAllocIFF", NULL },
	{ 0, -36, "_LVOOpenIFF", NULL },
	{ 0, -42, "_LVOParseIFF", NULL },
	{ 0, -48, "_LVOCloseIFF", NULL },
	{ 0, -54, "_LVOFreeIFF", NULL },
	{ 0, -60, "_LVOReadChunkBytes", NULL },
	{ 0, -66, "_LVOWriteChunkBytes", NULL },
	{ 0, -72, "_LVOReadChunkRecords", NULL },
	{ 0, -78, "_LVOWriteChunkRecords", NULL },
	{ 0, -84, "_LVOPushChunk", NULL },
	{ 0, -90, "_LVOPopChunk", NULL },
	{ 0, -102, "_LVOEntryHandler", NULL },
	{ 0, -108, "_LVOExitHandler", NULL },
	{ 0, -114, "_LVOPropChunk", NULL },
	{ 0, -120, "_LVOPropChunks", NULL },
	{ 0, -126, "_LVOStopChunk", NULL },
	{ 0, -132, "_LVOStopChunks", NULL },
	{ 0, -138, "_LVOCollectionChunk", NULL },
	{ 0, -144, "_LVOCollectionChunks", NULL },
	{ 0, -150, "_LVOStopOnExit", NULL },
	{ 0, -156, "_LVOFindProp", NULL },
	{ 0, -162, "_LVOFindCollection", NULL },
	{ 0, -168, "_LVOFindPropContext", NULL },
	{ 0, -174, "_LVOCurrentChunk", NULL },
	{ 0, -180, "_LVOParentChunk", NULL },
	{ 0, -186, "_LVOAllocLocalItem", NULL },
	{ 0, -192, "_LVOLocalItemData", NULL },
	{ 0, -198, "_LVOSetLocalItemPurge", NULL },
	{ 0, -204, "_LVOFreeLocalItem", NULL },
	{ 0, -210, "_LVOFindLocalItem", NULL },
	{ 0, -216, "_LVOStoreLocalItem", NULL },
	{ 0, -222, "_LVOStoreItemInContext", NULL },
	{ 0, -228, "_LVOInitIFF", NULL },
	{ 0, -234, "_LVOInitIFFasDOS", NULL },
	{ 0, -240, "_LVOInitIFFasClip", NULL },
	{ 0, -246, "_LVOOpenClipboard", NULL },
	{ 0, -252, "_LVOCloseClipboard", NULL },
	{ 0, -258, "_LVOGoodID", NULL },
	{ 0, -264, "_LVOGoodType", NULL },
	{ 0, -270, "_LVOIDtoStr", NULL },
	{ 0, 0, NULL, NULL }
};

char *M68k_Lib_IFFParse( struct M68kStruct *ms, int16_t val )
{
char *lvo;
int pos;

	pos = 0;
	lvo = NULL;

	while( LVOS[pos].Name )
	{
		if ( LVOS[pos].Offset == val )
		{
			Global_IFFParse_Used = true;
			LVOS[pos].Used = true;
			lvo = LVOS[pos].Name;

			if ( LVOS[pos].Func )
			{
				LVOS[pos].Func( ms );
			}
			break;
		}
		else
		{
			pos++;
		}
	}

	if ( lvo == NULL )
	{
printf( "Unknown IFFParse libcall %d at $%08x\n", val, ms->ms_MemoryAdr );
	}

	return( lvo );
}

// --

static char *Header ="\n; IFFParse\n";

int Save_Lib_IFFParse( void )
{
int error;
int pos;
int len;

	if ( Global_IFFParse_Used == false )
	{
		error = false;
		goto bailout;
	}

	sprintf( SaveLineBuffer, "%s", Header );

	error = SaveWriteString( SaveLineBuffer, strlen( SaveLineBuffer ));

	if ( error )
	{
		printf( "%s:%04d: Error Writting IFFParse lvo's\n", __FILE__, __LINE__ );
		goto bailout;
	}

	pos = 0;

	while( LVOS[pos].Name )
	{
		if ( LVOS[pos].Used )
		{
			len = strlen( LVOS[pos].Name ) + 1;

			if ( len < 8 )
			{
				sprintf( SaveLineBuffer, "%s:\t\t\tEQU\t%d\n", LVOS[pos].Name, LVOS[pos].Offset );
			}
			else if ( len < 16 )
			{
				sprintf( SaveLineBuffer, "%s:\t\tEQU\t%d\n", LVOS[pos].Name, LVOS[pos].Offset );
			}
			else if ( len < 24 )
			{
				sprintf( SaveLineBuffer, "%s:\tEQU\t%d\n", LVOS[pos].Name, LVOS[pos].Offset );
			}
			else
			{
				sprintf( SaveLineBuffer, "%s: EQU\t%d\n", LVOS[pos].Name, LVOS[pos].Offset );
			}

			error = SaveWriteString( SaveLineBuffer, strlen( SaveLineBuffer ));

			if ( error )
			{
				printf( "%s:%04d: Error Writting IFFParse Data\n", __FILE__, __LINE__ );
				goto bailout;
			}
		}

		pos++;
	}

	error = false;

bailout:

	return( error );
}

// --
