
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

//enum RS4DecodeStat AOS3_Mathieeedoubbas_Func_OpenLibrary( enum RS4ErrorCode *errcode, RS4Trace *rt );

// --

AOS3_LVOStruct AOS3_MathieeedoubbasBase[] =
{
	AOS3_LVO_Name( -30, IEEEDPFix ),
	AOS3_LVO_Name( -36, IEEEDPFlt ),
	AOS3_LVO_Name( -42, IEEEDPCmp ),
	AOS3_LVO_Name( -48, IEEEDPTst ),
	AOS3_LVO_Name( -54, IEEEDPAbs ),
	AOS3_LVO_Name( -60, IEEEDPNeg ),
	AOS3_LVO_Name( -66, IEEEDPAdd ),
	AOS3_LVO_Name( -72, IEEEDPSub ),
	AOS3_LVO_Name( -78, IEEEDPMul ),
	AOS3_LVO_Name( -84, IEEEDPDiv ),
	AOS3_LVO_Name( -90, IEEEDPFloor ),
	AOS3_LVO_Name( -96, IEEEDPCeil ),
	AOS3_LVO_EndMarker()
};

// --
