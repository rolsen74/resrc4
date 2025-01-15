
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

#include "Resourcer/ReSrc4.h"

// --

//enum RS4DecodeStat AOS3_Gadtools_Func_OpenLibrary( enum RS4ErrorCode *errcode, RS4Trace *rt );

// --

AOS3_LVOStruct AOS3_GadToolsBase[] =
{
	AOS3_LVO_Name( -30, AllocFileRequest ),
	AOS3_LVO_Name( -36, FreeFileRequest ),
	AOS3_LVO_Name( -42, RequestFile ),
	AOS3_LVO_Name( -48, AllocGadtoolsRequest ),
	AOS3_LVO_Name( -54, FreeGadtoolsRequest ),
	AOS3_LVO_Name( -60, GadtoolsRequest ),
	AOS3_LVO_Name( -66, LayoutMenusA ),
	AOS3_LVO_Name( -72, GT_GetIMsg ),
	AOS3_LVO_Name( -78, GT_ReplyIMsg ),
	AOS3_LVO_Name( -84, GT_RefreshWindow ),
	AOS3_LVO_Name( -90, GT_BeginRefresh ),
	AOS3_LVO_Name( -96, GT_EndRefresh ),
	AOS3_LVO_Name( -102, GT_FilterIMsg ),
	AOS3_LVO_Name( -108, GT_PostFilterIMsg ),
	AOS3_LVO_Name( -114, CreateContext ),
	AOS3_LVO_Name( -120, DrawBevelBoxA ),
	AOS3_LVO_Name( -126, GetVisualInfoA ),
	AOS3_LVO_Name( -132, FreeVisualInfoA ),
	AOS3_LVO_Name( -174, GT_GetGadgetAttrsA ),
	AOS3_LVO_EndMarker()
};

// --
