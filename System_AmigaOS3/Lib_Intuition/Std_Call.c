
/*
 * Copyright (c) 2014-2025 Rene W. Olsen < renewolsen @ gmail . com >
 *
 * This software is released under the GNU General Public License, version 3.
 * For the full text of the license, please visit:
 * https://www.gnu.org/licenses/gpl-3.0.html
 *
 * You can also find a copy of the license in the LICENSE file included with this software.
 */

// --

#include "Resourcer/ReSrc4.h"

// --

//static char *Header ="\n; Intuition\n";
//static int Global_Intuition_Used = FALSE;

// --

//enum RS4DecodeStat AOS3_Intuition_Func_PrintIText( enum RS4ErrorCode *errcode, RS4Trace *rt );

// --

#if 0
PrintIText( RastPort, IText, LeftOffset, TopOffset )
			A0        A1     D0          D1

VOID PrintIText( struct RastPort *, struct IntuiText *, WORD, WORD );
#endif

static AOS3_RegStruct AOS3_PrintIText_Regs[] =
{
//	AOS3_REG_Struct( M68KREGT_D0, RS4StructID_RastPort ),
	AOS3_REG_Struct( M68KREGT_A1, RS4StructID_IntuiText ),
	AOS3_REG_EndMarker()
};

// --

AOS3_LVOStruct AOS3_IntuitionBase[] =
{
	AOS3_LVO_Name( -30, OpenIntuition ),
	AOS3_LVO_Name( -36, Intuition ),
	AOS3_LVO_Name( -42, AddGadget ),
	AOS3_LVO_Name( -48, ClearDMRequest ),
	AOS3_LVO_Name( -54, ClearMenuStrip ),
	AOS3_LVO_Name( -60, ClearPointer ),
	AOS3_LVO_Name( -66, CloseScreen ),
	AOS3_LVO_Name( -72, CloseWindow ),
	AOS3_LVO_Name( -78, CloseWorkBench ),
	AOS3_LVO_Name( -84, CurrentTime ),
	AOS3_LVO_Name( -90, DisplayAlert ),
	AOS3_LVO_Name( -96, DisplayBeep ),
	AOS3_LVO_Name( -102, DoubleClick ),
	AOS3_LVO_Name( -108, DrawBorder ),
	AOS3_LVO_Name( -114, DrawImage ),
	AOS3_LVO_Name( -120, EndRequest ),
	AOS3_LVO_Name( -126, GetDefPrefs ),
	AOS3_LVO_Name( -132, GetPrefs ),
	AOS3_LVO_Name( -138, InitRequester ),
	AOS3_LVO_Name( -144, ItemAddress ),
	AOS3_LVO_Name( -150, ModifyIDCMP ),
	AOS3_LVO_Name( -156, ModifyProp ),
	AOS3_LVO_Name( -162, MoveScreen ),
	AOS3_LVO_Name( -168, MoveWindow ),
	AOS3_LVO_Name( -174, OffGadget ),
	AOS3_LVO_Name( -180, OffMenu ),
	AOS3_LVO_Name( -186, OnGadget ),
	AOS3_LVO_Name( -192, OnMenu ),
	AOS3_LVO_Name( -198, OpenScreen ),
	AOS3_LVO_Name( -204, OpenWindow ),
	AOS3_LVO_Name( -210, OpenWorkBench ),
	AOS3_LVO_Regs( -216, PrintIText ),
	AOS3_LVO_Name( -222, RefreshGadgets ),
	AOS3_LVO_Name( -228, RemoveGadget ),
	AOS3_LVO_Name( -234, ReportMouse ),
	AOS3_LVO_Name( -240, Request ),
	AOS3_LVO_Name( -246, ScreenToBack ),
	AOS3_LVO_Name( -252, ScreenToFront ),
	AOS3_LVO_Name( -258, SetDMRequest ),
	AOS3_LVO_Name( -264, SetMenuStrip ),
	AOS3_LVO_Name( -270, SetPointer ),
	AOS3_LVO_Name( -276, SetWindowTitles ),
	AOS3_LVO_Name( -282, ShowTitle ),
	AOS3_LVO_Name( -288, SizeWindow ),
	AOS3_LVO_Name( -294, ViewAddress ),
	AOS3_LVO_Name( -300, ViewPortAddress ),
	AOS3_LVO_Name( -306, WindowToBack ),
	AOS3_LVO_Name( -312, WindowToFront ),
	AOS3_LVO_Name( -318, WindowLimits ),
	AOS3_LVO_Name( -324, SetPrefs ),
	AOS3_LVO_Name( -330, IntuiTextLength ),
	AOS3_LVO_Name( -336, WBenchToBack ),
	AOS3_LVO_Name( -342, WBenchToFront ),
	AOS3_LVO_Name( -348, AutoRequest ),
	AOS3_LVO_Name( -354, BeginRefresh ),
	AOS3_LVO_Name( -360, BuildSysRequest ),
	AOS3_LVO_Name( -366, EndRefresh ),
	AOS3_LVO_Name( -372, FreeSysRequest ),
	AOS3_LVO_Name( -378, MakeScreen ),
	AOS3_LVO_Name( -384, RemakeDisplay ),
	AOS3_LVO_Name( -390, RethinkDisplay ),
	AOS3_LVO_Name( -396, AllocRemember ),
	AOS3_LVO_Name( -402, AlohaWorkbench ),
	AOS3_LVO_Name( -408, FreeRemember ),
	AOS3_LVO_Name( -414, LockIBase ),
	AOS3_LVO_Name( -420, UnlockIBase ),
	AOS3_LVO_Name( -426, GetScreenData ),
	AOS3_LVO_Name( -432, RefreshGList ),
	AOS3_LVO_Name( -438, AddGList ),
	AOS3_LVO_Name( -444, RemoveGList ),
	AOS3_LVO_Name( -450, ActivateWindow ),
	AOS3_LVO_Name( -456, RefreshWindowFrame ),
	AOS3_LVO_Name( -462, ActivateGadget ),
	AOS3_LVO_Name( -468, NewModifyProp ),
	AOS3_LVO_Name( -474, QueryOverscan ),
	AOS3_LVO_Name( -480, MoveWindowInFrontOf ),
	AOS3_LVO_Name( -486, ChangeWindowBox ),
	AOS3_LVO_Name( -492, SetEditHook ),
	AOS3_LVO_Name( -498, SetMouseQueue ),
	AOS3_LVO_Name( -504, ZipWindow ),
	AOS3_LVO_Name( -510, LockPubScreen ),
	AOS3_LVO_Name( -516, UnlockPubScreen ),
	AOS3_LVO_Name( -522, LockPubScreenList ),
	AOS3_LVO_Name( -528, UnlockPubScreenList ),
	AOS3_LVO_Name( -534, NextPubScreen ),
	AOS3_LVO_Name( -540, SetDefaultPubScreen ),
	AOS3_LVO_Name( -546, SetPubScreenModes ),
	AOS3_LVO_Name( -552, PubScreenStatus ),
	AOS3_LVO_Name( -558, ObtainGIRPort ),
	AOS3_LVO_Name( -564, ReleaseGIRPort ),
	AOS3_LVO_Name( -570, GadgetMouse ),
	AOS3_LVO_Name( -582, GetDefaultPubScreen ),
	AOS3_LVO_Name( -588, EasyRequestArgs ),
	AOS3_LVO_Name( -594, BuildEasyRequestArgs ),
	AOS3_LVO_Name( -600, SysReqHandler ),
	AOS3_LVO_Name( -606, OpenWindowTagList ),
	AOS3_LVO_Name( -612, OpenScreenTagList ),
	AOS3_LVO_Name( -618, DrawImageState ),
	AOS3_LVO_Name( -624, PointInImage ),
	AOS3_LVO_Name( -630, EraseImage ),
	AOS3_LVO_Name( -636, NewObjectA ),
	AOS3_LVO_Name( -642, DisposeObject ),
	AOS3_LVO_Name( -648, SetAttrsA ),
	AOS3_LVO_Name( -654, GetAttr ),
	AOS3_LVO_Name( -660, SetGadgetAttrsA ),
	AOS3_LVO_Name( -660, NextObject ),
	AOS3_LVO_Name( -678, MakeClass ),
	AOS3_LVO_Name( -684, AddClass ),
	AOS3_LVO_Name( -690, GetScreenDrawInfo ),
	AOS3_LVO_Name( -696, FreeScreenDrawInfo ),
	AOS3_LVO_Name( -702, ResetMenuStrip ),
	AOS3_LVO_Name( -708, RemoveClass ),
	AOS3_LVO_Name( -714, FreeClass ),
	AOS3_LVO_Name( -720, lockPubClass ),
	AOS3_LVO_Name( -726, unlockPubClass ),
	// --
	AOS3_LVO_Name( -768, AllocScreenBuffer ),
	AOS3_LVO_Name( -774, FreeScreenBuffer ),
	AOS3_LVO_Name( -780, ChangeScreenBuffer ),
	AOS3_LVO_Name( -786, ScreenDepth ),
	AOS3_LVO_Name( -792, ScreenPosition ),
	AOS3_LVO_Name( -798, ScrollWindowRaster ),
	AOS3_LVO_Name( -804, LendMenus ),
	AOS3_LVO_Name( -810, DoGadgetMethodA ),
	AOS3_LVO_Name( -816, SetWindowPointerA ),
	AOS3_LVO_Name( -822, TimedDisplayAlert ),
	AOS3_LVO_Name( -828, HelpControl ),
	AOS3_LVO_EndMarker()
};

// --
