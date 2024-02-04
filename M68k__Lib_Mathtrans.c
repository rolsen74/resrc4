
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

static int Global_Mathtrans_Used = false;

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
	{ 0, -30, "_LVOSPAtan", NULL },
	{ 0, -36, "_LVOSPSin", NULL },
	{ 0, -42, "_LVOSPCos", NULL },
	{ 0, -48, "_LVOSPTan", NULL },
	{ 0, -54, "_LVOSPSincos", NULL },
	{ 0, -60, "_LVOSPSinh", NULL },
	{ 0, -66, "_LVOSPCosh", NULL },
	{ 0, -72, "_LVOSPTanh", NULL },
	{ 0, -78, "_LVOSPExp", NULL },
	{ 0, -84, "_LVOSPLog", NULL },
	{ 0, -90, "_LVOSPPow", NULL },
	{ 0, -96, "_LVOSPSqrt", NULL },
	{ 0, -102, "_LVOSPTieee", NULL },
	{ 0, -108, "_LVOSPFieee", NULL },
	{ 0, -114, "_LVOSPAsin", NULL },
	{ 0, -120, "_LVOSPAcos", NULL },
	{ 0, -126, "_LVOSPLog10", NULL },
	{ 0, 0, NULL, NULL }
};

char *M68k_Lib_Mathtrans( struct M68kStruct *ms, int16_t val )
{
char *lvo;
int pos;

	pos = 0;
	lvo = NULL;

	while( LVOS[pos].Name )
	{
		if ( LVOS[pos].Offset == val )
		{
			Global_Mathtrans_Used = true;
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
printf( "Unknown Mathtrans libcall %d at $%08x\n", val, ms->ms_MemoryAdr );
	}

	return( lvo );
}

// --

static char *Header ="\n; Mathtrans\n";

int Save_Lib_Mathtrans( void )
{
int error;
int pos;
int len;

	if ( Global_Mathtrans_Used == false )
	{
		error = false;
		goto bailout;
	}

	sprintf( SaveLineBuffer, "%s", Header );

	error = SaveWriteString( SaveLineBuffer, strlen( SaveLineBuffer ));

	if ( error )
	{
		printf( "%s:%04d: Error Writting Mathtrans lvo's\n", __FILE__, __LINE__ );
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
				printf( "%s:%04d: Error Writting Mathtrans Data\n", __FILE__, __LINE__ );
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
