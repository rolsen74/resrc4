
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

//enum RS4DecodeStat AOS3_Icon_Func_OpenLibrary( enum RS4ErrorCode *errcode, RS4Trace *rt );

// --

AOS3_LVOStruct AOS3_IconBase[] =
{
	AOS3_LVO_Name( -42, GetIcon ),
	AOS3_LVO_Name( -48, PutIcon ),
	AOS3_LVO_Name( -54, FreeFreeList ),
	AOS3_LVO_Name( -72, AddFreeList ),
	AOS3_LVO_Name( -78, GetDiskObject ),
	AOS3_LVO_Name( -84, PutDiskObject ),
	AOS3_LVO_Name( -90, FreeDiskObject ),
	AOS3_LVO_Name( -96, FindToolType ),
	AOS3_LVO_Name( -102, MatchToolValue ),
	AOS3_LVO_Name( -108, BumpRevision ),
	AOS3_LVO_Name( -120, GetDefDiskObject ),
	AOS3_LVO_Name( -126, PutDefDiskObject ),
	AOS3_LVO_Name( -132, GetDiskObjectNew ),
	AOS3_LVO_Name( -138, DeleteDiskObject ),
	AOS3_LVO_EndMarker()
};

// --
