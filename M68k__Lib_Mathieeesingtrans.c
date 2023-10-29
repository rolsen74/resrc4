
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

static int Global_Mathieeesingtrans_Used = false;

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
	{ 0, -30, "_LVOIEEESPAtan", NULL },
	{ 0, -36, "_LVOIEEESPSin", NULL },
	{ 0, -42, "_LVOIEEESPCos", NULL },
	{ 0, -48, "_LVOIEEESPTan", NULL },
	{ 0, -54, "_LVOIEEESPSincos", NULL },
	{ 0, -60, "_LVOIEEESPSinh", NULL },
	{ 0, -66, "_LVOIEEESPCosh", NULL },
	{ 0, -72, "_LVOIEEESPTanh", NULL },
	{ 0, -78, "_LVOIEEESPExp", NULL },
	{ 0, -84, "_LVOIEEESPLog", NULL },
	{ 0, -90, "_LVOIEEESPPow", NULL },
	{ 0, -96, "_LVOIEEESPSqrt", NULL },
	{ 0, -102, "_LVOIEEESPTieee", NULL },
	{ 0, -108, "_LVOIEEESPFieee", NULL },
	{ 0, -114, "_LVOIEEESPAsin", NULL },
	{ 0, -120, "_LVOIEEESPAcos", NULL },
	{ 0, -126, "_LVOIEEESPLog10", NULL },
	{ 0, 0, NULL, NULL }
};

char *M68k_Lib_Mathieeesingtrans( struct M68kStruct *ms, int16_t val )
{
char *lvo;
int pos;

	pos = 0;
	lvo = NULL;

	while( LVOS[pos].Name )
	{
		if ( LVOS[pos].Offset == val )
		{
			Global_Mathieeesingtrans_Used = true;
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
printf( "Unknown Mathieeesingtrans libcall %d at $%08x\n", val, ms->ms_MemoryAdr );
	}

	return( lvo );
}

// --

static char *Header ="\n; Mathieeesingtrans\n";

int Save_Lib_Mathieeesingtrans( void )
{
int error;
int pos;
int len;

	if ( Global_Mathieeesingtrans_Used == false )
	{
		error = false;
		goto bailout;
	}

	sprintf( SaveLineBuffer, "%s", Header );

	error = SaveWriteString( SaveLineBuffer, strlen( SaveLineBuffer ));

	if ( error )
	{
		printf( "%s:%04d: Error Writting Mathieeesingtrans lvo's\n", __FILE__, __LINE__ );
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
				printf( "%s:%04d: Error Writting Mathieeesingtrans Data\n", __FILE__, __LINE__ );
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
