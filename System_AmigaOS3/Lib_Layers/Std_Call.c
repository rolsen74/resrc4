
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

//enum RS4DecodeStat AOS3_Layers_Func_OpenLibrary( enum RS4ErrorCode *errcode, RS4Trace *rt );

// --

AOS3_LVOStruct AOS3_LayersBase[] =
{
	AOS3_LVO_Name( -30, InitLayers ),
	AOS3_LVO_Name( -36, CreateUpfrontLayer ),
	AOS3_LVO_Name( -42, CreateBehindLayer ),
	AOS3_LVO_Name( -48, UpfrontLayer ),
	AOS3_LVO_Name( -54, BehindLayer ),
	AOS3_LVO_Name( -60, MoveLayer ),
	AOS3_LVO_Name( -66, SizeLayer ),
	AOS3_LVO_Name( -72, ScrollLayer ),
	AOS3_LVO_Name( -78, BeginUpdate ),
	AOS3_LVO_Name( -84, EndUpdate ),
	AOS3_LVO_Name( -90, DeleteLayer ),
	AOS3_LVO_Name( -96, LockLayer ),
	AOS3_LVO_Name( -102, UnlockLayer ),
	AOS3_LVO_Name( -108, LockLayers ),
	AOS3_LVO_Name( -114, UnlockLayers ),
	AOS3_LVO_Name( -120, LockLayerInfo ),
	AOS3_LVO_Name( -126, SwapBitsRastPortClipRect ),
	AOS3_LVO_Name( -132, WhichLayer ),
	AOS3_LVO_Name( -138, UnlockLayerInfo ),
	AOS3_LVO_Name( -144, NewLayerInfo ),
	AOS3_LVO_Name( -150, DisposeLayerInfo ),
	AOS3_LVO_Name( -156, FattenLayerInfo ),
	AOS3_LVO_Name( -162, ThinLayerInfo ),
	AOS3_LVO_Name( -168, MoveLayerInFrontOf ),
	AOS3_LVO_Name( -174, InstallClipRegion ),
	AOS3_LVO_Name( -180, MoveSizeLayer ),
	AOS3_LVO_Name( -186, CreateUpfrontHookLayer ),
	AOS3_LVO_Name( -192, CreateBehindHookLayer ),
	AOS3_LVO_Name( -198, InstallLayerHook ),
	AOS3_LVO_Name( -204, InstallLayerInfoHook ),
	AOS3_LVO_Name( -210, SortLayerCR ),
	AOS3_LVO_Name( -216, DoHookClipRects ),
	AOS3_LVO_EndMarker()
};

// --
