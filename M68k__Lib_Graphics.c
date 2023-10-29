
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

static int Global_Graphics_Used = false;

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
	{ 0, -30, "_LVOBltBitMap", NULL },
	{ 0, -36, "_LVOBltTemplate", NULL },
	{ 0, -42, "_LVOClearEOL", NULL },
	{ 0, -48, "_LVOClearScreen", NULL },
	{ 0, -54, "_LVOTextLength", NULL },
	{ 0, -60, "_LVOText", NULL },
	{ 0, -66, "_LVOSetFont", NULL },
	{ 0, -72, "_LVOOpenFont", NULL },
	{ 0, -78, "_LVOCloseFont", NULL },
	{ 0, -84, "_LVOAskSoftStyle", NULL },
	{ 0, -90, "_LVOSetSoftStyle", NULL },
	{ 0, -96, "_LVOAddBob", NULL },
	{ 0, -102, "_LVOAddVSprite", NULL },
	{ 0, -108, "_LVODoCollision", NULL },
	{ 0, -114, "_LVODrawGList", NULL },
	{ 0, -120, "_LVOInitGels", NULL },
	{ 0, -126, "_LVOInitMasks", NULL },
	{ 0, -132, "_LVORemIBob", NULL },
	{ 0, -138, "_LVORemVSprite", NULL },
	{ 0, -144, "_LVOSetCollision", NULL },
	{ 0, -150, "_LVOSortGList", NULL },
	{ 0, -156, "_LVOAddAnimOb", NULL },
	{ 0, -162, "_LVOAnimate", NULL },
	{ 0, -168, "_LVOGetGBuffers", NULL },
	{ 0, -174, "_LVOInitGMasks", NULL },
	{ 0, -180, "_LVODrawEllipse", NULL },
	{ 0, -186, "_LVOAreaEllipse", NULL },
	{ 0, -192, "_LVOLoadRGB4", NULL },
	{ 0, -198, "_LVOInitRastPort", NULL },
	{ 0, -204, "_LVOInitVPort", NULL },
	{ 0, -210, "_LVOMrgCop", NULL },
	{ 0, -216, "_LVOMakeVPort", NULL },
	{ 0, -222, "_LVOLoadView", NULL },
	{ 0, -228, "_LVOWaitBlit", NULL },
	{ 0, -234, "_LVOSetRast", NULL },
	{ 0, -240, "_LVOMove", NULL },
	{ 0, -246, "_LVODraw", NULL },
	{ 0, -252, "_LVOAreaMove", NULL },
	{ 0, -258, "_LVOAreaDraw", NULL },
	{ 0, -264, "_LVOAreaEnd", NULL },
	{ 0, -270, "_LVOWaitTOF", NULL },
	{ 0, -276, "_LVOQBlit", NULL },
	{ 0, -282, "_LVOInitArea", NULL },
	{ 0, -288, "_LVOSetRGB4", NULL },
	{ 0, -294, "_LVOQBSBlit", NULL },
	{ 0, -300, "_LVOBltClear", NULL },
	{ 0, -306, "_LVORectFill", NULL },
	{ 0, -312, "_LVOBltPattern", NULL },
	{ 0, -318, "_LVOReadPixel", NULL },
	{ 0, -324, "_LVOWritePixel", NULL },
	{ 0, -330, "_LVOFlood", NULL },
	{ 0, -336, "_LVOPolyDraw", NULL },
	{ 0, -342, "_LVOSetAPen", NULL },
	{ 0, -348, "_LVOSetBPen", NULL },
	{ 0, -354, "_LVOSetDrMd", NULL },
	{ 0, -360, "_LVOInitView", NULL },
	{ 0, -366, "_LVOCBump", NULL },
	{ 0, -372, "_LVOCMove", NULL },
	{ 0, -378, "_LVOCWait", NULL },
	{ 0, -384, "_LVOVBeamPos", NULL },
	{ 0, -390, "_LVOInitBitMap", NULL },
	{ 0, -396, "_LVOScrollRaster", NULL },
	{ 0, -402, "_LVOWaitBOVP", NULL },
	{ 0, -408, "_LVOGetSprite", NULL },
	{ 0, -414, "_LVOFreeSprite", NULL },
	{ 0, -420, "_LVOChangeSprite", NULL },
	{ 0, -426, "_LVOMoveSprite", NULL },
	{ 0, -432, "_LVOLockLayerRom", NULL },
	{ 0, -438, "_LVOUnlockLayerRom", NULL },
	{ 0, -444, "_LVOSyncSBitMap", NULL },
	{ 0, -450, "_LVOCopySBitMap", NULL },
	{ 0, -456, "_LVOOwnBlitter", NULL },
	{ 0, -462, "_LVODisownBlitter", NULL },
	{ 0, -468, "_LVOInitTmpRas", NULL },
	{ 0, -474, "_LVOAskFont", NULL },
	{ 0, -480, "_LVOAddFont", NULL },
	{ 0, -486, "_LVORemFont", NULL },
	{ 0, -492, "_LVOAllocRaster", NULL },
	{ 0, -498, "_LVOFreeRaster", NULL },
	{ 0, -504, "_LVOAndRectRegion", NULL },
	{ 0, -510, "_LVOOrRectRegion", NULL },
	{ 0, -516, "_LVONewRegion", NULL },
	{ 0, -522, "_LVOClearRectRegion", NULL },
	{ 0, -528, "_LVOClearRegion", NULL },
	{ 0, -534, "_LVODisposeRegion", NULL },
	{ 0, -540, "_LVOFreeVPortCopLists", NULL },
	{ 0, -546, "_LVOFreeCopList", NULL },
	{ 0, -552, "_LVOClipBlit", NULL },
	{ 0, -558, "_LVOXorRectRegion", NULL },
	{ 0, -564, "_LVOFreeCprList", NULL },
	{ 0, -570, "_LVOGetColorMap", NULL },
	{ 0, -576, "_LVOFreeColorMap", NULL },
	{ 0, -582, "_LVOGetRGB4", NULL },
	{ 0, -588, "_LVOScrollVPort", NULL },
	{ 0, -594, "_LVOUCopperListInit", NULL },
	{ 0, -600, "_LVOFreeGBuffers", NULL },
	{ 0, -606, "_LVOBltBitMapRastPort", NULL },
	{ 0, -612, "_LVOOrRegionRegion", NULL },
	{ 0, -618, "_LVOXorRegionRegion", NULL },
	{ 0, -624, "_LVOAndRegionRegion", NULL },
	{ 0, -630, "_LVOSetRGB4CM", NULL },
	{ 0, -636, "_LVOBltMaskBitMapRastPort", NULL },
	{ 0, -642, "_LVOGfxInternal1", NULL },
	{ 0, -648, "_LVOGfxInternal2", NULL },
	{ 0, -654, "_LVOAttemptLockLayerRom", NULL },
	{ 0, -660, "_LVOGfxNew", NULL },
	{ 0, -666, "_LVOGfxFree", NULL },
	{ 0, -672, "_LVOGfxAssociate", NULL },
	{ 0, -678, "_LVOBitMapScale", NULL },
	{ 0, -684, "_LVOScalerDiv", NULL },
	{ 0, -690, "_LVOTextExtent", NULL },
	{ 0, -696, "_LVOTextFit", NULL },
	{ 0, -702, "_LVOGfxLookUp", NULL },
	{ 0, -708, "_LVOVideoControl", NULL },
	{ 0, -714, "_LVOOpenMonitor", NULL },
	{ 0, -720, "_LVOCloseMonitor", NULL },
	{ 0, -726, "_LVOFindDisplayInfo", NULL },
	{ 0, -732, "_LVONextDisplayInfo", NULL },
	{ 0, -756, "_LVOGetDisplayInfoData", NULL },
	{ 0, -762, "_LVOFontExtent", NULL },
	{ 0, -768, "_LVOReadPixelLine8", NULL },
	{ 0, -774, "_LVOWritePixelLine8", NULL },
	{ 0, -780, "_LVOReadPixelArray8", NULL },
	{ 0, -786, "_LVOWritePixelArray8", NULL },
	{ 0, -792, "_LVOGetVPModeID", NULL },
	{ 0, -798, "_LVOModeNotAvailable", NULL },
	{ 0, -804, "_LVOWeighTAMatch", NULL },
	{ 0, -810, "_LVOEraseRect", NULL },
	{ 0, -816, "_LVOExtendFont", NULL },
	{ 0, -822, "_LVOStripFont", NULL },
	{ 0, -828, "_LVOCalcIVG", NULL },
	{ 0, -834, "_LVOAttachPalExtra", NULL },
	{ 0, -840, "_LVOObtainBestPenA", NULL },
	{ 0, -846, "_LVOGfxInternal3", NULL },
	{ 0, -852, "_LVOSetRGB32", NULL },
	{ 0, -858, "_LVOGetAPen", NULL },
	{ 0, -864, "_LVOGetBPen", NULL },
	{ 0, -870, "_LVOGetDrMd", NULL },
	{ 0, -876, "_LVOGetOutlinePen", NULL },
	{ 0, -882, "_LVOLoadRGB32", NULL },
	{ 0, -888, "_LVOSetChipRev", NULL },
	{ 0, -894, "_LVOSetABPenDrMd", NULL },
	{ 0, -900, "_LVOGetRGB32", NULL },
	{ 0, -906, "_LVOGfxSpare1", NULL },
	{ 0, -918, "_LVOAllocBitMap", NULL },
	{ 0, -924, "_LVOFreeBitMap", NULL },
	{ 0, -930, "_LVOGetExtSpriteA", NULL },
	{ 0, -936, "_LVOCoerceMode", NULL },
	{ 0, -942, "_LVOChangeVPBitMap", NULL },
	{ 0, -948, "_LVOReleasePen", NULL },
	{ 0, -954, "_LVOObtainPen", NULL },
	{ 0, -960, "_LVOGetBitMapAttr", NULL },
	{ 0, -966, "_LVOAllocDBufInfo", NULL },
	{ 0, -972, "_LVOFreeDBufInfo", NULL },
	{ 0, -978, "_LVOSetOutlinePen", NULL },
	{ 0, -984, "_LVOSetWriteMask", NULL },
	{ 0, -990, "_LVOSetMaxPen", NULL },
	{ 0, -996, "_LVOSetRGB32CM", NULL },
	{ 0, -1002, "_LVOScrollRasterBF", NULL },
	{ 0, -1008, "_LVOFindColor", NULL },
	{ 0, -1014, "_LVOGfxSpare2", NULL },
	{ 0, -1020, "_LVOAllocSpriteDataA", NULL },
	{ 0, -1026, "_LVOChangeExtSpriteA", NULL },
	{ 0, -1032, "_LVOFreeSpriteData", NULL },
	{ 0, -1038, "_LVOSetRPAttrsA", NULL },
	{ 0, -1044, "_LVOGetRPAttrsA", NULL },
	{ 0, -1050, "_LVOBestModeIDA", NULL },
	{ 0, 0, NULL, NULL }
};

char *M68k_Lib_Graphics( struct M68kStruct *ms, int16_t val )
{
char *lvo;
int pos;

	pos = 0;
	lvo = NULL;

	while( LVOS[pos].Name )
	{
		if ( LVOS[pos].Offset == val )
		{
			Global_Graphics_Used = true;
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
printf( "Unknown Graphics libcall %d at $%08x\n", val, ms->ms_MemoryAdr );
	}

	return( lvo );
}

// --

static char *Header ="\n; Graphics\n";

int Save_Lib_Graphics( void )
{
int error;
int pos;
int len;

	if ( Global_Graphics_Used == false )
	{
		error = false;
		goto bailout;
	}

	sprintf( SaveLineBuffer, "%s", Header );

	error = SaveWriteString( SaveLineBuffer, strlen( SaveLineBuffer ));

	if ( error )
	{
		printf( "%s:%04d: Error Writting Graphics lvo's\n", __FILE__, __LINE__ );
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
				printf( "%s:%04d: Error Writting Graphics Data\n", __FILE__, __LINE__ );
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
