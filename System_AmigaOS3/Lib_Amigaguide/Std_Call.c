
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

//enum RS4DecodeStat AOS3_Amigaguide_Func_OpenLibrary( enum RS4ErrorCode *errcode, RS4Trace *rt );

// --

AOS3_LVOStruct AOS3_AmigaguideBase[] =
{
	AOS3_LVO_Name( -30, AGARexxHost ),
	AOS3_LVO_Name( -36, LockAmigaGuideBase ),
	AOS3_LVO_Name( -42, UnlockAmigaGuideBase ),
	AOS3_LVO_Name( -54, OpenAmigaGuideA ),
	AOS3_LVO_Name( -60, OpenAmigaGuideAsyncA ),
	AOS3_LVO_Name( -66, CloseAmigaGuide ),
	AOS3_LVO_Name( -72, AmigaGuideSignal ),
	AOS3_LVO_Name( -78, GetAmigaGuideMsg ),
	AOS3_LVO_Name( -84, ReplyAmigaGuideMsg ),
	AOS3_LVO_Name( -90, SetAmigaGuideContextA ),
	AOS3_LVO_Name( -96, SendAmigaGuideContextA ),
	AOS3_LVO_Name( -102, SendAmigaGuideCmdA ),
	AOS3_LVO_Name( -108, SetAmigaGuideAttrsA ),
	AOS3_LVO_Name( -114, GetAmigaGuideAttr ),
	AOS3_LVO_Name( -138, AddAmigaGuideHostA ),
	AOS3_LVO_Name( -144, RemoveAmigaGuideHostA ),
	AOS3_LVO_Name( -210, GetAmigaGuideString ),
	AOS3_LVO_EndMarker()
};

// --
