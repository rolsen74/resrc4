
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

struct DataStructHeader Struct_BitMap =
{
	40,						// sizeof( struct BitMap ) / bm_SIZEOF
	13,						// Data Entries
	"struct BitMap",		// STRUCTURE BitMap,0
{
	{ DST_Word,		2, "bm_BytesPerRow",	0 },
	{ DST_Word,		2, "bm_Rows",			0 },
	{ DST_Byte,		1, "bm_Flags",			0 },
	{ DST_Byte,		1, "bm_Depth",			0 },
	{ DST_Word,		2, "bm_Pad",			0 },
	{ DST_Pointer,	4, "bm_Planes[0]",		0 },
	{ DST_Pointer,	4, "bm_Planes[1]",		0 },
	{ DST_Pointer,	4, "bm_Planes[2]",		0 },
	{ DST_Pointer,	4, "bm_Planes[3]",		0 },
	{ DST_Pointer,	4, "bm_Planes[4]",		0 },
	{ DST_Pointer,	4, "bm_Planes[5]",		0 },
	{ DST_Pointer,	4, "bm_Planes[6]",		0 },
	{ DST_Pointer,	4, "bm_Planes[7]",		0 }},
};

// --

struct DataStructHeader Struct_IntuiText =
{
	20,						// sizeof( struct IntuiText ) / it_SIZEOF
	9,						// Data Entries
	"struct IntuiText",		// STRUCTURE IntuiText,0
{
	{ DST_Byte,		1, "it_FrontPen",		0 },
	{ DST_Byte,		1, "it_BackPen",		0 },
	{ DST_Byte,		1, "it_DrawMode",		0 },
	{ DST_Byte,		1, "it_KludgeFill00",	0 },
	{ DST_Word,		2, "it_LeftEdge",		0 },
	{ DST_Word,		2, "it_TopEdge",		0 },
	{ DST_Struct,	4, "it_ITextFont",		RS4StructID_TextAttr },		// struct TextAttr *
	{ DST_String,	4, "it_IText",			0 },
	{ DST_Struct,	4, "it_NextText",		RS4StructID_IntuiText }},	// struct IntuiText *
};

// --

struct DataStructHeader Struct_NewScreen =
{
	32,						// sizeof( struct NewScreen ) / it_SIZEOF
	13,						// Data Entries
	"struct NewScreen",		// STRUCTURE NewScreen,0
{
	{ DST_Word,		2, "ns_LeftEdge",		0 },
	{ DST_Word,		2, "ns_TopEdge",		0 },
	{ DST_Word,		2, "ns_Width",			0 },
	{ DST_Word,		2, "ns_Height",			0 },
	{ DST_Word,		2, "ns_Depth",			0 },
	{ DST_Byte,		1, "ns_DetailPen",		0 },
	{ DST_Byte,		1, "ns_BlockPen",		0 },
	{ DST_Word,		2, "ns_ViewModes",		0 },
	{ DST_Word,		2, "ns_Type",			0 },
	{ DST_Struct,	4, "ns_Font",			RS4StructID_TextAttr },		// struct TextAttr *
	{ DST_String,	4, "ns_DefaultTitle",	0 },	// UBYTE *
	{ DST_Struct,	4, "ns_Gadgets",		RS4StructID_Gadget },		// struct Gadget *
	{ DST_Struct,	4, "ns_CustomBitMap",	RS4StructID_BitMap }},		// struct BitMap *
};

// --

struct DataStructHeader Struct_Gadget =
{
	44,						// sizeof( struct Gadget ) / gg_SIZEOF
	13,						// Data Entries
	"struct Gadget",		// STRUCTURE Gadget,0
{
	{ DST_Struct,	4, "gg_NextGadget",		RS4StructID_Gadget },		// struct Gadget *
	{ DST_Word,		2, "gg_LeftEdge",		0 },
	{ DST_Word,		2, "gg_TopEdge",		0 },
	{ DST_Word,		2, "gg_Width",			0 },
	{ DST_Word,		2, "gg_Height",			0 },
	{ DST_Word,		2, "gg_Flags",			0 },
	{ DST_Word,		2, "gg_Activation",		0 },
	{ DST_Word,		2, "gg_GadgetType",		0 },
	{ DST_Pointer,	4, "gg_GadgetRender",	0 },
	{ DST_Pointer,	4, "gg_SelectRender",	0 },
	{ DST_Struct,	4, "gg_GadgetText",		RS4StructID_IntuiText },	// struct IntuiText *
	{ DST_Long,		4, "gg_MutualExclude",	0 },
	{ DST_Pointer,	4, "gg_SpecialInfo",	0 },
	{ DST_Word,		2, "gg_GadgetID",		0 },
	{ DST_Long,		4, "gg_UserData",		0 }},
};

// --

struct DataStructHeader Struct_TextAttr =
{
	8,						// sizeof( struct TextAttr ) / ta_SIZEOF
	4,						// Data Entries
	"struct TextAttr",		// STRUCTURE TextAttr,0
{
	{ DST_String,	4, "ta_Name",			0 },	// STRPTR
	{ DST_Word,		2, "ta_YSize",			0 },
	{ DST_Byte,		1, "ta_Style",			0 },
	{ DST_Byte,		1, "ta_Flags",			0 }},
};

// --

struct DataStructHeader Struct_RastPort =
{
	100,					// sizeof( struct RastPort ) / rp_SIZEOF
	53,						// Data Entries
	"struct RastPort",		// STRUCTURE RastPort,0
{
	{ DST_Pointer,	4, "rp_Layer",			0 },	// struct Layer *
	{ DST_Struct,	4, "rp_BitMap",			RS4StructID_BitMap },	// struct BitMap *
	{ DST_Pointer,	4, "rp_AreaPtrn",		0 },	// UWORD *
	{ DST_Pointer,	4, "rp_TmpRas",			0 },	// struct TmpRas *
	{ DST_Pointer,	4, "rp_AreaInfo",		0 },	// struct AreaInfo *
	{ DST_Pointer,	4, "rp_GelsInfo",		0 },	// struct GelsInfo *
	{ DST_Byte,		1, "rp_Mask",			0 },
	{ DST_Byte,		1, "rp_FgPen",			0 },
	{ DST_Byte,		1, "rp_BgPen",			0 },
	{ DST_Byte,		1, "rp_AOLPen",			0 },
	{ DST_Byte,		1, "rp_DrawMode",		0 },
	{ DST_Byte,		1, "rp_AreaPtSz",		0 },
	{ DST_Byte,		1, "rp_linpatcnt",		0 },
	{ DST_Byte,		1, "rp_Dummy",			0 },
	{ DST_Word,		2, "rp_Flags",			0 },
	{ DST_Word,		2, "rp_LinePtrn",		0 },
	{ DST_Word,		2, "rp_cp_x",			0 },
	{ DST_Word,		2, "rp_cp_y",			0 },
	{ DST_Byte,		1, "rp_minterms[0]",	0 },
	{ DST_Byte,		1, "rp_minterms[1]",	0 },
	{ DST_Byte,		1, "rp_minterms[2]",	0 },
	{ DST_Byte,		1, "rp_minterms[3]",	0 },
	{ DST_Byte,		1, "rp_minterms[4]",	0 },
	{ DST_Byte,		1, "rp_minterms[5]",	0 },
	{ DST_Byte,		1, "rp_minterms[6]",	0 },
	{ DST_Byte,		1, "rp_minterms[7]",	0 },
	{ DST_Word,		2, "rp_PenWidth",		0 },
	{ DST_Word,		2, "rp_PenHeight",		0 },
	{ DST_Struct,	4, "rp_Font",			RS4StructID_TextAttr },		// struct TextFont *
	{ DST_Byte,		1, "rp_AlgoStyle",		0 },
	{ DST_Byte,		1, "rp_TxFlags",		0 },
	{ DST_Word,		2, "rp_TxHeight",		0 },
	{ DST_Word,		2, "rp_TxWidth",		0 },
	{ DST_Word,		2, "rp_TxBaseline",		0 },
	{ DST_Word,		2, "rp_TxSpacing",		0 },
	{ DST_Long,		4, "rp_RP_User",		0 },
	{ DST_Long,		4, "rp_longreserved[0]",	0 },
	{ DST_Long,		4, "rp_longreserved[1]",	0 },
	{ DST_Word,		2, "rp_wordreserved[0]",	0 },
	{ DST_Word,		2, "rp_wordreserved[1]",	0 },
	{ DST_Word,		2, "rp_wordreserved[2]",	0 },
	{ DST_Word,		2, "rp_wordreserved[3]",	0 },
	{ DST_Word,		2, "rp_wordreserved[4]",	0 },
	{ DST_Word,		2, "rp_wordreserved[5]",	0 },
	{ DST_Word,		2, "rp_wordreserved[6]",	0 },
	{ DST_Byte,		1, "rp_reserved[0]",	0 },
	{ DST_Byte,		1, "rp_reserved[1]",	0 },
	{ DST_Byte,		1, "rp_reserved[2]",	0 },
	{ DST_Byte,		1, "rp_reserved[3]",	0 },
	{ DST_Byte,		1, "rp_reserved[4]",	0 },
	{ DST_Byte,		1, "rp_reserved[5]",	0 },
	{ DST_Byte,		1, "rp_reserved[6]",	0 },
	{ DST_Byte,		1, "rp_reserved[7]",	0 }},
};

// --

struct DataStructHeader Struct_Rectangle =
{
	8,						// sizeof( struct Rectangle ) / ra_SIZEOF
	4,						// Data Entries
	"struct Rectangle",		// STRUCTURE Rectangle,0
{
	{ DST_Word,		2, "ra_MinX",			0 },
	{ DST_Word,		2, "ra_MinY",			0 },
	{ DST_Word,		2, "ra_MaxX",			0 },
	{ DST_Word,		2, "ra_MaxY",			0 }},
};

// --

struct DataStructHeader Struct_LWGadData =
{
	32,						// Size
	13,						// Data Entries
	"LW Gadget Data/Info",	//
{
	{ DST_Word,		2, "lwg_1",				0 },	//  0 : long?
	{ DST_Word,		2, "lwg_2",				0 },	//  2 :
	{ DST_Long,		4, "lwg_3",				0 },	//  4 : long
	{ DST_Word,		2, "lwg_5",				0 },	//  8 :
	{ DST_Word,		2, "lwg_type",			0 },	// 10 : 1 = Button, 2 = CheckBox, 3 = Popup, 4 = <> Value, 5 = Slider, 
	{ DST_Word,		2, "lwg_xpos",			0 },	// 12 : 6 = String, 7 = Tab/Page, 8 = Group?
	{ DST_Word,		2, "lwg_ypos",			0 },	// 14 :
	{ DST_Word,		2, "lwg_width",			0 },	// 16 :
	{ DST_Word,		2, "lwg_height",		0 },	// 18 :
	{ DST_Word,		2, "lwg_11",			0 },	// 20 : Used by Mx Buttons .. seen 6, 9, F or %0110, %1001, %1111
	{ DST_Word,		2, "lwg_ID?",			0 },	// 22 :
	{ DST_String,	4, "lwg_Label",			0 },	// 24 :
	{ DST_String,	4, "lwg_Title",			0 }},	// 28 :
};

// --

struct DataStructHeader Struct_Interrupt =
{
	22,						// sizeof( struct Interrupt ) / IS_SIZE
	7,						// Data Entries
	"struct Interrupt",		// STRUCTURE IS,LN_SIZE
{
	{ DST_Struct,	4, "LN_SUCC",			RS4StructID_Interrupt },
	{ DST_Struct,	4, "LN_PRED",			RS4StructID_Interrupt },
	{ DST_Byte,		1, "LN_TYPE",			0 },
	{ DST_Byte,		1, "LN_PRI",			0 },
	{ DST_String,	4, "LN_NAME",			0 },
	{ DST_Pointer,	4, "IS_DATA",			0 },
//	{ DST_Code,		4, "IS_CODE",			0 }},
	{ DST_Pointer,	4, "IS_CODE",			0 }},
};

// --
