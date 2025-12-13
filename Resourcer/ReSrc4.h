
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

#ifndef RESOURCER_RESRC4_H
#define RESOURCER_RESRC4_H 1

// --

#define _GNU_SOURCE
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>

// --

#include "Resourcer/Types.h"

// --

#define DATE				"13-Dec-2025"
#define YEAR				2025
#define VERSION				2
#define REVISION			16

#define MAX_REF_HASH		29U			// Prime Nr .. bigger value more memory pr. HunkNode
#define MAX_LAB_HASH		83U			// Prime Nr .. Global Label Hash
#define MAX_LabelName		32U			// Max Label Name Length

// --

#ifdef BIGENDIAN

#define SWAP16(x)			x
#define SWAP32(x)			x

#else

#define SWAP16(x)			(((x) & 0x00ffU ) << 8 | \
							 ((x) & 0xff00U ) >> 8 )

#define SWAP32(x)			(((x) & 0x000000ffUL ) << 24 | \
							 ((x) & 0x0000ff00UL ) <<  8 | \
							 ((x) & 0x00ff0000UL ) >>  8 | \
							 ((x) & 0xff000000UL ) >> 24 )

#endif

// --
// -- ReSrc4

typedef struct _RS4Ref RS4Ref;
typedef struct _RS4Node	RS4Node;
typedef struct _RS4Trace RS4Trace;
typedef struct _RS4Label RS4Label;
typedef struct _RS4Brance RS4Brance;
typedef struct _RS4Header RS4Header;
typedef struct _RS4Source RS4Source;
typedef struct _RS4FileHeader RS4FileHeader;
typedef struct _RS4FileSection RS4FileSection;
typedef struct _RS4FileSectionInfo RS4FileSecInfo;

// --

enum GSSType
{
	GSSType_Unset = 0,		// Init (Error)
	GSSType_Unknown,		// 1
	GSSType_Reg,			// 2
	GSSType_Exec,			// 3
	GSSType_Label,			// 4
};

// --

enum
{
	ANSER_Ask,
	ANSER_Yes,
	ANSER_No,
};

// --

struct _RS4Node
{
	PTR 				Next;
	PTR 				Prev;
};

// --

struct _RS4Header
{
	PTR 				Head;
	PTR 				Tail;
	S32					Nodes;
};

// --

enum RS4ID // Just random number for IDs
{
	RS4ID_FileHeader	= 0xC2A84843,
	RS4ID_FileSection	= 0xBE3D6217,
	RS4ID_Label			= 0x712C7AA6,
	RS4ID_Brance		= 0x65D91DEF,
	RS4ID_Trace			= 0x5E1A0D16,
	RS4ID_Ref			= 0x8062A795,
};

// --

enum RS4FileType
{
	RS4FileType_Error,
	RS4FileType_Unknown,

	#ifdef SUPPORT_FHR
	RS4FileType_FHR,
	#endif

	#ifdef SUPPORT_HUNK
	RS4FileType_Hunk,
	#endif

	RS4FileType_Last
};

// --

enum RS4MemoryType
{
	RS4MT_Unset = 0,
	RS4MT_Data,
	RS4MT_Code,
	RS4MT_Last
};

// --

enum RS4SectionType
{
	RS4ST_Unset = 0,
	RS4ST_Code,
	RS4ST_Data,
	RS4ST_BSS,
	RS4ST_Last
};

// --

enum RS4ErrorCode
{
	RS4ErrStat_Okay,
	RS4ErrStat_Error,
	RS4ErrStat_OutOfMemory,					// Memory Alloc Failed
	RS4ErrStat_OutOfBounds,					// Value out of Bounds/Range
	RS4ErrStat_WrongType,					// Typing to Change Label type
	RS4ErrStat_FileIO,						// Error read/writing data (or other IO errors)
	RS4ErrStat_FileStructure,				// Error parsing Hunk/ELF file container
	RS4ErrStat_FileNotFound,
	RS4ErrStat_FileIsADir,
	RS4ErrStat_OpeningFile,
	RS4ErrStat_Internal,					// Internal Error
	RS4ErrStat_InvalidStructID,				// Internal Error
	RS4ErrStat_InvalidFileType,
	RS4ErrStat_InvalidLabelType,
	RS4ErrStat_UnsupportedFileType,
	RS4ErrStat_MD5Fail,
	RS4ErrStat_LineJunk,					// Error parsing Config file, unexpected data at end
	RS4ErrStat_ExpectedHexVal,				// Expected Hex value
	RS4ErrStat_NeedOneDigit,				// Need atleast one Digit, $ is not enough for a Hex value
	RS4ErrStat_MissingInputFile,			// Missing 
	RS4ErrStat_MissingOutputFile,
	RS4ErrStat_SameInOutFile,
	RS4ErrStat_Last
};

enum RS4FuncStat
{
	RS4FuncStat_Okay,
	RS4FuncStat_Error,
	RS4FuncStat_Last
};

enum RS4DecodeStat
{
	RS4DecodeStat_Okay,
	RS4DecodeStat_Error,
	RS4DecodeStat_UnknownCmd,
	RS4DecodeStat_Last
};

enum RS4JumpStat
{
	RS4JumpStat_Skip,				// Okay
	RS4JumpStat_Error,
	RS4JumpStat_Handled,
};

enum RS4LabelType
{
	RS4LabelType_Unset,				// Never Set
	RS4LabelType_Unknown,			// Two or more diffrent type.. not sure what it is 
	RS4LabelType_Code,
	RS4LabelType_String,
	RS4LabelType_Pointer,
	RS4LabelType_RelativeWord,
	RS4LabelType_Struct,
	RS4LabelType_Last
};

enum RS4LabelPtrType
{
	RS4LabelPtrType_Unset,			// Never Set
	RS4LabelPtrType_Unknown,		// Two or more diffrent type.. not sure what it is 
	RS4LabelPtrType_Library,
	RS4LabelPtrType_Struct,
};

enum RS4TracePass
{
	RS4TracePass_Unset,
	RS4TracePass_Trace,				// 1, First trace
	RS4TracePass_Label,				// 2, Adjust Labels
	RS4TracePass_Build,				// 3, Build Source
	RS4TracePass_Last
};

enum RS4RegType
{
	RRT_Unknown = 0,
	RRT_Label,
	RRT_Address,
	RRT_Library,
	RRT_Struct,
	RRT_Last,
};

enum RS4LabelSize
{
	RS4LABSIZE_Unset,		//
	RS4LABSIZE_Unknown,		// Diffrent sizes have been used on label
	RS4LABSIZE_Integer8,	// move.b
	RS4LABSIZE_Integer16,	// move.w
	RS4LABSIZE_Integer32,	// move.l
	RS4LABSIZE_Integer64,	// move.q
	RS4LABSIZE_Float32,		// move.s
	RS4LABSIZE_Float64,		// move.d
};

// --

enum RS4StructID
{
	RS4StructID_Unknown = 0,

	#ifdef SUPPORT_AMIGAOS3
	RS4StructID_IntuiText,		// 1
	RS4StructID_BitMap,			// 2
	RS4StructID_NewScreen,		// 3
	RS4StructID_Gadget,			// 4
	RS4StructID_TextAttr,		// 5
	RS4StructID_TextFont,		// 6
	RS4StructID_RastPort,		// 7
	RS4StructID_Rectangle,		// 8
	RS4StructID_LWGadData,		// 9
	RS4StructID_Interrupt,		// 10
	RS4StructID_MsgPort,		// 11
	RS4StructID_StackSwap,		// 12
	#endif

	RS4StructID_Last
};

// --

// --
// -- CPUs

#ifdef SUPPORT_M68K
#include "CPU_M68k/M68k.h"
#endif

// --
// -- Loaders

#ifdef SUPPORT_FHR
#include "Loader_FHR/FHR.h"
#endif

#ifdef SUPPORT_HUNK
#include "Loader_Hunk/Hunk.h"
#endif

// --
// -- Systems

#ifdef SUPPORT_AMIGAOS3
#include "System_AmigaOS3/Amiga.h"
#endif

// --
// -- ReSrc4 

struct _RS4FileSection
{
	RS4Node					rfs_Node;
	enum RS4ID				rfs_ID;

	S32						rfs_SecNr;
	enum RS4SectionType		rfs_SecType;
	RS4Header				rfs_SecLabels;
	RS4Header				rfs_SecRefs;
	RS4Ref *				rfs_SecRefs2[MAX_REF_HASH];

	S64						rfs_MemorySize;
	MEM 					rfs_MemoryType;
	S64						rfs_MemoryAdr;				// Virtual memory address
	MEM 					rfs_MemoryBuf;

	S64						rfs_DataSize;

	RS4FileHeader *			rfs_File;
	S64						rfs_FileOffset;

	#ifdef SUPPORT_HUNK
	struct HunkFileSection	rfs_HunkData;
	#endif
};

// --

struct _RS4FileSectionInfo
{
	RS4FileSection *		rsi_Section;
	S64						rsi_MemoryAdr;				// Virtual memory address
	S64						rsi_MemorySize;
};

// --

struct _RS4FileHeader
{
	RS4Node					rfh_Node;
	enum RS4ID				rfh_ID;

	CHR						rfh_FileMD5[34];			// Ascii
	STR 					rfh_FileName;
	enum RS4FileType		rfh_FileType;
	S64						rfh_FileSize;
	MEM 					rfh_FileBuffer;

	RS4Header				rfh_BranceList;
	RS4Header				rfh_SourceList;
	RS4Header				rfh_ExtLabelList;

	RS4FileSecInfo *		rfh_SecArray;
	S32						rfh_SecArraySize;
	S32						rfh_SecFirst;
	S64						rfh_StartAdr;

	RS4Label *				rfh_LabelHash[MAX_LAB_HASH];

	#ifdef SUPPORT_HUNK
	struct HunkFileHeader	rfh_HunkData;
	#endif
};

// -- 

struct _RS4Trace
{
	enum RS4ID				rt_ID;
	enum RS4TracePass		rt_Pass;
	RS4FileHeader *			rt_File;
	RS4FileSection *		rt_Section;
	S64						rt_CurMemAdr;
	MEM 					rt_CurMemBuf;
	MEM 					rt_CurMemType;
	enum RS4DecodeStat		(*rt_Decoder)( enum RS4ErrorCode *errcode, RS4Trace *rt );

	// -- File Containers

	union
	{
		#ifdef SUPPORT_HUNK
		struct HunkTrace		Hunk;
		#endif
	}							rt_Container;

	// -- CPUs

	union
	{
		#ifdef SUPPORT_M68K
		struct M68kTrace		M68k;
		#endif
	}							rt_CPU;

	// --
};

// --

struct _RS4Brance
{
	RS4Node					rb_Node;
	enum RS4ID				rb_ID;
	RS4FileSection *		rb_Section;
	S64						rb_Address;
	S64						rb_Offset;
	struct M68kRegister		rb_Registers[16];
};

// --


struct _RS4Label
{
	RS4Node					rl_Node;
	enum RS4ID				rl_ID;
	enum RS4LabelType		rl_Type1;
	S32						rl_Type2;
	S32						rl_Type3;
	enum RS4LabelSize		rl_Label_RW_Size;		// hmm : Byte/Word/Long/Float/Double
	S32						rl_UserLocked;
	RS4FileSection *		rl_Section;
	RS4Label *				rl_Parent;
	RS4Label *				rl_HashPtr;
	RS4Label *				rl_Ref;					// Jump Table Relative Address (Label)
	MEM 					rl_Memory;
	S64						rl_Offset;
	S64						rl_Address;				// Virtual 32bit memory address
	S64						rl_Size;				// ie Size of Jump Table or Struct (in bytes)
	CHR						rl_Name[ MAX_LabelName ];
	struct M68kRegister		rl_Registers[16];
};

// --

struct _RS4Ref
{
	RS4Node					rr_Node;
	enum RS4ID				rr_ID;
	RS4Ref *				rr_HashPtr;
	S64						rr_Address;
	S64						rr_Offset;
	S32						rr_Handled;				// Have we handled this Pointer Refs?
};

// --

enum RS4SourceType
{
	RS4SourceType_Unknown,
	RS4SourceType_Code,
	RS4SourceType_NewLine,
	RS4SourceType_String,
	RS4SourceType_Last,
};

struct _SrcCode
{
	S64						rs_Address;				// Debug info
	RS4Label *				rs_Label;
	CSTR 					rs_Opcode;
	STR 					rs_Argument;
	STR 					rs_Comment;
};

struct _SrcString
{
	STR 					rs_String;
};

struct _SrcNewLine
{
	S32						rs_Count;				// Number of Newlines
};

struct _RS4Source
{
	RS4Node					rs_Node;
	enum RS4SourceType		rs_Type;

	union
	{
		struct _SrcCode		rs_Code;
		struct _SrcNewLine	rs_NewLine;
		struct _SrcString	rs_String;
	
	}						rs_Data;
};

// --

enum DataStructType
{
	DST_Data,			// Unknown: dsn_Size holds Size
	DST_Pointer,		// Pointer: void Pointer
	DST_String,			// Pointer: NUL String (Calls Mark_NulString)
	DST_Struct,			// Pointer: Struct ID (Calls Mark_Struct)
	DST_Byte,			// Data...: 1 Byte
	DST_Word,			// Data...: 2 Byte
	DST_Long,			// Data...: 4 Byte
};

struct DataStructNode
{
	enum DataStructType		dsn_Type;
	S32						dsn_Size;		// Size
	STR 					dsn_Name;
	S32						dsn_ID;			// Struct ID
};

struct DataStructHeader
{
	S32						dsh_Size;		// Total size
	S32						dsh_Entries;
	CSTR 					dsh_Title;
	struct DataStructNode	dsh_Data[];
};


// --
// -- ReSrc4 Protos

// -- Brance
RS4Brance *			RS4AddBrance_File(		enum RS4ErrorCode *errcode, RS4FileHeader *fh, S64 addr );
enum RS4FuncStat	RS4FreeBrance(			enum RS4ErrorCode *errcode, RS4Brance *rb );

// -- Cmd Line
enum RS4FuncStat	RS4ParseArgs(			enum RS4ErrorCode *errcode, S32 argc, STR argv[] );
void				RS4PrintUsage(			void );

// -- Config
enum RS4FuncStat	RS4ParseConfig_File(	enum RS4ErrorCode *errcode, RS4FileHeader *fh );

// -- File Header
RS4FileHeader *		RS4LoadExe(				enum RS4ErrorCode *errcode, STR filename );
RS4FileHeader *		RS4LoadFile( 			enum RS4ErrorCode *errcode, STR filename );
void				RS4FreeFile(			enum RS4ErrorCode *errcode, RS4FileHeader *fh );

// -- File Section
RS4FileSection *	RS4AllocSection(		enum RS4ErrorCode *errcode, S32 secnr, S64 secsize );
void				RS4FreeSection(			enum RS4ErrorCode *errcode, RS4FileSection *sec );
RS4FileSection *	RS4FindSection_File(	RS4FileHeader *fh, S64 addr );

// -- Label
RS4Label *			RS4AddLabel_File(		enum RS4ErrorCode *errcode, RS4FileHeader *fh, S64 addr, enum RS4LabelType type, STR file );
RS4Label *			RS4AddLabel_Sec(		enum RS4ErrorCode *errcode, RS4FileSection *sec, S64 addr, enum RS4LabelType type );
enum RS4FuncStat	RS4FreeLabel(			enum RS4ErrorCode *errcode, RS4Label *rl );
RS4Label *			RS4FindLabel_File(		RS4FileHeader *fh, S64 addr, STR file );

// -- Label Magic
enum RS4FuncStat	RS4LabelMagic_File(		enum RS4ErrorCode *errcode, RS4FileHeader *fh );

// -- Misc
STR 				RS4Strdup(				STR string );
enum RS4FuncStat	RS4InitTrace(			enum RS4ErrorCode *errcode, RS4Trace *rt, RS4FileHeader *fh, enum RS4TracePass type );

// -- Nodes
void				RS4AddHead(				RS4Header *h, PTR new );
void				RS4AddTail(				RS4Header *h, PTR new );
void				RS4AddAfter( 			RS4Header *h, PTR cur, PTR new );
PTR 				RS4RemHead(				RS4Header *h );
PTR 				RS4GetHead(				RS4Header *h );
PTR 				RS4GetTail(				RS4Header *h );
PTR 				RS4GetNext(				PTR cur );
PTR 				RS4GetPrev(				PTR cur );

// -- Refs
RS4Ref *			RS4AddRef_Sec(			enum RS4ErrorCode *errcode, RS4FileSection *sec, S64 addr );
enum RS4FuncStat	RS4FreeRef(				enum RS4ErrorCode *errcode, RS4Ref *rr );
RS4Ref *			RS4FindRef_Sec(			RS4FileSection *sec, S64 addr );
RS4Ref *			RS4FirstReloc_Sec(		RS4FileSection *sec, S64 adr1, S64 adr2 );

// -- Sourc Build
enum RS4FuncStat	RS4BuildSource_File(	enum RS4ErrorCode *errcode, RS4FileHeader *fh );

// -- Source Save
enum RS4FuncStat	RS4SaveSource_File(		enum RS4ErrorCode *errcode, RS4FileHeader *fh, STR filename );
enum RS4FuncStat	RS4SaveWriteString(		enum RS4ErrorCode *errcode, STR buffer, U64 length );

// -- Trace
enum RS4FuncStat	RS4Trace_File(			enum RS4ErrorCode *errcode, RS4FileHeader *fh );
enum RS4FuncStat	RS4Trace_JumpTable(		enum RS4ErrorCode *errcode, RS4Trace *rt );

// -- hmm
enum RS4FuncStat	RS4BuildLabelString(	enum RS4ErrorCode *errcode, RS4Label *rl, STR buf );
enum RS4FuncStat	RS4BuildLabelString2(	enum RS4ErrorCode *errcode, RS4Trace *rt, STR buf, S64 adr, S64 val );
void				Mark_Code(				RS4Label *rl );
void				Mark_NulString(			RS4Label *rl );
enum RS4FuncStat	Mark_Struct(			enum RS4ErrorCode *errcode, RS4Label *rl, enum RS4StructID id, STR file );
STR 				FindFileName(			STR name );
RS4Label *			RS4AddExtLabel(			enum RS4ErrorCode *errcode, RS4FileHeader *fh, S64 addr );
enum RS4JumpStat	RS4JumpTable_Rel16(		enum RS4ErrorCode *errcode, RS4FileHeader *fh, S64 tabel_adr, S64 relative_adr, S32 entries );

#ifdef SUPPORT_M68K
void				M68k_ClearRegs(			RS4Trace *rt, S32 mask );
#endif

// --
// -- Extern 

extern struct DataStructHeader *DataStructTable[RS4StructID_Last];
extern CSTR ErrorStrings[RS4ErrStat_Last];
extern S64 RS4CurrentVirtAdr;
extern STR SaveLineBuffer;
extern STR ConfigFile;
extern STR OutputFile;
extern STR InputFile;
extern STR LabNames;
extern STR ExtNames;
extern STR SecNames;
extern S32 ShortOpcodes;
extern S32 OpcodeTabs;
extern S32 AutoAnser;
extern S32 DebugInfo;
extern S32 DoVerbose;
extern S32 LabTabs;
extern S32 ArgTabs;

#endif

// --
