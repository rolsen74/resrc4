
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

#ifndef M68KSIZE_AMIGA_H
#define M68KSIZE_AMIGA_H 1

#ifndef RESOURCER_RESRC4_H
#error ReSrc4.h must be included first
#endif

// --

typedef struct _AOS3_RegStruct	AOS3_RegStruct;
typedef struct _AOS3_LVOStruct	AOS3_LVOStruct;

#define AOS3_LVO_Name(x,y)		{ 0, x, "_LVO" #y,	AOS3_LVOType_Name,	NULL,				NULL }
#define AOS3_LVO_Func(x,y)		{ 0, x, "_LVO" #y,	AOS3_LVOType_Func,	AOS3_##y##_Func,	NULL }
#define AOS3_LVO_Regs(x,y)		{ 0, x, "_LVO" #y,	AOS3_LVOType_Regs,	NULL,				AOS3_##y##_Regs }
#define AOS3_LVO_EndMarker()	{ 0, 0, NULL,		AOS3_LVOType_End,	NULL,				NULL }

#define AOS3_REG_Code(x)		{ x, AOS3_RegType_Code, 0 }
#define AOS3_REG_String(x)		{ x, AOS3_RegType_String, 0 }
#define AOS3_REG_Struct(x,y)	{ x, AOS3_RegType_Struct, y }
#define AOS3_REG_EndMarker()	{ -1, 0, 0 }

enum AOS3_RegType
{
	AOS3_RegType_String = 0x10,
	AOS3_RegType_Struct,
	AOS3_RegType_Code,
};

enum AOS3_LVOType
{
	AOS3_LVOType_End = 0,
	AOS3_LVOType_Name = 0x20,	// Only define Name and Offset
	AOS3_LVOType_Regs,			// Name+Offset and Reg struct
	AOS3_LVOType_Func,			// Name+Offset and Custom Function
};

// Used by LabelType and RegType
// Libraries, Devices and Resource Bases
enum AOS3_LibType
{
	AOS3_LibType_AmigaguideBase,
	AOS3_LibType_AslBase,
	AOS3_LibType_BattclockBase,
	AOS3_LibType_BattmemBase,
	AOS3_LibType_BulletBase,
	AOS3_LibType_CardresBase,
	AOS3_LibType_CiaBase,
	AOS3_LibType_ColorwheelBase,
	AOS3_LibType_CommoditiesBase,
//	AOS3_LibType_ConsoleBase,
	AOS3_LibType_DatatypesBase,
	AOS3_LibType_DiskBase,
	AOS3_LibType_DiskfontBase,
	AOS3_LibType_DosBase,
	AOS3_LibType_DTClassBase,
	AOS3_LibType_ExecBase,
	AOS3_LibType_ExpansionBase,
	AOS3_LibType_GadToolsBase,
	AOS3_LibType_GraphicsBase,
	AOS3_LibType_IconBase,
	AOS3_LibType_IFFParseBase,
//	AOS3_LibType_InputBase,
	AOS3_LibType_IntuitionBase,
	AOS3_LibType_KeymapBase,
	AOS3_LibType_LayersBase,
	AOS3_LibType_LocaleBase,
	AOS3_LibType_LowlevelBase,
	AOS3_LibType_MathffpBase,
	AOS3_LibType_MathieeedoubbasBase,
	AOS3_LibType_MathieeedoubtransBase,
	AOS3_LibType_MathieeesingbasBase,
	AOS3_LibType_MathieeesingtransBase,
	AOS3_LibType_MathtransBase,
	AOS3_LibType_MiscBase,
	AOS3_LibType_NonvolatileBase,
	AOS3_LibType_PotgoBase,
//	AOS3_LibType_RamdriveBase,
	AOS3_LibType_RexxsyslibBase,
//	AOS3_LibType_TimerBase,
	AOS3_LibType_TranslatorBase,
	AOS3_LibType_UtilityBase,
	AOS3_LibType_WorkbenchBase,
};

// --

struct _AOS3_RegStruct
{
	enum M68kRegisterType	Reg3;
	enum AOS3_RegType		Type3;
	S32						Val3;
};

struct _AOS3_LVOStruct
{
	S16						Used2;
	S16						Offset2;
	STR 					Name2;
	enum AOS3_LVOType		Type2;
	enum RS4DecodeStat		(*Func2)( enum RS4ErrorCode *errcode, RS4Trace *rt );
	AOS3_RegStruct *		Regs2;
};

// --

struct AOS3LVOStruct
{
	S16		Used;
	S16		Offset;
	STR 	Name;
	enum RS4DecodeStat (*Func)( enum RS4ErrorCode *errcode, RS4Trace *rt );
};

// --

// -- Protos

enum RS4FuncStat AmigaOS3_SaveLibFunc( enum RS4ErrorCode *errcode );

extern AOS3_LVOStruct AOS3_AmigaguideBase[];
extern AOS3_LVOStruct AOS3_AslBase[];
extern AOS3_LVOStruct AOS3_BattclockBase[];
extern AOS3_LVOStruct AOS3_BattmemBase[];
extern AOS3_LVOStruct AOS3_BulletBase[];
extern AOS3_LVOStruct AOS3_CardresBase[];
extern AOS3_LVOStruct AOS3_CiaBase[];
extern AOS3_LVOStruct AOS3_ColorwheelBase[];
extern AOS3_LVOStruct AOS3_CommoditiesBase[];
extern AOS3_LVOStruct AOS3_ConsoleBase[];
extern AOS3_LVOStruct AOS3_DatatypesBase[];
extern AOS3_LVOStruct AOS3_DiskBase[];
extern AOS3_LVOStruct AOS3_DiskfontBase[];
extern AOS3_LVOStruct AOS3_DosBase[];
extern AOS3_LVOStruct AOS3_DTClassBase[];
extern AOS3_LVOStruct AOS3_ExecBase[];
extern AOS3_LVOStruct AOS3_ExpansionBase[];
extern AOS3_LVOStruct AOS3_GadToolsBase[];
extern AOS3_LVOStruct AOS3_GraphicsBase[];
extern AOS3_LVOStruct AOS3_IconBase[];
extern AOS3_LVOStruct AOS3_IFFParseBase[];
extern AOS3_LVOStruct AOS3_InputBase[];
extern AOS3_LVOStruct AOS3_IntuitionBase[];
extern AOS3_LVOStruct AOS3_KeymapBase[];
extern AOS3_LVOStruct AOS3_LayersBase[];
extern AOS3_LVOStruct AOS3_LocaleBase[];
extern AOS3_LVOStruct AOS3_MathffpBase[];
extern AOS3_LVOStruct AOS3_MathieeedoubbasBase[];
extern AOS3_LVOStruct AOS3_MathieeedoubtransBase[];
extern AOS3_LVOStruct AOS3_MathieeesingbasBase[];
extern AOS3_LVOStruct AOS3_MathieeesingtransBase[];
extern AOS3_LVOStruct AOS3_MathtransBase[];
extern AOS3_LVOStruct AOS3_MiscBase[];
extern AOS3_LVOStruct AOS3_PotgoBase[];
extern AOS3_LVOStruct AOS3_RamdriveBase[];
extern AOS3_LVOStruct AOS3_RexxsyslibBase[];
extern AOS3_LVOStruct AOS3_TimerBase[];
extern AOS3_LVOStruct AOS3_UtilityBase[];
extern AOS3_LVOStruct AOS3_WorkbenchBase[];

// --

enum RS4FuncStat	AmigaOS3_Misc_Move_Get(			enum RS4ErrorCode *errcode, RS4Trace *rt, struct M68kRegister *cur, MEM mem, struct AmigaOS3_Misc_Move_GetSetStruct *gss );
enum RS4FuncStat	AmigaOS3_Misc_Move_Set(			enum RS4ErrorCode *errcode, RS4Trace *rt, struct M68kRegister *cur, MEM mem, struct AmigaOS3_Misc_Move_GetSetStruct *gss );
enum RS4FuncStat	AmigaOS3_FindLibFunc_Func(		enum RS4ErrorCode *errcode, RS4Trace *rt, enum RS4DecodeStat (*Func)( enum RS4ErrorCode *errcode, RS4Trace *rt ));
enum RS4FuncStat	AmigaOS3_FindLibFunc_Regs(		enum RS4ErrorCode *errcode, RS4Trace *rt, struct _AOS3_RegStruct *rs );

// --

extern struct DataStructHeader Struct_IntuiText;
extern struct DataStructHeader Struct_BitMap;
extern struct DataStructHeader Struct_NewScreen;
extern struct DataStructHeader Struct_Gadget;
extern struct DataStructHeader Struct_TextAttr;
extern struct DataStructHeader Struct_TextFont;
extern struct DataStructHeader Struct_RastPort;
extern struct DataStructHeader Struct_Rectangle;
extern struct DataStructHeader Struct_LWGadData;
extern struct DataStructHeader Struct_Interrupt;
extern struct DataStructHeader Struct_MsgPort;
extern struct DataStructHeader Struct_StackSwap;

#endif
