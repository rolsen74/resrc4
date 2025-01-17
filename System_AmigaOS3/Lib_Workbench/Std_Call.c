
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

//enum RS4DecodeStat AOS3_Workbench_Func_OpenLibrary( enum RS4ErrorCode *errcode, RS4Trace *rt );

// --

AOS3_LVOStruct AOS3_WorkbenchBase[] =
{
	AOS3_LVO_Name( -30, UpdateWorkbench ),
	AOS3_LVO_Name( -36, QuoteWorkbench ),
	AOS3_LVO_Name( -42, StartWorkbench ),
	AOS3_LVO_Name( -48, AddAppWindowA ),
	AOS3_LVO_Name( -54, RemoveAppWindow ),
	AOS3_LVO_Name( -60, AddAppIconA ),
	AOS3_LVO_Name( -66, RemoveAppIcon ),
	AOS3_LVO_Name( -72, AddAppMenuItemA ),
	AOS3_LVO_Name( -78, RemoveAppMenuItem ),
	AOS3_LVO_Name( -90, WBInfo ),
	AOS3_LVO_EndMarker()
};

// --
