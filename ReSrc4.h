
/*
 * Copyright (c) 2014-2024 Rene W. Olsen < renewolsen @ gmail . com >
 *
 * This software is released under the GNU General Public License, version 3.
 * For the full text of the license, please visit:
 * https://www.gnu.org/licenses/gpl-3.0.html
 *
 * You can also find a copy of the license in the LICENSE file included with this software.
 */

#ifndef RESRC4_MAIN_H
#define RESRC4_MAIN_H 1

// --

#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// --

#define DATE				"12-Feb-2024"
#define YEAR				2024
#define VERSION				2
#define REVISION			5

// --

#define MAX_REFHASH			29			// Prime Nr .. bigger value more memory pr. HunkNode
#define MAX_LABHASH			83			// Prime Nr .. Global Label Hash
#define MAX_LabelName		32

#define HUNK_CODE			0x000003E9
#define HUNK_DATA			0x000003EA
#define HUNK_BSS			0x000003EB
#define HUNK_RELOC32		0x000003EC
#define HUNK_SYMBOL			0x000003F0
#define HUNK_DEBUG			0x000003F1
#define HUNK_END			0x000003F2
#define HUNK_HEADER			0x000003F3
#define HUNK_DREL32			0x000003F7	// Same as HUNK_RELOC32SHORT
#define HUNK_RELOC32SHORT	0x000003FC	// Same as HUNK_DREL32

// --

#ifdef BIGENDIAN

#define SWAP16(x)	x
#define SWAP32(x)	x

#else

#define SWAP16(x)	(((x) & 0x00ffU ) << 8 | \
					 ((x) & 0xff00U ) >> 8 )

#define SWAP32(x)	(((x) & 0x000000ffUL ) << 24 | \
					 ((x) & 0x0000ff00UL ) <<  8 | \
					 ((x) & 0x00ff0000UL ) >>  8 | \
					 ((x) & 0xff000000UL ) >> 24 )

#endif

#ifndef MIN
#define MIN(a, b)	((a) < (b) ? (a) : (b))
#endif

// --

// Used by LabelType and RegType
// Libraries, Devices and Resource Bases
enum LibType 
{
	LIBT_AmigaguideBase,
	LIBT_AslBase,
	LIBT_BattclockBase,
	LIBT_BattmemBase,
	LIBT_BulletBase,
	LIBT_CardresBase,
	LIBT_CiaBase,
	LIBT_ColorwheelBase,
	LIBT_CommoditiesBase,
//	LIBT_ConsoleBase,
	LIBT_DatatypesBase,
	LIBT_DiskBase,
	LIBT_DiskfontBase,
	LIBT_DosBase,
	LIBT_DTClassBase,
	LIBT_ExecBase,
	LIBT_ExpansionBase,
	LIBT_GadToolsBase,
	LIBT_GraphicsBase,
	LIBT_IconBase,
	LIBT_IFFParseBase,
//	LIBT_InputBase,
	LIBT_IntuitionBase,
	LIBT_KeymapBase,
	LIBT_LayersBase,
	LIBT_LocaleBase,
	LIBT_LowlevelBase,
	LIBT_MathffpBase,
	LIBT_MathieeedoubbasBase,
	LIBT_MathieeedoubtransBase,
	LIBT_MathieeesingbasBase,
	LIBT_MathieeesingtransBase,
	LIBT_MathtransBase,
	LIBT_MiscBase,
	LIBT_NonvolatileBase,
	LIBT_PotgoBase,
//	LIBT_RamdriveBase,
	LIBT_RexxsyslibBase,
//	LIBT_TimerBase,
	LIBT_TranslatorBase,
	LIBT_UtilityBase,
	LIBT_WorkbenchBase,
};

enum LabelType
{
	LT_Unset = 0,		// Starts blank/unset
	LT_Unknown,			// IF Label gets more than one Type, then we don't know what to use
	LT_Code,
	LT_String,
	LT_RelativeWord,
	LT_Pointer,
};

enum LabelPtrType
{
	LPT_Unset,			// Never Set
	LPT_Error,			// Two or more diffrent type.. not sure what it is 
	LPT_Library,
};

enum MemoryType
{
    MT_Unset = 5,		// Must not be Zero
    MT_Data = 13,
	MT_Code = 17,
};

enum Register
{
	REG_Dx = 0,
	REG_D0 = 0,
	REG_D1,
	REG_D2,
	REG_D3,
	REG_D4,
	REG_D5,
	REG_D6,
	REG_D7,
	REG_Ax = 8,
	REG_A0 = 8,
	REG_A1,
	REG_A2,
	REG_A3,
	REG_A4,
	REG_A5,
	REG_A6,
	REG_A7,
};

enum RegisterType
{
	RT_Unknown = 0,
	RT_Label,
	RT_Address,
	RT_Library,
};

// --

struct Node
{
	void *					Next;
	void *					Prev;
};

struct Header
{
	void *					Head;
	void *					Tail;
	int						Nodes;
};

struct M68kRegister
{
	int32_t					mr_Type;
//	int32_t					mr_Number;			// Temp debug
	int32_t					mr_Address;
	struct HunkLabel *		mr_LabelNode;
};

struct BranceNode
{
	struct Node				bn_Node;
	struct HunkNode *		bn_HunkNode;
	uint32_t				bn_HunkAddress;
	int32_t					bn_HunkOffset;
	struct M68kRegister		bn_Registers[16];
};

enum M68KStat
{
	MSTAT_UnknownCmd,
	MSTAT_Opcode,
	MSTAT_Error,
};

enum OpcodeSize
{
	OS_Unsized,
	OS_Byte,		// move.b
	OS_Word,		// move.w
	OS_Long,		// move.l
	OS_Single,		// fmove.s
	OS_Double,		// fmove.d
	OS_Extended,	// fmove.x
	OS_Packed,		// fmove.p
};

struct M68kStruct
{
	struct HunkStruct *		ms_HunkStruct;
	struct HunkNode *		ms_HunkNode;
	int32_t					ms_MemoryAdr;
	uint8_t *				ms_MemoryBuf;
	uint8_t *				ms_MemoryType;
	uint32_t				ms_Opcode;
	int						ms_OpcodeSize;
	int						ms_ClearRegMask;
	int						ms_LibCall;				// Well Possible LibCall
	int						ms_IsPea;

	const char *			ms_Str_Opcode;
	char *					ms_Buf_Argument;
	char *					ms_Buf_Comment;			// Only used by Build/Save Source
	struct HunkLabel *		ms_Ptr_Label;
	uint32_t				ms_StartAddr;

	enum M68KStat			ms_DecodeStatus;
	int						ms_JumpTable;
	int						ms_LastOpcode;			// Rts or Jmp .. sets this
	enum OpcodeSize			ms_ArgType;				// Opcode Type (b/w/l)
	int						ms_ArgSize;				// Opcode Size in bytes
	int						ms_ArgEMode;			// Effective Addreess Mode
	int						ms_ArgEReg;				// Effective Addreess Register
	int						ms_DecMode;
	int						ms_DoExternal;

	struct M68kRegister *	ms_CurRegister;
	struct M68kRegister		ms_JmpRegister;
	struct M68kRegister		ms_SrcRegister;
	struct M68kRegister		ms_DstRegister;
	struct M68kRegister		ms_Registers[16];
};

struct HunkRef
{
	struct Node				hr_Node;
	struct HunkRef *		hr_HashPtr;
	// --
	int32_t					hr_Address;
	int32_t					hr_Offset;
	int						hr_Used;
};

struct HunkLabel
{
	struct Node				hl_Node;
	struct HunkLabel *		hl_HashPtr;
	// --
	struct HunkLabel *		hl_Parent;
	struct HunkNode *		hl_HunkNode;
	struct HunkLabel *	    hl_Label_Ref;
	int32_t					hl_Label_Address;		// Virtual 32bit memory address
	uint8_t *				hl_Label_Memory;
	int32_t					hl_Label_Offset;
	enum LabelType			hl_Label_Type;
	int						hl_Label_SubType;
	int						hl_Label_FixedType;
	int32_t					hl_Label_Size;
	char					hl_Label_Name[ MAX_LabelName ];
	struct M68kRegister		hl_Registers[16];
};

struct HunkNode
{
	int						hn_HunkNr;
	int						hn_HunkType;
	int						hn_HunkMemType;

	int32_t					hn_MemorySize;
	uint8_t *				hn_MemoryType;
	int32_t					hn_MemoryAdr;			// Virtual 32bit memory address
	uint8_t *				hn_MemoryBuf;

	int32_t					hn_DataSize;
	int32_t					hn_FileOffset;

	struct Header			hn_Labels;
	struct Header			hn_Refs;
	struct HunkRef *		hn_Refs2[MAX_REFHASH];
};

struct HunkInfo
{
	struct HunkNode *		hi_HunkNode;
	int32_t					hi_MemoryAdr;			// Virtual 32bit memory address
	int32_t					hi_MemorySize;
};

struct HunkStruct
{
	int						hs_PassNr;				// 1 = Trace, 2 = Label, 3 = Build Src
	uint8_t *				hs_FileBuffer;
	int						hs_FileSize;
	char					hs_FileMD5[34];			// Ascii
	char *					hs_FileName;

	struct HunkInfo *		hs_HunkArray;
	int						hs_HunkArraySize;

	int						hs_HunkFirst;
	int						hs_HunkLast;

	struct Header			hs_SourceList;
	struct Header			hs_ExtLabelList;
	struct HunkLabel *		hs_LabelHash[MAX_LABHASH];
};

struct SourceNode
{
	struct Node				sn_Node;
	uint32_t				sn_Address;	
	struct HunkLabel *		sn_Label;
	const char *			sn_Opcode;
	char *				    sn_Argument;
	char *					sn_Comment;
};

// --

struct Misc_Move_GetSetStruct
{
	int					SrcType;
	int					LibType;
	struct HunkLabel *	Label;
	struct M68kRegister Reg;
};

// -- Extern
extern const char *FPx_RegNames[8];
extern const char *Ax_RegNames[8];
extern const char *Dx_RegNames[8];
extern const char *scale_Names[4];
extern char *SaveLineBuffer;
extern char *ConfigFile;
extern char *OutputFile;
extern char *InputFile;
extern char *LabNames;
extern char *ExtNames;
extern char *SecNames;
extern int ShortOpcodes;
extern int OpcodeTabs;
extern int DebugInfo;
extern int LabTabs;
extern int ArgTabs;

// -- BuildSource
int					BuildSource(			struct HunkStruct *hs );

// -- Config
int					HandleConfigFile(		struct HunkStruct *hs );

// -- Hunk
struct HunkStruct *	Hunk_LoadFile(			char *filename );
void				Hunk_FreeData(			struct HunkStruct *hs );
struct HunkNode *	Hunk_FindNode(			struct HunkStruct *hs, int32_t addr );
struct HunkLabel *	Hunk_AddLabel(			struct HunkStruct *hs, int32_t addr, enum LabelType type );
struct HunkLabel *	Hunk_AddLabel2(			struct HunkStruct *hs, struct HunkNode *hn, int32_t addr, enum LabelType type );
struct HunkLabel *	Hunk_AddExtLabel(		struct HunkStruct *hs, int32_t addr, enum LabelType type );
struct HunkRef *	Hunk_FindRef(			struct HunkNode *hn, int32_t adr );
struct HunkLabel *	Hunk_FindLabel(			struct HunkStruct *hs, int32_t addr );
void				BuildLabelString(		struct HunkLabel *hl, char *buf );

// -- Label Magic
int					LabelMagic(				struct HunkStruct *hs );

// -- M68k
void				M68k_Decode(			struct M68kStruct *ms );
void				M68k_EffectiveAddress(	struct M68kStruct *ms );
char *				M68k_FindLibFunc(		struct M68kStruct *ms, int16_t val );
char *				M68k_Lib_Amigaguide(	struct M68kStruct *ms, int16_t val );
int					Save_Lib_Amigaguide(	void );
char *				M68k_Lib_Asl(			struct M68kStruct *ms, int16_t val );
int					Save_Lib_Asl(			void );
char *				M68k_Lib_Battclock(		struct M68kStruct *ms, int16_t val );
int					Save_Lib_Battclock(		void );
char *				M68k_Lib_Battmem(		struct M68kStruct *ms, int16_t val );
int					Save_Lib_Battmem(		void );
char *				M68k_Lib_Bullet(		struct M68kStruct *ms, int16_t val );
int					Save_Lib_Bullet(		void );
char *				M68k_Lib_Cardres(		struct M68kStruct *ms, int16_t val );
int					Save_Lib_Cardres(		void );
char *				M68k_Lib_Cia(			struct M68kStruct *ms, int16_t val );
int					Save_Lib_Cia(			void );
char *				M68k_Lib_Colorwheel(	struct M68kStruct *ms, int16_t val );
int					Save_Lib_Colorwheel(	void );
char *				M68k_Lib_Commodities(	struct M68kStruct *ms, int16_t val );
int					Save_Lib_Commodities(	void );
//char *				M68k_Lib_Console(		struct M68kStruct *ms, int16_t val );
//int					Save_Lib_Console(		void );
char *				M68k_Lib_Datatypes(		struct M68kStruct *ms, int16_t val );
int					Save_Lib_Datatypes(		void );
char *				M68k_Lib_Disk(			struct M68kStruct *ms, int16_t val );
int					Save_Lib_Disk(			void );
char *				M68k_Lib_Diskfont(		struct M68kStruct *ms, int16_t val );
int					Save_Lib_Diskfont(		void );
char *				M68k_Lib_Dos(			struct M68kStruct *ms, int16_t val );
int					Save_Lib_Dos(			void );
char *				M68k_Lib_DTClass(		struct M68kStruct *ms, int16_t val );
int					Save_Lib_DTClass(		void );
char *				M68k_Lib_Exec(			struct M68kStruct *ms, int16_t val );
int					Save_Lib_Exec(			void );
char *				M68k_Lib_Expansion(		struct M68kStruct *ms, int16_t val );
int					Save_Lib_Expansion(		void );
char *				M68k_Lib_Gadtools(		struct M68kStruct *ms, int16_t val );
int					Save_Lib_Gadtools(		void );
char *				M68k_Lib_Graphics(		struct M68kStruct *ms, int16_t val );
int					Save_Lib_Graphics(		void );
char *				M68k_Lib_Icon(			struct M68kStruct *ms, int16_t val );
int					Save_Lib_Icon(			void );
char *				M68k_Lib_IFFParse(		struct M68kStruct *ms, int16_t val );
int					Save_Lib_IFFParse(		void );
//char *			M68k_Lib_Input(			struct M68kStruct *ms, int16_t val );
//int				Save_Lib_Input(			void );
char *				M68k_Lib_Intuition(		struct M68kStruct *ms, int16_t val );
int					Save_Lib_Intuition(		void );
char *				M68k_Lib_Keymap(		struct M68kStruct *ms, int16_t val );
int					Save_Lib_Keymap(		void );
char *				M68k_Lib_Layers(		struct M68kStruct *ms, int16_t val );
int					Save_Lib_Layers(		void );
char *				M68k_Lib_Locale(		struct M68kStruct *ms, int16_t val );
int					Save_Lib_Locale(		void );
char *				M68k_Lib_Lowlevel(		struct M68kStruct *ms, int16_t val );
int					Save_Lib_Lowlevel(		void );
char *				M68k_Lib_Mathffp(		struct M68kStruct *ms, int16_t val );
int					Save_Lib_Mathffp(		void );
char *				M68k_Lib_Mathieeedoubbas(	struct M68kStruct *ms, int16_t val );
int					Save_Lib_Mathieeedoubbas(	void );
char *				M68k_Lib_Mathieeedoubtrans(	struct M68kStruct *ms, int16_t val );
int					Save_Lib_Mathieeedoubtrans(	void );
char *				M68k_Lib_Mathieeesingbas(	struct M68kStruct *ms, int16_t val );
int					Save_Lib_Mathieeesingbas(	void );
char *				M68k_Lib_Mathieeesingtrans(	struct M68kStruct *ms, int16_t val );
int					Save_Lib_Mathieeesingtrans(	void );
char *				M68k_Lib_Mathtrans(		struct M68kStruct *ms, int16_t val );
int					Save_Lib_Mathtrans(		void );
char *				M68k_Lib_Misc(			struct M68kStruct *ms, int16_t val );
int					Save_Lib_Misc(			void );
char *				M68k_Lib_Nonvolatile(	struct M68kStruct *ms, int16_t val );
int					Save_Lib_Nonvolatile(	void );
char *				M68k_Lib_Potgo(			struct M68kStruct *ms, int16_t val );
int					Save_Lib_Potgo(			void );
//char *			M68k_Lib_Ramdrive(		struct M68kStruct *ms, int16_t val );
//int				Save_Lib_Ramdrive(		void );
char *				M68k_Lib_Rexxsyslib(	struct M68kStruct *ms, int16_t val );
int					Save_Lib_Rexxsyslib(	void );
//char *			M68k_Lib_Timer(			struct M68kStruct *ms, int16_t val );
//int				Save_Lib_Timer(			void );
char *				M68k_Lib_Translator(	struct M68kStruct *ms, int16_t val );
int					Save_Lib_Translator(	void );
char *				M68k_Lib_Utility(		struct M68kStruct *ms, int16_t val );
int					Save_Lib_Utility(		void );
char *				M68k_Lib_Workbench(		struct M68kStruct *ms, int16_t val );
int					Save_Lib_Workbench(		void );
void				Misc_Move_Get(			struct M68kStruct *ms, struct M68kRegister *cur, uint8_t *mem, struct Misc_Move_GetSetStruct *gss );
void				Misc_Move_Set(			struct M68kStruct *ms, struct M68kRegister *cur, uint8_t *mem, struct Misc_Move_GetSetStruct *gss );

// -- Misc
char *				myStrdup(				char *string );
void				Mark_NulString(			struct HunkLabel *hl );
void				Mark_D1_NulString(		struct M68kStruct *ms );
void				Mark_D1D2_NulString(	struct M68kStruct *ms );
void				Mark_D2_NulString(		struct M68kStruct *ms );
void				Mark_D3_NulString(		struct M68kStruct *ms );
void				Mark_A0_NulString(		struct M68kStruct *ms );
void				Mark_A1_NulString(		struct M68kStruct *ms );

// -- Nodes
void				myAddHead(				struct Header *h, void *new );
void				myAddTail(				struct Header *h, void *new );
void				myAddAfter( 			struct Header *h, void *cur, void *new );
void *				myRemHead(				struct Header *h );
void *				myGetHead(				struct Header *h );
void *				myGetTail(				struct Header *h );
void *				myGetNext(				void *cur );
void *				myGetPrev(				void *cur );

// -- SaveSource
int					SaveSource(				struct HunkStruct *hs, char *filename );
int					SaveWriteString(		char *buffer, size_t length );

// -- Trace
int					TraceHunk(				struct HunkStruct *hs );
int					Trace_AddBrance(		struct HunkStruct *hs, struct M68kStruct *ms, int32_t addr );

// -- Trace_JumpTables
int					Trace_JumpTable(		struct HunkStruct *hs, struct M68kStruct *ms );
int					JumpTable_Rel16( 		struct HunkStruct *hs, int32_t tabeladdr, int32_t relativeaddr, int entries );

// -- Opcodes
void Cmd_ABCD(		struct M68kStruct *ms );
void Cmd_ADD(		struct M68kStruct *ms );
void Cmd_ADDA(		struct M68kStruct *ms );
void Cmd_ADDI(		struct M68kStruct *ms );
void Cmd_ADDQ(		struct M68kStruct *ms );
void Cmd_ADDX(		struct M68kStruct *ms );
void Cmd_AND(		struct M68kStruct *ms );
void Cmd_ANDI(		struct M68kStruct *ms );
void Cmd_ASL(		struct M68kStruct *ms );
void Cmd_ASL2(		struct M68kStruct *ms );
void Cmd_ASR(		struct M68kStruct *ms );
void Cmd_ASR2(		struct M68kStruct *ms );
void Cmd_Bcc(		struct M68kStruct *ms );
void Cmd_BCHG(		struct M68kStruct *ms );
void Cmd_BCHG2(		struct M68kStruct *ms );
void Cmd_BCLR(		struct M68kStruct *ms );
void Cmd_BCLR2(		struct M68kStruct *ms );
void Cmd_BGND(		struct M68kStruct *ms );
void Cmd_BKPT(		struct M68kStruct *ms );
void Cmd_BSET(		struct M68kStruct *ms );
void Cmd_BSET2(		struct M68kStruct *ms );
void Cmd_BTST(		struct M68kStruct *ms );
void Cmd_BTST2(		struct M68kStruct *ms );
void Cmd_CALLM(		struct M68kStruct *ms );
void Cmd_CHK(		struct M68kStruct *ms );
void Cmd_CHK2(		struct M68kStruct *ms );
void Cmd_CLR(		struct M68kStruct *ms );
void Cmd_CMP(		struct M68kStruct *ms );
void Cmd_CMP2(		struct M68kStruct *ms );
void Cmd_CMPA(		struct M68kStruct *ms );
void Cmd_CMPI(		struct M68kStruct *ms );
void Cmd_CMPM(		struct M68kStruct *ms );
void Cmd_DBcc(		struct M68kStruct *ms );
void Cmd_DIV(		struct M68kStruct *ms );
void Cmd_DIVS_L(	struct M68kStruct *ms );
void Cmd_DIVU_L(	struct M68kStruct *ms );
void Cmd_EOR(		struct M68kStruct *ms );
void Cmd_EORI(		struct M68kStruct *ms );
void Cmd_EXG( 		struct M68kStruct *ms );
void Cmd_EXT(		struct M68kStruct *ms );
void Cmd_ILLEGAL(	struct M68kStruct *ms );
void Cmd_JMP(		struct M68kStruct *ms );
void Cmd_JSR(		struct M68kStruct *ms );
void Cmd_LEA(		struct M68kStruct *ms );
void Cmd_LINK(		struct M68kStruct *ms );
void Cmd_LINK_L(	struct M68kStruct *ms );
void Cmd_LSL(		struct M68kStruct *ms );
void Cmd_LSL2(		struct M68kStruct *ms );
void Cmd_LSR(		struct M68kStruct *ms );
void Cmd_LSR2(		struct M68kStruct *ms );
void Cmd_MOVE(		struct M68kStruct *ms );
void Cmd_MOVEA(		struct M68kStruct *ms );
void Cmd_MOVEM(		struct M68kStruct *ms );
void Cmd_MOVEP(		struct M68kStruct *ms );
void Cmd_MOVEQ(		struct M68kStruct *ms );
void Cmd_MUL(		struct M68kStruct *ms );
void Cmd_MULS_L(	struct M68kStruct *ms );
void Cmd_MULU_L(	struct M68kStruct *ms );
void Cmd_NBCD(		struct M68kStruct *ms );
void Cmd_NEG(		struct M68kStruct *ms );
void Cmd_NOP(		struct M68kStruct *ms );
void Cmd_NOT(		struct M68kStruct *ms );
void Cmd_OR(		struct M68kStruct *ms );
void Cmd_ORI(		struct M68kStruct *ms );
void Cmd_PACK(		struct M68kStruct *ms );
void Cmd_PEA(		struct M68kStruct *ms );
void Cmd_RESET(		struct M68kStruct *ms );
void Cmd_ROL(		struct M68kStruct *ms );
void Cmd_ROL2(		struct M68kStruct *ms );
void Cmd_ROR(		struct M68kStruct *ms );
void Cmd_ROR2(		struct M68kStruct *ms );
void Cmd_ROXL(		struct M68kStruct *ms );
void Cmd_ROXL2(		struct M68kStruct *ms );
void Cmd_ROXR(		struct M68kStruct *ms );
void Cmd_ROXR2(		struct M68kStruct *ms );
void Cmd_RTD(		struct M68kStruct *ms );
void Cmd_RTE(		struct M68kStruct *ms );
void Cmd_RTM(		struct M68kStruct *ms );
void Cmd_RTR(		struct M68kStruct *ms );
void Cmd_RTS(		struct M68kStruct *ms );
void Cmd_SBCD(		struct M68kStruct *ms );
void Cmd_Scc(		struct M68kStruct *ms );
void Cmd_SUB(		struct M68kStruct *ms );
void Cmd_SUBA(		struct M68kStruct *ms );
void Cmd_SUBI(		struct M68kStruct *ms );
void Cmd_SUBQ(		struct M68kStruct *ms );
void Cmd_SUBX(		struct M68kStruct *ms );
void Cmd_SWAP(		struct M68kStruct *ms );
void Cmd_TRAP(		struct M68kStruct *ms );
void Cmd_TRAPV(		struct M68kStruct *ms );
void Cmd_TST(		struct M68kStruct *ms );
void Cmd_UNLK(		struct M68kStruct *ms );
void Cmd_UNPK(		struct M68kStruct *ms );


void Cmd_FABS(		struct M68kStruct *ms );
void Cmd_FABS2(		struct M68kStruct *ms );
void Cmd_FACOS(		struct M68kStruct *ms );
void Cmd_FACOS2(	struct M68kStruct *ms );
void Cmd_FADD(		struct M68kStruct *ms );
void Cmd_FADD2(		struct M68kStruct *ms );
void Cmd_FASIN(		struct M68kStruct *ms );
void Cmd_FASIN2(	struct M68kStruct *ms );
void Cmd_FATAN(		struct M68kStruct *ms );
void Cmd_FATAN2(	struct M68kStruct *ms );
void Cmd_FATANH(	struct M68kStruct *ms );
void Cmd_FATANH2(	struct M68kStruct *ms );
void Cmd_FBcc(		struct M68kStruct *ms );
void Cmd_FCMP(		struct M68kStruct *ms );
void Cmd_FCMP2(		struct M68kStruct *ms );
void Cmd_FCOS(		struct M68kStruct *ms );
void Cmd_FCOS2(		struct M68kStruct *ms );
void Cmd_FCOSH(		struct M68kStruct *ms );
void Cmd_FCOSH2(	struct M68kStruct *ms );
void Cmd_FDABS(		struct M68kStruct *ms );
void Cmd_FDABS2(	struct M68kStruct *ms );
void Cmd_FDADD(		struct M68kStruct *ms );
void Cmd_FDADD2(	struct M68kStruct *ms );
void Cmd_FDDIV(		struct M68kStruct *ms );
void Cmd_FDDIV2(	struct M68kStruct *ms );
void Cmd_FDIV(		struct M68kStruct *ms );
void Cmd_FDIV2(		struct M68kStruct *ms );
void Cmd_FDMOVE(	struct M68kStruct *ms );
void Cmd_FDMOVE2(	struct M68kStruct *ms );
void Cmd_FDMUL(		struct M68kStruct *ms );
void Cmd_FDMUL2(	struct M68kStruct *ms );
void Cmd_FDNEG(		struct M68kStruct *ms );
void Cmd_FDNEG2(	struct M68kStruct *ms );
void Cmd_FDSQRT(	struct M68kStruct *ms );
void Cmd_FDSQRT2(	struct M68kStruct *ms );
void Cmd_FDSUB(		struct M68kStruct *ms );
void Cmd_FDSUB2(	struct M68kStruct *ms );
void Cmd_FETOX(		struct M68kStruct *ms );
void Cmd_FETOX2(	struct M68kStruct *ms );
void Cmd_FETOXM1(	struct M68kStruct *ms );
void Cmd_FETOXM12(	struct M68kStruct *ms );
void Cmd_FGETEXP(	struct M68kStruct *ms );
void Cmd_FGETEXP2(	struct M68kStruct *ms );
void Cmd_FGETMAN(	struct M68kStruct *ms );
void Cmd_FGETMAN2(	struct M68kStruct *ms );
void Cmd_FINT(		struct M68kStruct *ms );
void Cmd_FINT2(		struct M68kStruct *ms );
void Cmd_FINTRZ(	struct M68kStruct *ms );
void Cmd_FINTRZ2(	struct M68kStruct *ms );
void Cmd_FLOG2(		struct M68kStruct *ms );
void Cmd_FLOG22(	struct M68kStruct *ms );
void Cmd_FLOG10(	struct M68kStruct *ms );
void Cmd_FLOG102(	struct M68kStruct *ms );
void Cmd_FLOGN(		struct M68kStruct *ms );
void Cmd_FLOGN2(	struct M68kStruct *ms );
void Cmd_FLOGNP1(	struct M68kStruct *ms );
void Cmd_FLOGNP12(	struct M68kStruct *ms );
void Cmd_FMOD(		struct M68kStruct *ms );
void Cmd_FMOD2(		struct M68kStruct *ms );
void Cmd_FMOVE(		struct M68kStruct *ms );
void Cmd_FMOVE2(	struct M68kStruct *ms );
void Cmd_FMOVE3(	struct M68kStruct *ms );
void Cmd_FMOVE4(	struct M68kStruct *ms );
void Cmd_FMOVECR(	struct M68kStruct *ms );
void Cmd_FMOVEM(	struct M68kStruct *ms );
void Cmd_FMOVEM2(	struct M68kStruct *ms );
void Cmd_FMUL(		struct M68kStruct *ms );
void Cmd_FMUL2(		struct M68kStruct *ms );
void Cmd_FNEG(		struct M68kStruct *ms );
void Cmd_FNEG2(		struct M68kStruct *ms );
void Cmd_FREM(		struct M68kStruct *ms );
void Cmd_FREM2(		struct M68kStruct *ms );
void Cmd_FSABS(		struct M68kStruct *ms );
void Cmd_FSABS2(	struct M68kStruct *ms );
void Cmd_FSADD(		struct M68kStruct *ms );
void Cmd_FSADD2(	struct M68kStruct *ms );
void Cmd_FSCALE(	struct M68kStruct *ms );
void Cmd_FSCALE2(	struct M68kStruct *ms );
void Cmd_FScc(		struct M68kStruct *ms );
void Cmd_FSDIV(		struct M68kStruct *ms );
void Cmd_FSDIV2(	struct M68kStruct *ms );
void Cmd_FSGLDIV(	struct M68kStruct *ms );
void Cmd_FSGLDIV2(	struct M68kStruct *ms );
void Cmd_FSGLMUL(	struct M68kStruct *ms );
void Cmd_FSGLMUL2(	struct M68kStruct *ms );
void Cmd_FSIN(		struct M68kStruct *ms );
void Cmd_FSIN2(		struct M68kStruct *ms );
void Cmd_FSINCOS(	struct M68kStruct *ms );
void Cmd_FSINCOS2(	struct M68kStruct *ms );
void Cmd_FSINH(		struct M68kStruct *ms );
void Cmd_FSINH2(	struct M68kStruct *ms );
void Cmd_FSMOVE(	struct M68kStruct *ms );
void Cmd_FSMOVE2(	struct M68kStruct *ms );
void Cmd_FSMUL(		struct M68kStruct *ms );
void Cmd_FSMUL2(	struct M68kStruct *ms );
void Cmd_FSNEG(		struct M68kStruct *ms );
void Cmd_FSNEG2(	struct M68kStruct *ms );
void Cmd_FSQRT(		struct M68kStruct *ms );
void Cmd_FSQRT2(	struct M68kStruct *ms );
void Cmd_FSSQRT(	struct M68kStruct *ms );
void Cmd_FSSQRT2(	struct M68kStruct *ms );
void Cmd_FSSUB(		struct M68kStruct *ms );
void Cmd_FSSUB2(	struct M68kStruct *ms );
void Cmd_FSUB(		struct M68kStruct *ms );
void Cmd_FSUB2(		struct M68kStruct *ms );
void Cmd_FTAN(		struct M68kStruct *ms );
void Cmd_FTAN2(		struct M68kStruct *ms );
void Cmd_FTANh(		struct M68kStruct *ms );
void Cmd_FTANh2(	struct M68kStruct *ms );
void Cmd_FTENTOX(	struct M68kStruct *ms );
void Cmd_FTENTOX2(	struct M68kStruct *ms );
void Cmd_FTST(		struct M68kStruct *ms );
void Cmd_FTST2(		struct M68kStruct *ms );
void Cmd_FTWOTOX(	struct M68kStruct *ms );
void Cmd_FTWOTOX2(	struct M68kStruct *ms );

#endif
