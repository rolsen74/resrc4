
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

//enum RS4DecodeStat AOS3_Locale_Func_OpenLibrary( enum RS4ErrorCode *errcode, RS4Trace *rt );

// --

AOS3_LVOStruct AOS3_LocaleBase[] =
{
	AOS3_LVO_Name( -36, CloseCatalog ),
	AOS3_LVO_Name( -42, CloseLocale ),
	AOS3_LVO_Name( -48, ConvToLower ),
	AOS3_LVO_Name( -54, ConvToUpper ),
	AOS3_LVO_Name( -60, FormatDate ),
	AOS3_LVO_Name( -66, FormatString ),
	AOS3_LVO_Name( -72, GetCatalogStr ),
	AOS3_LVO_Name( -78, GetLocaleStr ),
	AOS3_LVO_Name( -84, IsAlNum ),
	AOS3_LVO_Name( -90, IsAlpha ),
	AOS3_LVO_Name( -96, IsCntrl ),
	AOS3_LVO_Name( -102, IsDigit ),
	AOS3_LVO_Name( -108, IsGraph ),
	AOS3_LVO_Name( -114, IsLower ),
	AOS3_LVO_Name( -120, IsPrint ),
	AOS3_LVO_Name( -126, IsPunct ),
	AOS3_LVO_Name( -132, IsSpace ),
	AOS3_LVO_Name( -138, IsUpper ),
	AOS3_LVO_Name( -144, IsXDigit ),
	AOS3_LVO_Name( -150, OpenCatalogA ),
	AOS3_LVO_Name( -156, OpenLocale ),
	AOS3_LVO_Name( -162, ParseDate ),
	AOS3_LVO_Name( -174, StrConvert ),
	AOS3_LVO_Name( -180, StrnCmp ),
	AOS3_LVO_EndMarker()
};

// --
