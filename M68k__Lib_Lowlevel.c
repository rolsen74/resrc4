
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

static int Global_Lowlevel_Used = false;

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
	{ 0, -30, "_LVOReadJoyPort", NULL },
	{ 0, -36, "_LVOGetLanguageSelection", NULL },
//	{ 0, -42, "_LVOlowlevelPrivate1", NULL },
	{ 0, -48, "_LVOGetKey", NULL },
	{ 0, -54, "_LVOQueryKeys", NULL },
	{ 0, -60, "_LVOAddKBInt", NULL },
	{ 0, -66, "_LVORemKBInt", NULL },
	{ 0, -72, "_LVOSystemControlA", NULL },
	{ 0, -78, "_LVOAddTimerInt", NULL },
	{ 0, -84, "_LVORemTimerInt", NULL },
	{ 0, -90, "_LVOStopTimerInt", NULL },
	{ 0, -96, "_LVOStartTimerInt", NULL },
	{ 0, -102, "_LVOElapsedTime", NULL },
	{ 0, -108, "_LVOAddVBlankInt", NULL },
	{ 0, -114, "_LVORemVBlankInt", NULL },
//	{ 0, -120, "_LVOlowlevelPrivate2", NULL },
//	{ 0, -126, "_LVOlowlevelPrivate3", NULL },
	{ 0, -132, "_LVOSetJoyPortAttrsA", NULL },
//	{ 0, -138, "_LVOlowlevelPrivate4", NULL },
//	{ 0, -144, "_LVOlowlevelPrivate5", NULL },
//	{ 0, -150, "_LVOlowlevelPrivate6", NULL },
//	{ 0, -156, "_LVOlowlevelPrivate7", NULL },
//	{ 0, -162, "_LVOlowlevelPrivate8", NULL },
	{ 0, 0, NULL, NULL }
};

char *M68k_Lib_Lowlevel( struct M68kStruct *ms, int16_t val )
{
char *lvo;
int pos;

	pos = 0;
	lvo = NULL;

	while( LVOS[pos].Name )
	{
		if ( LVOS[pos].Offset == val )
		{
			Global_Lowlevel_Used = true;
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
printf( "Unknown Lowlevel libcall %d at $%08x\n", val, ms->ms_MemoryAdr );
	}

	return( lvo );
}

// --

static char *Header ="\n; Lowlevel\n";

int Save_Lib_Lowlevel( void )
{
int error;
int pos;
int len;

	if ( Global_Lowlevel_Used == false )
	{
		error = false;
		goto bailout;
	}

	sprintf( SaveLineBuffer, "%s", Header );

	error = SaveWriteString( SaveLineBuffer, strlen( SaveLineBuffer ));

	if ( error )
	{
		printf( "%s:%04d: Error Writting Lowlevel lvo's\n", __FILE__, __LINE__ );
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
				printf( "%s:%04d: Error Writting Lowlevel Data\n", __FILE__, __LINE__ );
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
