
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

static int Global_Utility_Used = false;

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
	{ 0, -30, "_LVOFindTagItem", NULL },
	{ 0, -36, "_LVOGetTagData", NULL },
	{ 0, -42, "_LVOPackBoolTags", NULL },
	{ 0, -48, "_LVONextTagItem", NULL },
	{ 0, -54, "_LVOFilterTagChanges", NULL },
	{ 0, -60, "_LVOMapTags", NULL },
	{ 0, -66, "_LVOAllocateTagItems", NULL },
	{ 0, -72, "_LVOCloneTagItems", NULL },
	{ 0, -78, "_LVOFreeTagItems", NULL },
	{ 0, -84, "_LVORefreshTagItemClones", NULL },
	{ 0, -90, "_LVOTagInArray", NULL },
	{ 0, -96, "_LVOFilterTagItems", NULL },
	{ 0, -102, "_LVOCallHookPkt", NULL },
	{ 0, -120, "_LVOAmiga2Date", NULL },
	{ 0, -126, "_LVODate2Amiga", NULL },
	{ 0, -132, "_LVOCheckDate", NULL },
	{ 0, -138, "_LVOSMult32", NULL },
	{ 0, -144, "_LVOUMult32", NULL },
	{ 0, -150, "_LVOSDivMod32", NULL },
	{ 0, -156, "_LVOUDivMod32", NULL },
	{ 0, -162, "_LVOStricmp", NULL },
	{ 0, -168, "_LVOStrnicmp", NULL },
	{ 0, -174, "_LVOToUpper", NULL },
	{ 0, -180, "_LVOToLower", NULL },
	{ 0, -186, "_LVOApplyTagChanges", NULL },
	{ 0, -198, "_LVOSMult64", NULL },
	{ 0, -204, "_LVOUMult64", NULL },
	{ 0, -210, "_LVOPackStructureTags", NULL },
	{ 0, -216, "_LVOUnpackStructureTags", NULL },
	{ 0, -222, "_LVOAddNamedObject", NULL },
	{ 0, -228, "_LVOAllocNamedObjectA", NULL },
	{ 0, -234, "_LVOAttemptRemNamedObject", NULL },
	{ 0, -240, "_LVOFindNamedObject", NULL },
	{ 0, -246, "_LVOFreeNamedObject", NULL },
	{ 0, -252, "_LVONamedObjectName", NULL },
	{ 0, -258, "_LVOReleaseNamedObject", NULL },
	{ 0, -264, "_LVORemNamedObject", NULL },
	{ 0, -270, "_LVOGetUniqueID", NULL },
	{ 0, 0, NULL, NULL }
};

char *M68k_Lib_Utility( struct M68kStruct *ms, int16_t val )
{
char *lvo;
int pos;

	pos = 0;
	lvo = NULL;

	while( LVOS[pos].Name )
	{
		if ( LVOS[pos].Offset == val )
		{
			Global_Utility_Used = true;
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
printf( "Unknown Utility libcall %d at $%08x\n", val, ms->ms_MemoryAdr );
	}

	return( lvo );
}

// --

static char *Header ="\n; Utility\n";

int Save_Lib_Utility( void )
{
int error;
int pos;
int len;

	if ( Global_Utility_Used == false )
	{
		error = false;
		goto bailout;
	}

	sprintf( SaveLineBuffer, "%s", Header );

	error = SaveWriteString( SaveLineBuffer, strlen( SaveLineBuffer ));

	if ( error )
	{
		printf( "%s:%04d: Error Writting Utility lvo's\n", __FILE__, __LINE__ );
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
				printf( "%s:%04d: Error Writting Utility Data\n", __FILE__, __LINE__ );
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
