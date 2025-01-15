
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

//enum RS4DecodeStat AOS3_Mathieeesingbas_Func_OpenLibrary( enum RS4ErrorCode *errcode, RS4Trace *rt );

// --

AOS3_LVOStruct AOS3_MathieeesingbasBase[] =
{
	AOS3_LVO_Name( -30, IEEESPFix ),
	AOS3_LVO_Name( -36, IEEESPFlt ),
	AOS3_LVO_Name( -42, IEEESPCmp ),
	AOS3_LVO_Name( -48, IEEESPTst ),
	AOS3_LVO_Name( -54, IEEESPAbs ),
	AOS3_LVO_Name( -60, IEEESPNeg ),
	AOS3_LVO_Name( -66, IEEESPAdd ),
	AOS3_LVO_Name( -72, IEEESPSub ),
	AOS3_LVO_Name( -78, IEEESPMul ),
	AOS3_LVO_Name( -84, IEEESPDiv ),
	AOS3_LVO_Name( -90, IEEESPFloor ),
	AOS3_LVO_Name( -96, IEEESPCeil ),
	AOS3_LVO_EndMarker()
};

// --
