
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

static int Global_Amigaguide_Used = false;

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
	{ 0, -30, "_LVOAGARexxHost", NULL },
	{ 0, -36, "_LVOLockAmigaGuideBase", NULL },
	{ 0, -42, "_LVOUnlockAmigaGuideBase", NULL },
	{ 0, -54, "_LVOOpenAmigaGuideA", NULL },
	{ 0, -60, "_LVOOpenAmigaGuideAsyncA", NULL },
	{ 0, -66, "_LVOCloseAmigaGuide", NULL },
	{ 0, -72, "_LVOAmigaGuideSignal", NULL },
	{ 0, -78, "_LVOGetAmigaGuideMsg", NULL },
	{ 0, -84, "_LVOReplyAmigaGuideMsg", NULL },
	{ 0, -90, "_LVOSetAmigaGuideContextA", NULL },
	{ 0, -96, "_LVOSendAmigaGuideContextA", NULL },
	{ 0, -102, "_LVOSendAmigaGuideCmdA", NULL },
	{ 0, -108, "_LVOSetAmigaGuideAttrsA", NULL },
	{ 0, -114, "_LVOGetAmigaGuideAttr", NULL },
	{ 0, -138, "_LVOAddAmigaGuideHostA", NULL },
	{ 0, -144, "_LVORemoveAmigaGuideHostA", NULL },
	{ 0, -210, "_LVOGetAmigaGuideString", NULL },
	{ 0, 0, NULL, NULL }
};

char *M68k_Lib_Amigaguide( struct M68kStruct *ms, int16_t val )
{
char *lvo;
int pos;

	pos = 0;
	lvo = NULL;

	while( LVOS[pos].Name )
	{
		if ( LVOS[pos].Offset == val )
		{
			Global_Amigaguide_Used = true;
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
printf( "Unknown Amigaguide libcall %d at $%08x\n", val, ms->ms_MemoryAdr );
	}

	return( lvo );
}

// --

static char *Header ="\n; Amigaguide\n";

int Save_Lib_Amigaguide( void )
{
int error;
int pos;
int len;

	if ( Global_Amigaguide_Used == false )
	{
		error = false;
		goto bailout;
	}

	sprintf( SaveLineBuffer, "%s", Header );

	error = SaveWriteString( SaveLineBuffer, strlen( SaveLineBuffer ));

	if ( error )
	{
		printf( "%s:%04d: Error Writting Amigaguide lvo's\n", __FILE__, __LINE__ );
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
				printf( "%s:%04d: Error Writting Amigaguide Data\n", __FILE__, __LINE__ );
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
