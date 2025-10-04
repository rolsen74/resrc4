
/*
** Copyright (c) 2014-2025 Rene W. Olsen
**
** SPDX-License-Identifier: GPL-3.0-or-later
**
** This software is released under the GNU General Public License, version 3.
** For the full text of the license, please visit:
** https://www.gnu.org/licenses/gpl-3.0.html
**
** You can also find a copy of the license in the LICENSE file included with this software.
*/

// --

#include "Resourcer/ReSrc4.h"

// --
// Single Args structs

static AOS3_RegStruct _BitMap_A0[] =
{
	AOS3_REG_Struct( M68KREGT_A0, RS4StructID_BitMap ),
	AOS3_REG_EndMarker()
};

static AOS3_RegStruct _RastPort_A1[] =
{
	AOS3_REG_Struct( M68KREGT_A1, RS4StructID_RastPort ),
	AOS3_REG_EndMarker()
};

static AOS3_RegStruct _TextAttr_A0[] =
{
	AOS3_REG_Struct( M68KREGT_A0, RS4StructID_TextAttr ),
	AOS3_REG_EndMarker()
};

static AOS3_RegStruct _TextFont_A0[] =
{
	AOS3_REG_Struct( M68KREGT_A0, RS4StructID_TextFont ),
	AOS3_REG_EndMarker()
};

static AOS3_RegStruct _TextFont_A1[] =
{
	AOS3_REG_Struct( M68KREGT_A1, RS4StructID_TextFont ),
	AOS3_REG_EndMarker()
};

// --
// Double input

static AOS3_RegStruct _BitMap_Bitmap_A0_A1[] =
{
	AOS3_REG_Struct( M68KREGT_A0, RS4StructID_BitMap ),
	AOS3_REG_Struct( M68KREGT_A1, RS4StructID_BitMap ),
	AOS3_REG_EndMarker()
};

static AOS3_RegStruct _BitMap_RastPort_A0_A1[] =
{
	AOS3_REG_Struct( M68KREGT_A0, RS4StructID_BitMap ),
	AOS3_REG_Struct( M68KREGT_A1, RS4StructID_RastPort ),
	AOS3_REG_EndMarker()
};

static AOS3_RegStruct _TextFont_RastPort_A0_A1[] =
{
	AOS3_REG_Struct( M68KREGT_A0, RS4StructID_TextFont ),
	AOS3_REG_Struct( M68KREGT_A1, RS4StructID_RastPort ),
	AOS3_REG_EndMarker()
};

static AOS3_RegStruct _String_RastPort_A0_A1[] =
{
	AOS3_REG_String( M68KREGT_A0 ),
	AOS3_REG_Struct( M68KREGT_A1, RS4StructID_RastPort ),
	AOS3_REG_EndMarker()
};

// --
// Single Arg functions

#define AOS3_InitBitMap_Regs		_BitMap_A0
#define AOS3_FreeBitMap_Regs		_BitMap_A0

#define AOS3_OpenFont_Regs			_TextAttr_A0

#define AOS3_StripFont_Regs			_TextFont_A0

#define AOS3_CloseFont_Regs			_TextFont_A1
#define AOS3_AddFont_Regs			_TextFont_A1
#define AOS3_RemFont_Regs			_TextFont_A1

#define AOS3_ClearScreen_Regs		_RastPort_A1
#define AOS3_ClearEOL_Regs			_RastPort_A1
#define AOS3_SetRast_Regs			_RastPort_A1
#define AOS3_RectFill_Regs			_RastPort_A1
#define AOS3_AreaMove_Regs			_RastPort_A1
#define AOS3_AreaDraw_Regs			_RastPort_A1
#define AOS3_AreaEnd_Regs			_RastPort_A1
#define AOS3_Move_Regs				_RastPort_A1
#define AOS3_Draw_Regs				_RastPort_A1
#define AOS3_ClearEOL_Regs			_RastPort_A1
#define AOS3_InitRastPort_Regs		_RastPort_A1

// --
// Double Args

#define AOS3_BltBitMap_Regs			_BitMap_Bitmap_A0_A1

#define AOS3_BltBitMapRastPort_Regs	_BitMap_RastPort_A0_A1

#define AOS3_TextLength_Regs		_String_RastPort_A0_A1
#define AOS3_Text_Regs				_String_RastPort_A0_A1

#define AOS3_SetFont_Regs			_TextFont_RastPort_A0_A1

// --

AOS3_LVOStruct AOS3_GraphicsBase[] =
{
	AOS3_LVO_Regs( -30, BltBitMap ),
	AOS3_LVO_Name( -36, BltTemplate ),
	AOS3_LVO_Regs( -42, ClearEOL ),
	AOS3_LVO_Regs( -48, ClearScreen ),
	AOS3_LVO_Regs( -54, TextLength ),
	AOS3_LVO_Regs( -60, Text ),
	AOS3_LVO_Regs( -66, SetFont ),
	AOS3_LVO_Regs( -72, OpenFont ),
	AOS3_LVO_Regs( -78, CloseFont ),
	AOS3_LVO_Name( -84, AskSoftStyle ),
	AOS3_LVO_Name( -90, SetSoftStyle ),
	AOS3_LVO_Name( -96, AddBob ),
	AOS3_LVO_Name( -102, AddVSprite ),
	AOS3_LVO_Name( -108, DoCollision ),
	AOS3_LVO_Name( -114, DrawGList ),
	AOS3_LVO_Name( -120, InitGels ),
	AOS3_LVO_Name( -126, InitMasks ),
	AOS3_LVO_Name( -132, RemIBob ),
	AOS3_LVO_Name( -138, RemVSprite ),
	AOS3_LVO_Name( -144, SetCollision ),
	AOS3_LVO_Name( -150, SortGList ),
	AOS3_LVO_Name( -156, AddAnimOb ),
	AOS3_LVO_Name( -162, Animate ),
	AOS3_LVO_Name( -168, GetGBuffers ),
	AOS3_LVO_Name( -174, InitGMasks ),
	AOS3_LVO_Name( -180, DrawEllipse ),
	AOS3_LVO_Name( -186, AreaEllipse ),
	AOS3_LVO_Name( -192, LoadRGB4 ),
	AOS3_LVO_Regs( -198, InitRastPort ),
	AOS3_LVO_Name( -204, InitVPort ),
	AOS3_LVO_Name( -210, MrgCop ),
	AOS3_LVO_Name( -216, MakeVPort ),
	AOS3_LVO_Name( -222, LoadView ),
	AOS3_LVO_Name( -228, WaitBlit ),
	AOS3_LVO_Regs( -234, SetRast ),
	AOS3_LVO_Regs( -240, Move ),
	AOS3_LVO_Regs( -246, Draw ),
	AOS3_LVO_Regs( -252, AreaMove ),
	AOS3_LVO_Regs( -258, AreaDraw ),
	AOS3_LVO_Regs( -264, AreaEnd ),
	AOS3_LVO_Name( -270, WaitTOF ),
	AOS3_LVO_Name( -276, QBlit ),
	AOS3_LVO_Name( -282, InitArea ),
	AOS3_LVO_Name( -288, SetRGB4 ),
	AOS3_LVO_Name( -294, QBSBlit ),
	AOS3_LVO_Name( -300, BltClear ),
	AOS3_LVO_Regs( -306, RectFill ),
	AOS3_LVO_Name( -312, BltPattern ),
	AOS3_LVO_Name( -318, ReadPixel ),
	AOS3_LVO_Name( -324, WritePixel ),
	AOS3_LVO_Name( -330, Flood ),
	AOS3_LVO_Name( -336, PolyDraw ),
	AOS3_LVO_Name( -342, SetAPen ),
	AOS3_LVO_Name( -348, SetBPen ),
	AOS3_LVO_Name( -354, SetDrMd ),
	AOS3_LVO_Name( -360, InitView ),
	AOS3_LVO_Name( -366, CBump ),
	AOS3_LVO_Name( -372, CMove ),
	AOS3_LVO_Name( -378, CWait ),
	AOS3_LVO_Name( -384, VBeamPos ),
	AOS3_LVO_Regs( -390, InitBitMap ), //", AOS3_Graphics_Func_InitBitMap },
	AOS3_LVO_Name( -396, ScrollRaster ),
	AOS3_LVO_Name( -402, WaitBOVP ),
	AOS3_LVO_Name( -408, GetSprite ),
	AOS3_LVO_Name( -414, FreeSprite ),
	AOS3_LVO_Name( -420, ChangeSprite ),
	AOS3_LVO_Name( -426, MoveSprite ),
	AOS3_LVO_Name( -432, LockLayerRom ),
	AOS3_LVO_Name( -438, UnlockLayerRom ),
	AOS3_LVO_Name( -444, SyncSBitMap ),
	AOS3_LVO_Name( -450, CopySBitMap ),
	AOS3_LVO_Name( -456, OwnBlitter ),
	AOS3_LVO_Name( -462, DisownBlitter ),
	AOS3_LVO_Name( -468, InitTmpRas ),
	AOS3_LVO_Name( -474, AskFont ),
	AOS3_LVO_Regs( -480, AddFont ),
	AOS3_LVO_Name( -486, RemFont ),
	AOS3_LVO_Name( -492, AllocRaster ),
	AOS3_LVO_Name( -498, FreeRaster ),
	AOS3_LVO_Name( -504, AndRectRegion ),
	AOS3_LVO_Name( -510, OrRectRegion ),
	AOS3_LVO_Name( -516, NewRegion ),
	AOS3_LVO_Name( -522, ClearRectRegion ),
	AOS3_LVO_Name( -528, ClearRegion ),
	AOS3_LVO_Name( -534, DisposeRegion ),
	AOS3_LVO_Name( -540, FreeVPortCopLists ),
	AOS3_LVO_Name( -546, FreeCopList ),
	AOS3_LVO_Name( -552, ClipBlit ),
	AOS3_LVO_Name( -558, XorRectRegion ),
	AOS3_LVO_Name( -564, FreeCprList ),
	AOS3_LVO_Name( -570, GetColorMap ),
	AOS3_LVO_Name( -576, FreeColorMap ),
	AOS3_LVO_Name( -582, GetRGB4 ),
	AOS3_LVO_Name( -588, ScrollVPort ),
	AOS3_LVO_Name( -594, UCopperListInit ),
	AOS3_LVO_Name( -600, FreeGBuffers ),
	AOS3_LVO_Regs( -606, BltBitMapRastPort ),
	AOS3_LVO_Name( -612, OrRegionRegion ),
	AOS3_LVO_Name( -618, XorRegionRegion ),
	AOS3_LVO_Name( -624, AndRegionRegion ),
	AOS3_LVO_Name( -630, SetRGB4CM ),
	AOS3_LVO_Name( -636, BltMaskBitMapRastPort ),
	AOS3_LVO_Name( -642, GfxInternal1 ),
	AOS3_LVO_Name( -648, GfxInternal2 ),
	AOS3_LVO_Name( -654, AttemptLockLayerRom ),
	AOS3_LVO_Name( -660, GfxNew ),
	AOS3_LVO_Name( -666, GfxFree ),
	AOS3_LVO_Name( -672, GfxAssociate ),
	AOS3_LVO_Name( -678, BitMapScale ),
	AOS3_LVO_Name( -684, ScalerDiv ),
	AOS3_LVO_Name( -690, TextExtent ),
	AOS3_LVO_Name( -696, TextFit ),
	AOS3_LVO_Name( -702, GfxLookUp ),
	AOS3_LVO_Name( -708, VideoControl ),
	AOS3_LVO_Name( -714, OpenMonitor ),
	AOS3_LVO_Name( -720, CloseMonitor ),
	AOS3_LVO_Name( -726, FindDisplayInfo ),
	AOS3_LVO_Name( -732, NextDisplayInfo ),
	AOS3_LVO_Name( -756, GetDisplayInfoData ),
	AOS3_LVO_Name( -762, FontExtent ),
	AOS3_LVO_Name( -768, ReadPixelLine8 ),
	AOS3_LVO_Name( -774, WritePixelLine8 ),
	AOS3_LVO_Name( -780, ReadPixelArray8 ),
	AOS3_LVO_Name( -786, WritePixelArray8 ),
	AOS3_LVO_Name( -792, GetVPModeID ),
	AOS3_LVO_Name( -798, ModeNotAvailable ),
	AOS3_LVO_Name( -804, WeighTAMatch ),
	AOS3_LVO_Name( -810, EraseRect ),
	AOS3_LVO_Name( -816, ExtendFont ),
	AOS3_LVO_Regs( -822, StripFont ),
	AOS3_LVO_Name( -828, CalcIVG ),
	AOS3_LVO_Name( -834, AttachPalExtra ),
	AOS3_LVO_Name( -840, ObtainBestPenA ),
	AOS3_LVO_Name( -846, GfxInternal3 ),
	AOS3_LVO_Name( -852, SetRGB32 ),
	AOS3_LVO_Name( -858, GetAPen ),
	AOS3_LVO_Name( -864, GetBPen ),
	AOS3_LVO_Name( -870, GetDrMd ),
	AOS3_LVO_Name( -876, GetOutlinePen ),
	AOS3_LVO_Name( -882, LoadRGB32 ),
	AOS3_LVO_Name( -888, SetChipRev ),
	AOS3_LVO_Name( -894, SetABPenDrMd ),
	AOS3_LVO_Name( -900, GetRGB32 ),
	AOS3_LVO_Name( -906, GfxSpare1 ),
	AOS3_LVO_Name( -918, AllocBitMap ),
	AOS3_LVO_Regs( -924, FreeBitMap ),
	AOS3_LVO_Name( -930, GetExtSpriteA ),
	AOS3_LVO_Name( -936, CoerceMode ),
	AOS3_LVO_Name( -942, ChangeVPBitMap ),
	AOS3_LVO_Name( -948, ReleasePen ),
	AOS3_LVO_Name( -954, ObtainPen ),
	AOS3_LVO_Name( -960, GetBitMapAttr ),
	AOS3_LVO_Name( -966, AllocDBufInfo ),
	AOS3_LVO_Name( -972, FreeDBufInfo ),
	AOS3_LVO_Name( -978, SetOutlinePen ),
	AOS3_LVO_Name( -984, SetWriteMask ),
	AOS3_LVO_Name( -990, SetMaxPen ),
	AOS3_LVO_Name( -996, SetRGB32CM ),
	AOS3_LVO_Name( -1002, ScrollRasterBF ),
	AOS3_LVO_Name( -1008, FindColor ),
	AOS3_LVO_Name( -1014, GfxSpare2 ),
	AOS3_LVO_Name( -1020, AllocSpriteDataA ),
	AOS3_LVO_Name( -1026, ChangeExtSpriteA ),
	AOS3_LVO_Name( -1032, FreeSpriteData ),
	AOS3_LVO_Name( -1038, SetRPAttrsA ),
	AOS3_LVO_Name( -1044, GetRPAttrsA ),
	AOS3_LVO_Name( -1050, BestModeIDA ),
	AOS3_LVO_EndMarker()
};

// --
