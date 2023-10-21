
/*
 * Copyright (c) 2014-2023 by Rene W. Olsen < renewolsen @ gmail . com >
 * All rights reserved.
 *
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

#define DATE				"20-Oct-2023"
#define YEAR				2023
#define VERSION				2
#define REVISION			2

// --

#define MAX_REFHASH			29			// Prime Nr .. bigger value more memory pr. HunkNode
#define MAX_LABHASH			59			// Prime Nr .. Global Label Hash
#define MAX_LabelName		32

#define HUNK_CODE			0x000003E9
#define HUNK_DATA			0x000003EA
#define HUNK_BSS			0x000003EB
#define HUNK_RELOC32		0x000003EC
#define HUNK_SYMBOL			0x000003F0
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

enum LibType // Used by LabelType and RegType
{
	LIBT_DosBase,
	LIBT_DiskFontBase,
	LIBT_ExecBase,
	LIBT_GraphicsBase,
	LIBT_IntuitionBase,
};

enum LabelType
{
    LT_Unknown = 0,
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
	OS_Byte = 6,
	OS_Word,
	OS_Long,
};

enum ClearReg
{
	CR_No,				// Keep Regs
	CR_All,				// Clear D0-A7
	CR_Function,		// Clear D0-D1/A0-A1
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
	int						ms_TraceMode;
	enum ClearReg			ms_ClearRegs;
	int						ms_LibCall;				// Well Possible LibCall

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
	struct M68kRegister     ms_SrcRegister;
	struct M68kRegister     ms_DstRegister;
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
	int32_t					hl_Label_Size;
	char					hl_Label_Name[ MAX_LabelName ];
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

// -- Extern
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

// -- Label Magic
int					LabelMagic(				struct HunkStruct *hs );

// -- M68k
void				M68k_Decode(			struct M68kStruct *ms );
int					M68k_EffectiveAddress(	struct M68kStruct *ms, struct HunkRef *isRef, uint32_t RefType );
char *				M68k_FindLibFunc(		struct M68kStruct *ms, int16_t val );
char *				M68k_Lib_DiskFont(		struct M68kStruct *ms, int16_t val );
int					Save_Lib_DiskFont(		void );
char *				M68k_Lib_Dos(			struct M68kStruct *ms, int16_t val );
int					Save_Lib_Dos(			void );
char *				M68k_Lib_Exec(			struct M68kStruct *ms, int16_t val );
int					Save_Lib_Exec(			void );
char *				M68k_Lib_Graphics(		struct M68kStruct *ms, int16_t val );
int					Save_Lib_Graphics(		void );
char *				M68k_Lib_Intuition(		struct M68kStruct *ms, int16_t val );
int					Save_Lib_Intuition(		void );

// -- Nodes
void				myAddHead(				struct Header *h, void *new );
void				myAddTail(				struct Header *h, void *new );
void				myAddAfter( 			struct Header *h, void *cur, void *new );
void *				myRemHead(				struct Header *h );
void *				myGetHead(				struct Header *h );
void *				myGetTail(				struct Header *h );
void *				myGetNext(				void *cur );
void *				myGetPrev(				void *cur );

// -- ReSrc4
char *				myStrdup(				char *string );

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
void Cmd_CLR(		struct M68kStruct *ms );
void Cmd_CMP(		struct M68kStruct *ms );
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
void Cmd_SWAP(		struct M68kStruct *ms );
void Cmd_TRAP(		struct M68kStruct *ms );
void Cmd_TRAPV(		struct M68kStruct *ms );
void Cmd_TST(		struct M68kStruct *ms );
void Cmd_UNLK(		struct M68kStruct *ms );
void Cmd_UNPK(		struct M68kStruct *ms );

#endif
