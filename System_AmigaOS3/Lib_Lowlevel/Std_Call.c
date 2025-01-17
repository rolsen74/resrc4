
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

//enum RS4DecodeStat AOS3_Lowlevel_Func_OpenLibrary( enum RS4ErrorCode *errcode, RS4Trace *rt );

// --

AOS3_LVOStruct AOS3_LowlevelBase[] =
{
	AOS3_LVO_Name( -30, ReadJoyPort ),
	AOS3_LVO_Name( -36, GetLanguageSelection ),
//	AOS3_LVO_Name( -42, lowlevelPrivate1 ),
	AOS3_LVO_Name( -48, GetKey ),
	AOS3_LVO_Name( -54, QueryKeys ),
	AOS3_LVO_Name( -60, AddKBInt ),
	AOS3_LVO_Name( -66, RemKBInt ),
	AOS3_LVO_Name( -72, SystemControlA ),
	AOS3_LVO_Name( -78, AddTimerInt ),
	AOS3_LVO_Name( -84, RemTimerInt ),
	AOS3_LVO_Name( -90, StopTimerInt ),
	AOS3_LVO_Name( -96, StartTimerInt ),
	AOS3_LVO_Name( -102, ElapsedTime ),
	AOS3_LVO_Name( -108, AddVBlankInt ),
	AOS3_LVO_Name( -114, RemVBlankInt ),
//	AOS3_LVO_Name( -120, lowlevelPrivate2 ),
//	AOS3_LVO_Name( -126, lowlevelPrivate3 ),
	AOS3_LVO_Name( -132, SetJoyPortAttrsA ),
//	AOS3_LVO_Name( -138, lowlevelPrivate4 ),
//	AOS3_LVO_Name( -144, lowlevelPrivate5 ),
//	AOS3_LVO_Name( -150, lowlevelPrivate6 ),
//	AOS3_LVO_Name( -156, lowlevelPrivate7 ),
//	AOS3_LVO_Name( -162, lowlevelPrivate8 ),
	AOS3_LVO_EndMarker()
};

// --
