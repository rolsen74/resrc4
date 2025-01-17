
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

//enum RS4DecodeStat AOS3_Cardres_Func_OpenLibrary( enum RS4ErrorCode *errcode, RS4Trace *rt );

// --

AOS3_LVOStruct AOS3_CardresBase[] =
{
	AOS3_LVO_Name( -6, OwnCard ),
	AOS3_LVO_Name( -12, ReleaseCard ),
	AOS3_LVO_Name( -18, GetCardMap ),
	AOS3_LVO_Name( -24, BeginCardAccess ),
	AOS3_LVO_Name( -30, EndCardAccess ),
	AOS3_LVO_Name( -36, ReadCardStatus ),
	AOS3_LVO_Name( -42, CardResetRemove ),
	AOS3_LVO_Name( -48, CardMiscControl ),
	AOS3_LVO_Name( -54, CardAccessSpeed ),
	AOS3_LVO_Name( -60, CardProgramVoltage ),
	AOS3_LVO_Name( -66, CardResetCard ),
	AOS3_LVO_Name( -72, CopyTuple ),
	AOS3_LVO_Name( -78, DeviceTuple ),
	AOS3_LVO_Name( -84, IfAmigaXIP ),
	AOS3_LVO_Name( -90, CardForceChange ),
	AOS3_LVO_Name( -96, CardChangeCount ),
	AOS3_LVO_Name( -102, CardInterface ),
	AOS3_LVO_EndMarker()
};

// --
