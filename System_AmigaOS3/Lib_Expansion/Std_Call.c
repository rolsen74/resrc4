
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

//enum RS4DecodeStat AOS3_Expansion_Func_OpenLibrary( enum RS4ErrorCode *errcode, RS4Trace *rt );

// --

AOS3_LVOStruct AOS3_ExpansionBase[] =
{
	AOS3_LVO_Name( -30, AddConfigDev ),
	AOS3_LVO_Name( -36, AddBootNode ),
	AOS3_LVO_Name( -42, AllocBoardMem ),
	AOS3_LVO_Name( -48, AllocConfigDev ),
	AOS3_LVO_Name( -54, AllocExpansionMem ),
	AOS3_LVO_Name( -60, ConfigBoard ),
	AOS3_LVO_Name( -66, ConfigChain ),
	AOS3_LVO_Name( -72, FindConfigDev ),
	AOS3_LVO_Name( -78, FreeBoardMem ),
	AOS3_LVO_Name( -84, FreeConfigDev ),
	AOS3_LVO_Name( -90, FreeExpansionMem ),
	AOS3_LVO_Name( -96, ReadExpansionByte ),
	AOS3_LVO_Name( -102, ReadExpansionRom ),
	AOS3_LVO_Name( -108, RemConfigDev ),
	AOS3_LVO_Name( -114, WriteExpansionByte ),
	AOS3_LVO_Name( -120, ObtainConfigBinding ),
	AOS3_LVO_Name( -126, ReleaseConfigBinding ),
	AOS3_LVO_Name( -132, SetCurrentBinding ),
	AOS3_LVO_Name( -138, GetCurrentBinding ),
	AOS3_LVO_Name( -144, MakeDosNode ),
	AOS3_LVO_Name( -150, AddDosNode ),
//	AOS3_LVO_Name( -156, ExpansionReserved26 ),
	AOS3_LVO_Name( -162, WriteExpansionWord ),
	AOS3_LVO_EndMarker()
};

// --
