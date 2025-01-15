
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

//enum RS4DecodeStat AOS3_Rexxsyslib_Func_OpenLibrary( enum RS4ErrorCode *errcode, RS4Trace *rt );

// --

AOS3_LVOStruct AOS3_RexxsyslibBase[] =
{
	AOS3_LVO_Name( -126, CreateArgstring ),
	AOS3_LVO_Name( -132, DeleteArgstring ),
	AOS3_LVO_Name( -138, LengthArgstring ),
	AOS3_LVO_Name( -144, CreateRexxMsg ),
	AOS3_LVO_Name( -150, DeleteRexxMsg ),
	AOS3_LVO_Name( -156, ClearRexxMsg ),
	AOS3_LVO_Name( -162, FillRexxMsg ),
	AOS3_LVO_Name( -168, IsRexxMsg ),
	// --
	AOS3_LVO_Name( -450, LockRexxBase ),
	AOS3_LVO_Name( -456, UnlockRexxBase ),
	AOS3_LVO_EndMarker()
};

// --
