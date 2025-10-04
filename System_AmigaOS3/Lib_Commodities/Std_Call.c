
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

//enum RS4DecodeStat AOS3_Commodities_Func_OpenLibrary( enum RS4ErrorCode *errcode, RS4Trace *rt );

// --

AOS3_LVOStruct AOS3_CommoditiesBase[] =
{
	AOS3_LVO_Name( -30, CreateCxObj ),
	AOS3_LVO_Name( -36, CxBroker ),
	AOS3_LVO_Name( -42, ActivateCxObj ),
	AOS3_LVO_Name( -48, DeleteCxObj ),
	AOS3_LVO_Name( -54, DeleteCxObjAll ),
	AOS3_LVO_Name( -60, CxObjType ),
	AOS3_LVO_Name( -66, CxObjError ),
	AOS3_LVO_Name( -72, ClearCxObjError ),
	AOS3_LVO_Name( -78, SetCxObjPri ),
	AOS3_LVO_Name( -84, AttachCxObj ),
	AOS3_LVO_Name( -90, EnqueueCxObj ),
	AOS3_LVO_Name( -96, InsertCxObj ),
	AOS3_LVO_Name( -102, RemoveCxObj ),
	AOS3_LVO_Name( -114, SetTranslate ),
	AOS3_LVO_Name( -120, SetFilter ),
	AOS3_LVO_Name( -126, SetFilterIX ),
	AOS3_LVO_Name( -132, ParseIX ),
	AOS3_LVO_Name( -138, CxMsgType ),
	AOS3_LVO_Name( -144, CxMsgData ),
	AOS3_LVO_Name( -150, CxMsgID ),
	AOS3_LVO_Name( -156, DivertCxMsg ),
	AOS3_LVO_Name( -162, RouteCxMsg ),
	AOS3_LVO_Name( -168, DisposeCxMsg ),
	AOS3_LVO_Name( -174, InvertKeyMap ),
	AOS3_LVO_Name( -180, AddIEvents ),
	AOS3_LVO_Name( -204, MatchIX ),
	AOS3_LVO_EndMarker()
};

// --
