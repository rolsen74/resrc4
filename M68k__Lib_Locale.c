
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

static int Global_Locale_Used = false;

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
	{ 0, -36, "_LVOCloseCatalog", NULL },
	{ 0, -42, "_LVOCloseLocale", NULL },
	{ 0, -48, "_LVOConvToLower", NULL },
	{ 0, -54, "_LVOConvToUpper", NULL },
	{ 0, -60, "_LVOFormatDate", NULL },
	{ 0, -66, "_LVOFormatString", NULL },
	{ 0, -72, "_LVOGetCatalogStr", NULL },
	{ 0, -78, "_LVOGetLocaleStr", NULL },
	{ 0, -84, "_LVOIsAlNum", NULL },
	{ 0, -90, "_LVOIsAlpha", NULL },
	{ 0, -96, "_LVOIsCntrl", NULL },
	{ 0, -102, "_LVOIsDigit", NULL },
	{ 0, -108, "_LVOIsGraph", NULL },
	{ 0, -114, "_LVOIsLower", NULL },
	{ 0, -120, "_LVOIsPrint", NULL },
	{ 0, -126, "_LVOIsPunct", NULL },
	{ 0, -132, "_LVOIsSpace", NULL },
	{ 0, -138, "_LVOIsUpper", NULL },
	{ 0, -144, "_LVOIsXDigit", NULL },
	{ 0, -150, "_LVOOpenCatalogA", NULL },
	{ 0, -156, "_LVOOpenLocale", NULL },
	{ 0, -162, "_LVOParseDate", NULL },
	{ 0, -174, "_LVOStrConvert", NULL },
	{ 0, -180, "_LVOStrnCmp", NULL },
	{ 0, 0, NULL, NULL }
};

char *M68k_Lib_Locale( struct M68kStruct *ms, int16_t val )
{
char *lvo;
int pos;

	pos = 0;
	lvo = NULL;

	while( LVOS[pos].Name )
	{
		if ( LVOS[pos].Offset == val )
		{
			Global_Locale_Used = true;
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
printf( "Unknown Locale libcall %d at $%08x\n", val, ms->ms_MemoryAdr );
	}

	return( lvo );
}

// --

static char *Header ="\n; Locale\n";

int Save_Lib_Locale( void )
{
int error;
int pos;
int len;

	if ( Global_Locale_Used == false )
	{
		error = false;
		goto bailout;
	}

	sprintf( SaveLineBuffer, "%s", Header );

	error = SaveWriteString( SaveLineBuffer, strlen( SaveLineBuffer ));

	if ( error )
	{
		printf( "%s:%04d: Error Writting Locale lvo's\n", __FILE__, __LINE__ );
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
				printf( "%s:%04d: Error Writting Locale Data\n", __FILE__, __LINE__ );
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
