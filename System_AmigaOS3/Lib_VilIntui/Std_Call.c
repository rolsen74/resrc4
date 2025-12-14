
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

//enum RS4DecodeStat AOS3_Layers_Func_OpenLibrary( enum RS4ErrorCode *errcode, RS4Trace *rt );

// --

AOS3_LVOStruct AOS3_VilIntuiBase[] =
{
	AOS3_LVO_Name( -30, OpenVillageScreen ),
	AOS3_LVO_Name( -36, CloseVillageScreen ),
	AOS3_LVO_Name( -42, UnLockVillageScreen ),
	AOS3_LVO_Name( -48, LockVillageScreen ),
	AOS3_LVO_Name( -54, WaitVillageBlit ),
	AOS3_LVO_Name( -60, VillageBlitCopy ),
	AOS3_LVO_Name( -66, IsVillageScreen ),
	AOS3_LVO_Name( -72, GetMemSize ),
	AOS3_LVO_Name( -78, VillageRectFill ),
	AOS3_LVO_Name( -84, VillageModeRequest ),
	AOS3_LVO_Name( -90, VillageModeRequestTags ),
	AOS3_LVO_Name( -96, VillageModeList ),
	AOS3_LVO_Name( -102, VillageModeListTags ),
	AOS3_LVO_Name( -108, VillageGetSegment ),
	AOS3_LVO_Name( -114, VillageSetSegment ),
	AOS3_LVO_Name( -120, VillageBestModeID ),
	AOS3_LVO_Name( -126, VillageBestModeIDTags ),
	AOS3_LVO_Name( -132, OpenVillageScreenTagList ),
	AOS3_LVO_Name( -138, OpenVillageScreenTags ),
	AOS3_LVO_Name( -144, VillageScreenData ),
	AOS3_LVO_Name( -150, VillageGetBufAddr ),
	AOS3_LVO_Name( -156, VillageSetDisplayBuf ),
	AOS3_LVO_EndMarker()
};

// --
