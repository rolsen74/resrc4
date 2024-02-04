
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

static int Global_Commodities_Used = false;

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
	{ 0, -30, "_LVOCreateCxObj", NULL },
	{ 0, -36, "_LVOCxBroker", NULL },
	{ 0, -42, "_LVOActivateCxObj", NULL },
	{ 0, -48, "_LVODeleteCxObj", NULL },
	{ 0, -54, "_LVODeleteCxObjAll", NULL },
	{ 0, -60, "_LVOCxObjType", NULL },
	{ 0, -66, "_LVOCxObjError", NULL },
	{ 0, -72, "_LVOClearCxObjError", NULL },
	{ 0, -78, "_LVOSetCxObjPri", NULL },
	{ 0, -84, "_LVOAttachCxObj", NULL },
	{ 0, -90, "_LVOEnqueueCxObj", NULL },
	{ 0, -96, "_LVOInsertCxObj", NULL },
	{ 0, -102, "_LVORemoveCxObj", NULL },
	{ 0, -114, "_LVOSetTranslate", NULL },
	{ 0, -120, "_LVOSetFilter", NULL },
	{ 0, -126, "_LVOSetFilterIX", NULL },
	{ 0, -132, "_LVOParseIX", NULL },
	{ 0, -138, "_LVOCxMsgType", NULL },
	{ 0, -144, "_LVOCxMsgData", NULL },
	{ 0, -150, "_LVOCxMsgID", NULL },
	{ 0, -156, "_LVODivertCxMsg", NULL },
	{ 0, -162, "_LVORouteCxMsg", NULL },
	{ 0, -168, "_LVODisposeCxMsg", NULL },
	{ 0, -174, "_LVOInvertKeyMap", NULL },
	{ 0, -180, "_LVOAddIEvents", NULL },
	{ 0, -204, "_LVOMatchIX", NULL },
	{ 0, 0, NULL, NULL }
};

char *M68k_Lib_Commodities( struct M68kStruct *ms, int16_t val )
{
char *lvo;
int pos;

	pos = 0;
	lvo = NULL;

	while( LVOS[pos].Name )
	{
		if ( LVOS[pos].Offset == val )
		{
			Global_Commodities_Used = true;
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
printf( "Unknown Commodities libcall %d at $%08x\n", val, ms->ms_MemoryAdr );
	}

	return( lvo );
}

// --

static char *Header ="\n; Commodities\n";

int Save_Lib_Commodities( void )
{
int error;
int pos;
int len;

	if ( Global_Commodities_Used == false )
	{
		error = false;
		goto bailout;
	}

	sprintf( SaveLineBuffer, "%s", Header );

	error = SaveWriteString( SaveLineBuffer, strlen( SaveLineBuffer ));

	if ( error )
	{
		printf( "%s:%04d: Error Writting Commodities lvo's\n", __FILE__, __LINE__ );
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
				printf( "%s:%04d: Error Writting Commodities Data\n", __FILE__, __LINE__ );
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
