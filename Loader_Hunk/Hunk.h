
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

#ifndef LOADER_HUNK_H
#define LOADER_HUNK_H 1

#ifndef RESOURCER_RESRC4_H
#error ReSrc4.h must be included first
#endif

// --

#define HUNK_CODE			0x000003E9
#define HUNK_DATA			0x000003EA
#define HUNK_BSS			0x000003EB
#define HUNK_RELOC32		0x000003EC
#define HUNK_RELOC16		0x000003ED
#define HUNK_RELOC8			0x000003EE
#define HUNK_SYMBOL			0x000003F0
#define HUNK_DEBUG			0x000003F1
#define HUNK_END			0x000003F2
#define HUNK_HEADER			0x000003F3
#define HUNK_DREL32			0x000003F7	// Same as HUNK_RELOC32SHORT
#define HUNK_RELOC32SHORT	0x000003FC	// Same as HUNK_DREL32
#define HUNK_RELRELOC32		0x000003FD

// --

enum HunkMemType
{
	HUNK_MEMTYPE_Unknown = 0,
	HUNK_MEMTYPE_Chip,
	HUNK_MEMTYPE_Fast,
	HUNK_MEMTYPE_Public,
	HUNK_MEMTYPE_Last,
};

// --

struct HunkFileHeader
{
	S32						hfh_HunkFirst;
	S32						hfh_HunkLast;
};

// --

struct HunkFileSection
{
	enum HunkMemType		hfs_MemoryType;
};

// --

struct HunkTrace
{
//	RS4FileHeader *			ms_HunkStruct;
//	RS4FileSection *		ms_HunkNode;
//	S32						ms_MemoryAdr;
//	MEM 					ms_MemoryBuf;
//	MEM 					ms_MemoryType;
//	U32						ms_Opcode;
//	S32						ms_OpcodeSize;
//	S32						ms_ClearRegMask;
//	S32						ms_LibCall;				// Well Possible LibCall
//	S32						ms_IsPea;

	CSTR 					ms_Str_Opcode;
	STR 					ms_Buf_Argument;
	STR 					ms_Buf_Comment;			// Only used by Build/Save Source
	RS4Label *				ms_Ptr_Label;
	S64						ms_StartAddr;

//	enum M68KStat			ms_DecodeStatus;
//	S32						ms_JumpTable;
//	S32						ms_LastOpcode;			// Rts or Jmp .. sets this
//	enum M68KOpcodeSize		ms_ArgType;				// Opcode Type (b/w/l)
//	S32						ms_ArgSize;				// Opcode Size in bytes
//	S32						ms_ArgEMode;			// Effective Addreess Mode
//	S32						ms_ArgEReg;				// Effective Addreess Register
//	S32						ms_DecMode;
//	S32						ms_DoExternal;
//
//	struct M68kRegister *	ms_CurRegister;
//	struct M68kRegister		ms_JmpRegister;
//	struct M68kRegister		ms_SrcRegister;
//	struct M68kRegister		ms_DstRegister;
//	struct M68kRegister		ms_Registers[16];
};

// --

enum RS4FuncStat Hunk_ParseFile( enum RS4ErrorCode *errcode, RS4FileHeader *fh );

#endif
