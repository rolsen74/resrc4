
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

//enum RS4DecodeStat AOS3_IFFParse_Func_OpenLibrary( enum RS4ErrorCode *errcode, RS4Trace *rt );

// --

AOS3_LVOStruct AOS3_IFFParseBase[] =
{
	AOS3_LVO_Name( -30, AllocIFF ),
	AOS3_LVO_Name( -36, OpenIFF ),
	AOS3_LVO_Name( -42, ParseIFF ),
	AOS3_LVO_Name( -48, CloseIFF ),
	AOS3_LVO_Name( -54, FreeIFF ),
	AOS3_LVO_Name( -60, ReadChunkBytes ),
	AOS3_LVO_Name( -66, WriteChunkBytes ),
	AOS3_LVO_Name( -72, ReadChunkRecords ),
	AOS3_LVO_Name( -78, WriteChunkRecords ),
	AOS3_LVO_Name( -84, PushChunk ),
	AOS3_LVO_Name( -90, PopChunk ),
	AOS3_LVO_Name( -102, EntryHandler ),
	AOS3_LVO_Name( -108, ExitHandler ),
	AOS3_LVO_Name( -114, PropChunk ),
	AOS3_LVO_Name( -120, PropChunks ),
	AOS3_LVO_Name( -126, StopChunk ),
	AOS3_LVO_Name( -132, StopChunks ),
	AOS3_LVO_Name( -138, CollectionChunk ),
	AOS3_LVO_Name( -144, CollectionChunks ),
	AOS3_LVO_Name( -150, StopOnExit ),
	AOS3_LVO_Name( -156, FindProp ),
	AOS3_LVO_Name( -162, FindCollection ),
	AOS3_LVO_Name( -168, FindPropContext ),
	AOS3_LVO_Name( -174, CurrentChunk ),
	AOS3_LVO_Name( -180, ParentChunk ),
	AOS3_LVO_Name( -186, AllocLocalItem ),
	AOS3_LVO_Name( -192, LocalItemData ),
	AOS3_LVO_Name( -198, SetLocalItemPurge ),
	AOS3_LVO_Name( -204, FreeLocalItem ),
	AOS3_LVO_Name( -210, FindLocalItem ),
	AOS3_LVO_Name( -216, StoreLocalItem ),
	AOS3_LVO_Name( -222, StoreItemInContext ),
	AOS3_LVO_Name( -228, InitIFF ),
	AOS3_LVO_Name( -234, InitIFFasDOS ),
	AOS3_LVO_Name( -240, InitIFFasClip ),
	AOS3_LVO_Name( -246, OpenClipboard ),
	AOS3_LVO_Name( -252, CloseClipboard ),
	AOS3_LVO_Name( -258, GoodID ),
	AOS3_LVO_Name( -264, GoodType ),
	AOS3_LVO_Name( -270, IDtoStr ),
	AOS3_LVO_EndMarker()
};

// --
