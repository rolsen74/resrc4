
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

//enum RS4DecodeStat AOS3_Disk_Func_OpenLibrary( enum RS4ErrorCode *errcode, RS4Trace *rt );

// --

AOS3_LVOStruct AOS3_DiskBase[] =
{
	AOS3_LVO_Name( -6, AllocUnit ),
	AOS3_LVO_Name( -12, FreeUnit ),
	AOS3_LVO_Name( -18, GetUnit ),
	AOS3_LVO_Name( -24, GiveUnit ),
	AOS3_LVO_Name( -30, GetUnitID ),
	AOS3_LVO_Name( -36, ReadUnitID ),
	AOS3_LVO_EndMarker()
};

// --
