
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

static AOS3_RegStruct _String_A0[] =
{
	AOS3_REG_String( M68KREGT_A0 ),
	AOS3_REG_EndMarker()
};

static AOS3_RegStruct _String_A1[] =
{
	AOS3_REG_String( M68KREGT_A1 ),
	AOS3_REG_EndMarker()
};

#define AOS3_SetRexxPort_Regs		_String_A1
#define AOS3_ASyncRexxCmd_Regs		_String_A0
#define AOS3_SyncRexxCmd_Regs		_String_A0

// --

AOS3_LVOStruct AOS3_RexxappBase[] =
{
	AOS3_LVO_Regs( -30, SetRexxPort ),
	AOS3_LVO_Name( -36, ReceiveRexx ),
	AOS3_LVO_Name( -42, FreeRexxPort ),
	AOS3_LVO_Name( -48, SendRexxCmd ),
	AOS3_LVO_Regs( -54, ASyncRexxCmd ),
	AOS3_LVO_Regs( -60, SyncRexxCmd ),
	AOS3_LVO_Name( -66, SetupResults ),
	AOS3_LVO_EndMarker()
};

// --
