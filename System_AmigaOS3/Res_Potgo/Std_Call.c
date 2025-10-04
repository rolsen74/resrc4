
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

//enum RS4DecodeStat AOS3_Potgo_Func_OpenLibrary( enum RS4ErrorCode *errcode, RS4Trace *rt );

// --

AOS3_LVOStruct AOS3_PotgoBase[] =
{
	AOS3_LVO_Name( -6, AllocPotBits ),
	AOS3_LVO_Name( -12, FreePotBits ),
	AOS3_LVO_Name( -18, WritePotgo ),
	AOS3_LVO_EndMarker()
};

// --
