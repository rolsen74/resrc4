
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

static int Global_Rexxsyslib_Used = false;

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
	{ 0, -126, "_LVOCreateArgstring", NULL },
	{ 0, -132, "_LVODeleteArgstring", NULL },
	{ 0, -138, "_LVOLengthArgstring", NULL },
	{ 0, -144, "_LVOCreateRexxMsg", NULL },
	{ 0, -150, "_LVODeleteRexxMsg", NULL },
	{ 0, -156, "_LVOClearRexxMsg", NULL },
	{ 0, -162, "_LVOFillRexxMsg", NULL },
	{ 0, -168, "_LVOIsRexxMsg", NULL },
	// --
	{ 0, -450, "_LVOLockRexxBase", NULL },
	{ 0, -456, "_LVOUnlockRexxBase", NULL },
	{ 0, 0, NULL, NULL }
};

char *M68k_Lib_Rexxsyslib( struct M68kStruct *ms, int16_t val )
{
char *lvo;
int pos;

	pos = 0;
	lvo = NULL;

	while( LVOS[pos].Name )
	{
		if ( LVOS[pos].Offset == val )
		{
			Global_Rexxsyslib_Used = true;
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
printf( "Unknown Rexxsyslib libcall %d at $%08x\n", val, ms->ms_MemoryAdr );
	}

	return( lvo );
}

// --

static char *Header ="\n; Rexxsyslib\n";

int Save_Lib_Rexxsyslib( void )
{
int error;
int pos;
int len;

	if ( Global_Rexxsyslib_Used == false )
	{
		error = false;
		goto bailout;
	}

	sprintf( SaveLineBuffer, "%s", Header );

	error = SaveWriteString( SaveLineBuffer, strlen( SaveLineBuffer ));

	if ( error )
	{
		printf( "%s:%04d: Error Writting Rexxsyslib lvo's\n", __FILE__, __LINE__ );
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
				printf( "%s:%04d: Error Writting Rexxsyslib Data\n", __FILE__, __LINE__ );
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
