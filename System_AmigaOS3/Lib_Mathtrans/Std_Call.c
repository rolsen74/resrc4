
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

//enum RS4DecodeStat AOS3_Mathtrans_Func_OpenLibrary( enum RS4ErrorCode *errcode, RS4Trace *rt );

// --

AOS3_LVOStruct AOS3_MathtransBase[] =
{
	AOS3_LVO_Name( -30, SPAtan ),
	AOS3_LVO_Name( -36, SPSin ),
	AOS3_LVO_Name( -42, SPCos ),
	AOS3_LVO_Name( -48, SPTan ),
	AOS3_LVO_Name( -54, SPSincos ),
	AOS3_LVO_Name( -60, SPSinh ),
	AOS3_LVO_Name( -66, SPCosh ),
	AOS3_LVO_Name( -72, SPTanh ),
	AOS3_LVO_Name( -78, SPExp ),
	AOS3_LVO_Name( -84, SPLog ),
	AOS3_LVO_Name( -90, SPPow ),
	AOS3_LVO_Name( -96, SPSqrt ),
	AOS3_LVO_Name( -102, SPTieee ),
	AOS3_LVO_Name( -108, SPFieee ),
	AOS3_LVO_Name( -114, SPAsin ),
	AOS3_LVO_Name( -120, SPAcos ),
	AOS3_LVO_Name( -126, SPLog10 ),
	AOS3_LVO_EndMarker()
};

// --
