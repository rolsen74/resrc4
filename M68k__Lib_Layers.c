
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

static int Global_Layers_Used = false;

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
	{ 0, -30, "_LVOInitLayers", NULL },
	{ 0, -36, "_LVOCreateUpfrontLayer", NULL },
	{ 0, -42, "_LVOCreateBehindLayer", NULL },
	{ 0, -48, "_LVOUpfrontLayer", NULL },
	{ 0, -54, "_LVOBehindLayer", NULL },
	{ 0, -60, "_LVOMoveLayer", NULL },
	{ 0, -66, "_LVOSizeLayer", NULL },
	{ 0, -72, "_LVOScrollLayer", NULL },
	{ 0, -78, "_LVOBeginUpdate", NULL },
	{ 0, -84, "_LVOEndUpdate", NULL },
	{ 0, -90, "_LVODeleteLayer", NULL },
	{ 0, -96, "_LVOLockLayer", NULL },
	{ 0, -102, "_LVOUnlockLayer", NULL },
	{ 0, -108, "_LVOLockLayers", NULL },
	{ 0, -114, "_LVOUnlockLayers", NULL },
	{ 0, -120, "_LVOLockLayerInfo", NULL },
	{ 0, -126, "_LVOSwapBitsRastPortClipRect", NULL },
	{ 0, -132, "_LVOWhichLayer", NULL },
	{ 0, -138, "_LVOUnlockLayerInfo", NULL },
	{ 0, -144, "_LVONewLayerInfo", NULL },
	{ 0, -150, "_LVODisposeLayerInfo", NULL },
	{ 0, -156, "_LVOFattenLayerInfo", NULL },
	{ 0, -162, "_LVOThinLayerInfo", NULL },
	{ 0, -168, "_LVOMoveLayerInFrontOf", NULL },
	{ 0, -174, "_LVOInstallClipRegion", NULL },
	{ 0, -180, "_LVOMoveSizeLayer", NULL },
	{ 0, -186, "_LVOCreateUpfrontHookLayer", NULL },
	{ 0, -192, "_LVOCreateBehindHookLayer", NULL },
	{ 0, -198, "_LVOInstallLayerHook", NULL },
	{ 0, -204, "_LVOInstallLayerInfoHook", NULL },
	{ 0, -210, "_LVOSortLayerCR", NULL },
	{ 0, -216, "_LVODoHookClipRects", NULL },
	{ 0, 0, NULL, NULL }
};

char *M68k_Lib_Layers( struct M68kStruct *ms, int16_t val )
{
char *lvo;
int pos;

	pos = 0;
	lvo = NULL;

	while( LVOS[pos].Name )
	{
		if ( LVOS[pos].Offset == val )
		{
			Global_Layers_Used = true;
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
printf( "Unknown Layers libcall %d at $%08x\n", val, ms->ms_MemoryAdr );
	}

	return( lvo );
}

// --

static char *Header ="\n; Layers\n";

int Save_Lib_Layers( void )
{
int error;
int pos;
int len;

	if ( Global_Layers_Used == false )
	{
		error = false;
		goto bailout;
	}

	sprintf( SaveLineBuffer, "%s", Header );

	error = SaveWriteString( SaveLineBuffer, strlen( SaveLineBuffer ));

	if ( error )
	{
		printf( "%s:%04d: Error Writting Layers lvo's\n", __FILE__, __LINE__ );
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
				printf( "%s:%04d: Error Writting Layers Data\n", __FILE__, __LINE__ );
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
