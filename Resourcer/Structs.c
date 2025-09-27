
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

#include "ReSrc4.h"

// --

// -- Error Strings

CSTR ErrorStrings[RS4ErrStat_Last] =
{
	[RS4ErrStat_Okay]				= "No error",
	[RS4ErrStat_Error]				= "General Error",
	[RS4ErrStat_OutOfMemory]		= "Out of Memory",
	[RS4ErrStat_OutOfBounds]		= "RS4ErrStat_OutOfBounds",
	[RS4ErrStat_FileIO]				= "RS4ErrStat_FileIO",
	[RS4ErrStat_FileStructure]		= "RS4ErrStat_FileStructure",
	[RS4ErrStat_FileNotFound]		= "RS4ErrStat_FileNotFound",
	[RS4ErrStat_FileIsADir]			= "RS4ErrStat_FileIsADir",
	[RS4ErrStat_OpeningFile]		= "RS4ErrStat_OpeningFile",
	[RS4ErrStat_Internal]			= "RS4ErrStat_Internal",
	[RS4ErrStat_InvalidStructID]	= "RS4ErrStat_InvalidStructID",
	[RS4ErrStat_InvalidFileType]	= "RS4ErrStat_InvalidFileType",
	[RS4ErrStat_InvalidLabelType]	= "Unknown Label Type",
	[RS4ErrStat_UnsupportedFileType]= "RS4ErrStat_UnsupportedFileType",
	[RS4ErrStat_MD5Fail]			= "RS4ErrStat_MD5Fail",
	[RS4ErrStat_LineJunk]			= "RS4ErrStat_LineJunk",
	[RS4ErrStat_ExpectedHexVal]		= "RS4ErrStat_ExpectedHexVal",
	[RS4ErrStat_NeedOneDigit]		= "RS4ErrStat_NeedOneDigi",
	[RS4ErrStat_MissingInputFile]	= "RS4ErrStat_MissingInputFile",
	[RS4ErrStat_MissingOutputFile]	= "RS4ErrStat_MissingOutputFile",
	[RS4ErrStat_SameInOutFile]		= "RS4ErrStat_SameInOutFile",
	[RS4ErrStat_WrongType]			= "RS4ErrStat_WrongType",
};

// --

struct DataStructHeader	*DataStructTable[RS4StructID_Last] =
{
	[RS4StructID_Unknown]			= NULL,

	#ifdef SUPPORT_AMIGAOS3
	[RS4StructID_IntuiText]			= & Struct_IntuiText,
	[RS4StructID_BitMap]			= & Struct_BitMap,
	[RS4StructID_NewScreen]			= & Struct_NewScreen,
	[RS4StructID_Gadget]			= & Struct_Gadget,
	[RS4StructID_TextAttr]			= & Struct_TextAttr,
	[RS4StructID_RastPort]			= & Struct_RastPort,
	[RS4StructID_Rectangle]			= & Struct_Rectangle,
	[RS4StructID_LWGadData]			= & Struct_LWGadData,
	#endif
};

// --
