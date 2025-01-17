
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

//enum RS4DecodeStat AOS3_Datatypes_Func_OpenLibrary( enum RS4ErrorCode *errcode, RS4Trace *rt );

// --

AOS3_LVOStruct AOS3_DatatypesBase[] =
{
	AOS3_LVO_Name( -30, RLDispatch ),
	AOS3_LVO_Name( -36, ObtainDataTypeA ),
	AOS3_LVO_Name( -42, ReleaseDataType ),
	AOS3_LVO_Name( -48, NewDTObjectA ),
	AOS3_LVO_Name( -54, DisposeDTObject ),
	AOS3_LVO_Name( -60, SetDTAttrsA ),
	AOS3_LVO_Name( -66, GetDTAttrsA ),
	AOS3_LVO_Name( -72, AddDTObject ),
	AOS3_LVO_Name( -78, RefreshDTObjectA ),
	AOS3_LVO_Name( -84, DoAsyncLayout ),
	AOS3_LVO_Name( -90, DoDTMethodA ),
	AOS3_LVO_Name( -96, RemoveDTObject ),
	AOS3_LVO_Name( -102, GetDTMethods ),
	AOS3_LVO_Name( -108, GetDTTriggerMethods ),
	AOS3_LVO_Name( -114, PrintDTObjectA ),
	AOS3_LVO_Name( -138, GetDTString ),
	AOS3_LVO_EndMarker()
};

// --
