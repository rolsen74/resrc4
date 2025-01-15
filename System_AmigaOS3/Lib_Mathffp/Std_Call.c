
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

//enum RS4DecodeStat AOS3_Mathffp_Func_OpenLibrary( enum RS4ErrorCode *errcode, RS4Trace *rt );

// --

AOS3_LVOStruct AOS3_MathffpBase[] =
{
	AOS3_LVO_Name( -30, SPFix ),
	AOS3_LVO_Name( -36, SPFlt ),
	AOS3_LVO_Name( -42, SPCmp ),
	AOS3_LVO_Name( -48, SPTst ),
	AOS3_LVO_Name( -54, SPAbs ),
	AOS3_LVO_Name( -60, SPNeg ),
	AOS3_LVO_Name( -66, SPAdd ),
	AOS3_LVO_Name( -72, SPSub ),
	AOS3_LVO_Name( -78, SPMul ),
	AOS3_LVO_Name( -84, SPDiv ),
	AOS3_LVO_Name( -90, SPFloor ),
	AOS3_LVO_Name( -96, SPCeil ),
	AOS3_LVO_EndMarker()
};

// --
