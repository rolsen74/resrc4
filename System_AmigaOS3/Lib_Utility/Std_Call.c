
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

//enum RS4DecodeStat AOS3_Utility_Func_OpenLibrary( enum RS4ErrorCode *errcode, RS4Trace *rt );

// --

AOS3_LVOStruct AOS3_UtilityBase[] =
{
	AOS3_LVO_Name( -30, FindTagItem ),
	AOS3_LVO_Name( -36, GetTagData ),
	AOS3_LVO_Name( -42, PackBoolTags ),
	AOS3_LVO_Name( -48, NextTagItem ),
	AOS3_LVO_Name( -54, FilterTagChanges ),
	AOS3_LVO_Name( -60, MapTags ),
	AOS3_LVO_Name( -66, AllocateTagItems ),
	AOS3_LVO_Name( -72, CloneTagItems ),
	AOS3_LVO_Name( -78, FreeTagItems ),
	AOS3_LVO_Name( -84, RefreshTagItemClones ),
	AOS3_LVO_Name( -90, TagInArray ),
	AOS3_LVO_Name( -96, FilterTagItems ),
	AOS3_LVO_Name( -102, CallHookPkt ),
	AOS3_LVO_Name( -120, Amiga2Date ),
	AOS3_LVO_Name( -126, Date2Amiga ),
	AOS3_LVO_Name( -132, CheckDate ),
	AOS3_LVO_Name( -138, SMult32 ),
	AOS3_LVO_Name( -144, UMult32 ),
	AOS3_LVO_Name( -150, SDivMod32 ),
	AOS3_LVO_Name( -156, UDivMod32 ),
	AOS3_LVO_Name( -162, Stricmp ),
	AOS3_LVO_Name( -168, Strnicmp ),
	AOS3_LVO_Name( -174, ToUpper ),
	AOS3_LVO_Name( -180, ToLower ),
	AOS3_LVO_Name( -186, ApplyTagChanges ),
	AOS3_LVO_Name( -198, SMult64 ),
	AOS3_LVO_Name( -204, UMult64 ),
	AOS3_LVO_Name( -210, PackStructureTags ),
	AOS3_LVO_Name( -216, UnpackStructureTags ),
	AOS3_LVO_Name( -222, AddNamedObject ),
	AOS3_LVO_Name( -228, AllocNamedObjectA ),
	AOS3_LVO_Name( -234, AttemptRemNamedObject ),
	AOS3_LVO_Name( -240, FindNamedObject ),
	AOS3_LVO_Name( -246, FreeNamedObject ),
	AOS3_LVO_Name( -252, NamedObjectName ),
	AOS3_LVO_Name( -258, ReleaseNamedObject ),
	AOS3_LVO_Name( -264, RemNamedObject ),
	AOS3_LVO_Name( -270, GetUniqueID ),
	AOS3_LVO_EndMarker()
};

// --
