
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

//enum RS4DecodeStat AOS3_Console_Func_OpenLibrary( enum RS4ErrorCode *errcode, RS4Trace *rt );

// --

AOS3_LVOStruct AOS3_ConsoleBase[] =
{
	AOS3_LVO_Name( -42, CDInputHandler ),
	AOS3_LVO_Name( -48, RawKeyConvert ),
	AOS3_LVO_Name( -54, GetConSnip ),
	AOS3_LVO_Name( -60, SetConSnip ),
	AOS3_LVO_Name( -66, AddConSnipHook ),
	AOS3_LVO_Name( -72, RemConSnipHook ),
	AOS3_LVO_EndMarker()
};

// --
