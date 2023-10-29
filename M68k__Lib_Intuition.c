
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

static int Global_Intuition_Used = false;

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
	{ 0, -30, "_LVOOpenIntuition", NULL },
	{ 0, -36, "_LVOIntuition", NULL },
	{ 0, -42, "_LVOAddGadget", NULL },
	{ 0, -48, "_LVOClearDMRequest", NULL },
	{ 0, -54, "_LVOClearMenuStrip", NULL },
	{ 0, -60, "_LVOClearPointer", NULL },
	{ 0, -66, "_LVOCloseScreen", NULL },
	{ 0, -72, "_LVOCloseWindow", NULL },
	{ 0, -78, "_LVOCloseWorkBench", NULL },
	{ 0, -84, "_LVOCurrentTime", NULL },
	{ 0, -90, "_LVODisplayAlert", NULL },
	{ 0, -96, "_LVODisplayBeep", NULL },
	{ 0, -102, "_LVODoubleClick", NULL },
	{ 0, -108, "_LVODrawBorder", NULL },
	{ 0, -114, "_LVODrawImage", NULL },
	{ 0, -120, "_LVOEndRequest", NULL },
	{ 0, -126, "_LVOGetDefPrefs", NULL },
	{ 0, -132, "_LVOGetPrefs", NULL },
	{ 0, -138, "_LVOInitRequester", NULL },
	{ 0, -144, "_LVOItemAddress", NULL },
	{ 0, -150, "_LVOModifyIDCMP", NULL },
	{ 0, -156, "_LVOModifyProp", NULL },
	{ 0, -162, "_LVOMoveScreen", NULL },
	{ 0, -168, "_LVOMoveWindow", NULL },
	{ 0, -174, "_LVOOffGadget", NULL },
	{ 0, -180, "_LVOOffMenu", NULL },
	{ 0, -186, "_LVOOnGadget", NULL },
	{ 0, -192, "_LVOOnMenu", NULL },
	{ 0, -198, "_LVOOpenScreen", NULL },
	{ 0, -204, "_LVOOpenWindow", NULL },
	{ 0, -210, "_LVOOpenWorkBench", NULL },
	{ 0, -216, "_LVOPrintIText", NULL },
	{ 0, -222, "_LVORefreshGadgets", NULL },
	{ 0, -228, "_LVORemoveGadget", NULL },
	{ 0, -234, "_LVOReportMouse", NULL },
	{ 0, -240, "_LVORequest", NULL },
	{ 0, -246, "_LVOScreenToBack", NULL },
	{ 0, -252, "_LVOScreenToFront", NULL },
	{ 0, -258, "_LVOSetDMRequest", NULL },
	{ 0, -264, "_LVOSetMenuStrip", NULL },
	{ 0, -270, "_LVOSetPointer", NULL },
	{ 0, -276, "_LVOSetWindowTitles", NULL },
	{ 0, -282, "_LVOShowTitle", NULL },
	{ 0, -288, "_LVOSizeWindow", NULL },
	{ 0, -294, "_LVOViewAddress", NULL },
	{ 0, -300, "_LVOViewPortAddress", NULL },
	{ 0, -306, "_LVOWindowToBack", NULL },
	{ 0, -312, "_LVOWindowToFront", NULL },
	{ 0, -318, "_LVOWindowLimits", NULL },
	{ 0, -324, "_LVOSetPrefs", NULL },
	{ 0, -330, "_LVOIntuiTextLength", NULL },
	{ 0, -336, "_LVOWBenchToBack", NULL },
	{ 0, -342, "_LVOWBenchToFront", NULL },
	{ 0, -348, "_LVOAutoRequest", NULL },
	{ 0, -354, "_LVOBeginRefresh", NULL },
	{ 0, -360, "_LVOBuildSysRequest", NULL },
	{ 0, -366, "_LVOEndRefresh", NULL },
	{ 0, -372, "_LVOFreeSysRequest", NULL },
	{ 0, -378, "_LVOMakeScreen", NULL },
	{ 0, -384, "_LVORemakeDisplay", NULL },
	{ 0, -390, "_LVORethinkDisplay", NULL },
	{ 0, -396, "_LVOAllocRemember", NULL },
	{ 0, -402, "_LVOAlohaWorkbench", NULL },
	{ 0, -408, "_LVOFreeRemember", NULL },
	{ 0, -414, "_LVOLockIBase", NULL },
	{ 0, -420, "_LVOUnlockIBase", NULL },
	{ 0, -426, "_LVOGetScreenData", NULL },
	{ 0, -432, "_LVORefreshGList", NULL },
	{ 0, -438, "_LVOAddGList", NULL },
	{ 0, -444, "_LVORemoveGList", NULL },
	{ 0, -450, "_LVOActivateWindow", NULL },
	{ 0, -456, "_LVORefreshWindowFrame", NULL },
	{ 0, -462, "_LVOActivateGadget", NULL },
	{ 0, -468, "_LVONewModifyProp", NULL },
	{ 0, -474, "_LVOQueryOverscan", NULL },
	{ 0, -480, "_LVOMoveWindowInFrontOf", NULL },
	{ 0, -486, "_LVOChangeWindowBox", NULL },
	{ 0, -492, "_LVOSetEditHook", NULL },
	{ 0, -498, "_LVOSetMouseQueue", NULL },
	{ 0, -504, "_LVOZipWindow", NULL },
	{ 0, -510, "_LVOLockPubScreen", NULL },
	{ 0, -516, "_LVOUnlockPubScreen", NULL },
	{ 0, -522, "_LVOLockPubScreenList", NULL },
	{ 0, -528, "_LVOUnlockPubScreenList", NULL },
	{ 0, -534, "_LVONextPubScreen", NULL },
	{ 0, -540, "_LVOSetDefaultPubScreen", NULL },
	{ 0, -546, "_LVOSetPubScreenModes", NULL },
	{ 0, -552, "_LVOPubScreenStatus", NULL },
	{ 0, -558, "_LVOObtainGIRPort", NULL },
	{ 0, -564, "_LVOReleaseGIRPort", NULL },
	{ 0, -570, "_LVOGadgetMouse", NULL },
	{ 0, -582, "_LVOGetDefaultPubScreen", NULL },
	{ 0, -588, "_LVOEasyRequestArgs", NULL },
	{ 0, -594, "_LVOBuildEasyRequestArgs", NULL },
	{ 0, -600, "_LVOSysReqHandler", NULL },
	{ 0, -606, "_LVOOpenWindowTagList", NULL },
	{ 0, -612, "_LVOOpenScreenTagList", NULL },
	{ 0, -618, "_LVODrawImageState", NULL },
	{ 0, -624, "_LVOPointInImage", NULL },
	{ 0, -630, "_LVOEraseImage", NULL },
	{ 0, -636, "_LVONewObjectA", NULL },
	{ 0, -642, "_LVODisposeObject", NULL },
	{ 0, -648, "_LVOSetAttrsA", NULL },
	{ 0, -654, "_LVOGetAttr", NULL },
	{ 0, -660, "_LVOSetGadgetAttrsA", NULL },
	{ 0, -660, "_LVONextObject", NULL },
	{ 0, -678, "_LVOMakeClass", NULL },
	{ 0, -684, "_LVOAddClass", NULL },
	{ 0, -690, "_LVOGetScreenDrawInfo", NULL },
	{ 0, -696, "_LVOFreeScreenDrawInfo", NULL },
	{ 0, -702, "_LVOResetMenuStrip", NULL },
	{ 0, -708, "_LVORemoveClass", NULL },
	{ 0, -714, "_LVOFreeClass", NULL },
	{ 0, -720, "_LVOlockPubClass", NULL },
	{ 0, -726, "_LVOunlockPubClass", NULL },
	// --
	{ 0, -768, "_LVOAllocScreenBuffer", NULL },
	{ 0, -774, "_LVOFreeScreenBuffer", NULL },
	{ 0, -780, "_LVOChangeScreenBuffer", NULL },
	{ 0, -786, "_LVOScreenDepth", NULL },
	{ 0, -792, "_LVOScreenPosition", NULL },
	{ 0, -798, "_LVOScrollWindowRaster", NULL },
	{ 0, -804, "_LVOLendMenus", NULL },
	{ 0, -810, "_LVODoGadgetMethodA", NULL },
	{ 0, -816, "_LVOSetWindowPointerA", NULL },
	{ 0, -822, "_LVOTimedDisplayAlert", NULL },
	{ 0, -828, "_LVOHelpControl", NULL },
	{ 0, 0, NULL, NULL }
};

char *M68k_Lib_Intuition( struct M68kStruct *ms, int16_t val )
{
char *lvo;
int pos;

	pos = 0;
	lvo = NULL;

	while( LVOS[pos].Name )
	{
		if ( LVOS[pos].Offset == val )
		{
			Global_Intuition_Used = true;
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
printf( "Unknown Intuition libcall %d at $%08x\n", val, ms->ms_MemoryAdr );
	}

	return( lvo );
}

// --

static char *Header ="\n; Intuition\n";

int Save_Lib_Intuition( void )
{
int error;
int pos;
int len;

	if ( Global_Intuition_Used == false )
	{
		error = false;
		goto bailout;
	}

	sprintf( SaveLineBuffer, "%s", Header );

	error = SaveWriteString( SaveLineBuffer, strlen( SaveLineBuffer ));

	if ( error )
	{
		printf( "%s:%04d: Error Writting Intuition lvo's\n", __FILE__, __LINE__ );
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
				printf( "%s:%04d: Error Writting Intuition Data\n", __FILE__, __LINE__ );
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
