
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

//enum RS4DecodeStat AOS3_Mathieeedoubtrans_Func_OpenLibrary( enum RS4ErrorCode *errcode, RS4Trace *rt );

// --

AOS3_LVOStruct AOS3_MathieeedoubtransBase[] =
{
	AOS3_LVO_Name( -30, IEEEDPAtan ),
	AOS3_LVO_Name( -36, IEEEDPSin ),
	AOS3_LVO_Name( -42, IEEEDPCos ),
	AOS3_LVO_Name( -48, IEEEDPTan ),
	AOS3_LVO_Name( -54, IEEEDPSincos ),
	AOS3_LVO_Name( -60, IEEEDPSinh ),
	AOS3_LVO_Name( -66, IEEEDPCosh ),
	AOS3_LVO_Name( -72, IEEEDPTanh ),
	AOS3_LVO_Name( -78, IEEEDPExp ),
	AOS3_LVO_Name( -84, IEEEDPLog ),
	AOS3_LVO_Name( -90, IEEEDPPow ),
	AOS3_LVO_Name( -96, IEEEDPSqrt ),
	AOS3_LVO_Name( -102, IEEEDPTieee ),
	AOS3_LVO_Name( -108, IEEEDPFieee ),
	AOS3_LVO_Name( -114, IEEEDPAsin ),
	AOS3_LVO_Name( -120, IEEEDPAcos ),
	AOS3_LVO_Name( -126, IEEEDPLog10 ),
	AOS3_LVO_EndMarker()
};

// --
