
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
// Single Arg

static AOS3_RegStruct _Interrupt_A1[] =
{
	AOS3_REG_Struct( M68KREGT_A1, RS4StructID_Interrupt ),
	AOS3_REG_EndMarker()
};

// --
// Single Args

#define AOS3_AddICRVector_Regs		_Interrupt_A1
#define AOS3_RemICRVector_Regs		_Interrupt_A1

// --

AOS3_LVOStruct AOS3_CiaBase[] =
{
	AOS3_LVO_Regs( -6, AddICRVector ),
	AOS3_LVO_Regs( -12, RemICRVector ),
	AOS3_LVO_Name( -18, AbleICR ),
	AOS3_LVO_Name( -24, SetICR ),
	AOS3_LVO_EndMarker()
};

// --
