
/*
** Copyright (c) 2025 by Rene W. Olsen
**
** SPDX-License-Identifier: GPL-3.0-or-later
**
** This software is released under the GNU General Public License, version 3.
** For the full text of the license, please visit:
** https://www.gnu.org/licenses/gpl-3.0.html
**
** You can also find a copy of the license in the LICENSE file included with this software.
**
** --
** Revision : v1.00 -- 20-Feb-2025 --
** Revision : v1.01 -- 12-Dec-2025 --
** Revision : v1.02 -- 14-Dec-2025 --
**
*/

// --

#ifndef LOADER_FHR_H
#define LOADER_FHR_H 1

#ifndef RESOURCER_RESRC4_H
#error ReSrc4.h must be included first
#endif

// --

#if defined(__GNUC__) || defined(__clang__)
    #define PACKED __attribute__((packed))
#elif defined(_MSC_VER)
    #pragma pack(push, 1)
    #define PACKED
#else
    #error "Unknown compiler — please define PACKED for this compiler"
#endif

// --

/*
** -- Section IDs -- 
**
** Note: All IDs are always in Big Endian format and 4 bytes
**
*/

#define FHR_HEADER				0x46485200		/* FHR\0 - File Start (Header) */
// -
#define FHR_COD0				0x434F4400		/* COD\0 - Compact format, for Exes */
#define FHR_COD1				0x434F4401		/* COD\1 - Fast format, for Objects */
// -
#define FHR_DAT0				0x44415400		/* DAT\0 - Compact format, for Exes */
#define FHR_DAT1				0x44415401		/* DAT\1 - Fast format, for Objects */
// -
#define FHR_BSS0				0x42535300		/* BSS\0 - Compact format, for Exes */
#define FHR_BSS1				0x42535301		/* BSS\1 - Fast format, for Objects */
// -
#define FHR_END0				0x454E4400		/* END\0 - Section End, for CODx, DATx, BSSx sections */
#define FHR_REL0				0x52454C00		/* REL\0 - Relocs */
#define FHR_SYM0				0x53594D00		/* SYM\0 - Symbol Names */

/* -- Reloc Types -- */

#define FHR_RELTYPE_RELOC		0				/* Exe + Obj : ABS 64 : Offset */
#define FHR_RELTYPE_DEF			1				/*       Obj : ABS 64 : Define   : Offset + NameIdx (Create a public label) */
#define FHR_RELTYPE_REF			2				/*       Obj : ABS 64 : External : Offset + NameIdx (Use a public label) */

// todo : FHR_RELTYPE_PC_x idea is to implement ( Offset, Rx )
#define FHR_RELTYPE_PC_32		3				/*       Obj : PC  32 : External : Offset + NameIdx (Use a public label) */
#define FHR_RELTYPE_PC_64		4				/*       Obj : PC  64 : External : Offset + NameIdx (Use a public label) */

/* -- File Header -- */

#define FHR_SIZE_32				0
#define FHR_SIZE_64				1

#define FHR_ENDIAN_Little		0
#define FHR_ENDIAN_Big			1

#define FHR_TYPE_Exe			0				/* Exe File */
#define FHR_TYPE_Object			1				/* Object File (vbcc) */

#define FHR_ENC_None			0				/* Unencrypted */

#define FHR_PACK_None			0				/* File is not packed */
#define FHR_PACK_ZLib			1				/* zlib */
#define FHR_PACK_ZX0			2				/* zx0 */

#define FHR_OS_RVM				0x0100
#define FHR_OS_AmigaOS3			0x0101
#define FHR_OS_AmigaOS4			0x0102

#define FHR_CPU_RVM				0x0100
#define FHR_CPU_M68k			0x0101
#define FHR_CPU_PowerPC			0x0102

/* Default Header */

struct PACKED FHR_Header
{
	U32		FHR_ID;				/* FHR_HEADER */
	U8		FHR_Size;			/* 0 = 32 bits, 1 = 64 bits */
	U8		FHR_Endian;			/* 0 = Little, 1 = Big */
	U8		FHR_Type;			/* 0 = Exe, 1 = Object (vbcc) */
	U8		FHR_Pack;			/* 0 = None */
	U16		FHR_Encryption;		/* 0 = None */
	U16		FHR_OSType;			/* OS Type */
	U16		FHR_CPUType;		/* CPU Type */
	U16		FHR_Sections;		/* Number of Sections */
};

/*
** Note: All header entries are always in Big Endian format.
**
** Note 2: FHR_Sections counts the number of (CODx/DATx/BSSx) sections, each
** ending with an END0 marker. SYM0 and REL0 sections are NOT counted here.
*/

/* -- Section Memory Flags -- */

#define SECFLAG_WRITE			1	/* Memory must be writable */
#define SECFLAG_READ			2	/* Memory must be readable */
#define SECFLAG_EXE				4	/* Memory must be executable */
#define SECFLAG_SHARED			8	/* Global/Public/Shared mem */

/* -- Section Header Info (for Exe Files) -- */

struct PACKED SEC_Header_32		// Header is set to FHR_SIZE_32
{
	U32		SEC_MemorySize;
	U16		SEC_MemoryAlign;
	U16		SEC_MemoryFlags;
};

struct PACKED SEC_Header_64		// Header is set to FHR_SIZE_64
{
	U64		SEC_MemorySize;		/* Memory allocated for Section */
	U16		SEC_MemoryAlign;	/* Minimum alignment for Section */
	U16		SEC_MemoryFlags;	/* Memory type requested */
};

/*
** Note: Sections do not store type information. Each section must be read to determine its ID.
*/
 
/* -- COD0 : Header -- */
/* Main Section, needs END marker */

struct PACKED COD0_Header_32	// Header is set to FHR_SIZE_32
{
	U32		COD0_ID;			/* FHR_COD0 */
	U32		COD0_Length;		/* Section length (excluding ID and Length) */
	U16		COD0_CRC;			/* CRC16 value for COD0_Data */
	U8		COD0_Data[1];		/* Minimum data size is 1 */
};

struct PACKED COD0_Header_64	// Header is set to FHR_SIZE_64
{
	U32		COD0_ID;			/* FHR_COD0 */
	U64		COD0_Length;		/* Section length (excluding ID and Length) */
	U16		COD0_CRC;			/* CRC16 value for COD0_Data */
	U8		COD0_Data[1];		/* Minimum data size is 1 */
};

/*
** Note: FHR_COD0 sections are only used for FHR_TYPE_Exe files.
**
** Note 2: FHR_COD0 is the compact format for executable files.
**
** Note 3: FHR_COD0 should automatically receive executable memory,
**          whereas FHR_DAT0 and FHR_BSS0 should not. Section Info
**          must set SECFLAG_EXE for COD0 and clear it for DAT0 and BSS0.
**          Mismatched flags and section types should be treated as errors.
**
** Note 4: COD0_Data includes both code and relocation data.
**
** Note 5: Minimum size for COD0_Data is 1 byte. You can have 0 Bytes for
**          code but you need atleast 1 Reloc Terminator which is 1 byte.
**
** Note 6: COD0_Length and COD0_CRC entries both follow file header 
**          FHR_Endian setting.
**
** Note 7: a FHR_COD0 section shall be terminated with a FHR_END0 section.
**
** Note 8: There can only be one 'active' CODx/DATx/BSSx at a time before 
**          the END0 marker.
*/

/* -- COD1 : Header -- */
/* Main Section, needs END marker */

struct PACKED COD1_Header_64	// Header is set to FHR_SIZE_64
{
	U32		COD1_ID;			/* FHR_COD1 */
	U64		COD1_Length;		/* Section length (excluding ID and Length) */
	U16		COD1_Align;
	U16		COD1_Flags;
	U16		COD1_NameIdx;
	U8		COD1_Data[1];		/* Data[ COD1_Length - 6 ] */
};

/* -- DAT0 : Header -- */
/* Main Section, needs END marker */

struct PACKED DAT0_Header_32	// Header is set to FHR_SIZE_32
{
	U32		DAT0_ID;			/* FHR_DAT0 */
	U32		DAT0_Length;		/* Section length (excluding ID and Length) */
	U16		DAT0_CRC;			/* CRC16 value for DAT0_Data */
	U8		DAT0_Data[1];		/* Minimum data size is 1 */
};

struct PACKED DAT0_Header_64	// Header is set to FHR_SIZE_64
{
	U32		DAT0_ID;			/* FHR_DAT0 */
	U64		DAT0_Length;		/* Section length (excluding ID and Length) */
	U16		DAT0_CRC;			/* CRC16 value for DAT0_Data */
	U8		DAT0_Data[1];		/* Minimum data size is 1 */
};

/*
** Note: FHR_DAT0 sections are only used for FHR_TYPE_Exe files.
**
** Note 2: FHR_DAT0 is the compact format for executable files.
**
** Note 3: FHR_DAT0 are just like FHR_COD0 except it shall have no
**          executable memory. Section Info must not have SECFLAG_EXE.
**
** Note 4: DAT0_Data includes both data and relocation data.
**
** Note 5: Minimum size for DAT0_Data is 1 byte. You can have 0 Bytes for
**          data but you need atleast 1 Reloc Terminator which is 1 bytes.
**
** Note 6: DAT0_Length and DAT0_CRC entries both follow file header 
**          FHR_Endian setting.
**
** Note 7: a FHR_DAT0 section shall be terminated with a FHR_END0 section.
**
** Note 8: There can only be one 'active' CODx/DATx/BSSx at a time before 
**          the END0 marker.
*/

/* -- DAT1 : Header -- */
/* Main Section, needs END marker */

struct PACKED DAT1_Header_64	// Header is set to FHR_SIZE_64
{
	U32		DAT1_ID;			/* FHR_COD1 */
	U64		DAT1_Length;		/* Section length (excluding ID and Length) */
	U16		DAT1_Align;
	U16		DAT1_Flags;
	U16		DAT1_NameIdx;
	U8		DAT1_Data[1];		/* Data[ DAT1_Length - 6 ] */
};

/* -- BSS0 : Header -- */
/* Main Section, needs END marker */

struct PACKED BSS0_Header_32	// Header is set to FHR_SIZE_32
{
	U32		BSS0_ID;			/* FHR_BSS0 */
};

struct PACKED BSS0_Header_64	// Header is set to FHR_SIZE_64
{
	U32		BSS0_ID;			/* FHR_BSS0 */
};

/*
** Note: FHR_BSS0 sections are only used for FHR_TYPE_Exe files.
**
** Note 2: FHR_BSS0 is the compact format for executable files.
**
** Note 3: FHR_BSS0 holds no data, system allocate memory according
**          to Section info. System must make sure its memory zeroed
**          and it shall be none executable memory. 
**          Section Info must not have SECFLAG_EXE.
**
** Note 4: a FHR_BSS0 section shall be terminated with a FHR_END0 section.
**
** Note 5: All Sections have ID+Length infront, but there are two execptions
**          FHR_BSS0 and FHR_END0 only have an ID. Simply to save diskspace.
**
** Note 6: There can only be one 'active' CODx/DATx/BSSx at a time before 
**          the END0 marker.
*/

/* -- BSS1 : Header -- */
/* Main Section, needs END marker */

struct PACKED BSS1_Header_64	// Header is set to FHR_SIZE_64
{
	U32		BSS1_ID;			/* FHR_COD1 */
	U64		BSS1_Length;		/* Section length (excluding ID and Length) */
	U64		BSS1_Size;			/* Mem Size */
	U16		BSS1_Align;
	U16		BSS1_Flags;
	U16		BSS1_NameIdx;
};

/* -- END0 : Header -- */
/* Main Section terminator */

struct PACKED END0_Header_32	// Header is set to FHR_SIZE_32
{
	U32		END0_ID;			/* FHR_END0 */
};

struct PACKED END0_Header_64	// Header is set to FHR_SIZE_64
{
	U32		END0_ID;			/* FHR_END0 */
};

/*
** Note: FHR_END0 sections are for all file types.
**
** Note 2: File header tell you how many sections you need to parse.
**          CODx, DATx and BSSx starts section parsing, and END0 
**          ends it again.
**
** Note 3: Getting an END0 without any CODx/DATx/BSSx first, is an
**          error.
**
** Note 4: Getting two END0 or more in a row is an error.
**
** Note 5: It may be tempting to exclude the END0 marker in the
**          last section of the file, but that is also an error.
**
** Note 6: The end marker is not build into CODx/DATx/BSSx sections
**          as that makes it possible to add extra info in the 
**          future. Just place it between start and END0 marker.
*/

/* -- SYM0 : Header -- */

struct PACKED SYM0_Header_32	// Header is set to FHR_SIZE_32
{
	U32		SYM0_ID;			/* FHR_SYM0 */
	U32		SYM0_Length;		/* Section length (excluding ID and Length) */
//	RELOC	SYM0_Strings;		/* Number of Strings (U64 Relocs Value) */

// -Loop [ SYM0_Strings Times ]
//	RELOC	SYM0_NameLen		/* Include NUL in name length */
//	U8		SYM0_Name[ SYM0_NameLen ]

};

struct PACKED SYM0_Header_64	// Header is set to FHR_SIZE_64
{
	U32		SYM0_ID;			/* FHR_SYM0 */
	U64		SYM0_Length;		/* Section length (excluding ID and Length) */
//	RELOC	SYM0_Strings;		/* Number of Strings (U64 Relocs Value) */

// -Loop [ SYM0_Strings Times ]
//	RELOC	SYM0_NameLen		/* Include NUL in name length */
//	U8		SYM0_Name[ SYM0_NameLen ]

};

/* -- CRC16 : Function -- */
/* CRC-16-ANSI (IBM) polynomial */

// -- C Version --
// U16 rvm_Calc_CRC16( U8 *data, U64 len )
// {
// U16 crc = 0xffff;
//
//     while( len-- )
//     {
//         crc ^= ( *data++ << 8 );
//
//         for( int cnt=0 ; cnt<8 ; cnt++ )
//         {
//             if ( crc & 0x8000 )
//             {
//                 crc = crc << 1;
//                 crc = crc ^ 0x8005;
//             }
//             else
//             {
//                 crc = crc << 1;
//             }
//         }
//     }
//
//     return( crc );
// }

// -- Asm Version --
// ;
// ; Input:
// ;
// ;  r2.q = Buffer	(U8 *)
// ;  r3.q = Length	(U64) (unsigned)
// ;
// ; Output:
// ;
// ;  r0.w = CRC
// ;
//
// rvm_Calc_CRC16: 
//     pushf       r2-r4
//
//     vmove.w     #-1,r0
//
//     rtst.q      r3
//     beq         .9
// .1:
//     move.b      (r2)+,r4
//     vlsl.w      #8,r4,r4
//     reor.w      r4,r0,r0
//
//     vmove.q     #8,r4
// .2:
//     vlsl.w      #1,r0,r0
//     bcc         .3
//     eor.w       #$8005,r0
// .3:
//     vsub.q      #1,r4,r4
//     bne         .2
//
//     vsub.q      #1,r3,r3
//     bne         .1
// .9:
//     popf
//     rts

/* -- Reloc : Function -- */

// -- C Version --
// U64 rvm_Read_Reloc( U8 *data )
// {
// U64 val;
//
//     int c = *data++;
//
//     if ( c < 0x80 )
//     {
//         val = c;
//     }
//     else
//     {
//         val = 0;
//
//         for( int mask=1 ; mask<256 ; mask << 1 )
//         {
//             val = val << 8;
//
//             if ( c & mask )
//             {
//                 val += *data++;
//             }
//         }
//     }
//
//     return( val );
// }

// -- Asm Version --
// ;
// ; Input:
// ;
// ;  r2.q = Buffer	(U8 *)
// ;
// ; Output:
// ;
// ;  r0.q = Value (U64)
// ;
//
// rvm_Read_Reloc:
//     pushf       r1-r3
// 
//     exts.b      (r2)+,r0
//     bpl         .3
//
//     rmove.q     r0,r1
//     rclr.q      r0
//     rclr.q      r3
// .1:
//     vlsl.q      #8,r0,r0
//
//     rbtst.q     r3,r1
//     beq         .2
//     move.b      (r2)+,r0
// .2:
//     vadd.q      #1,r3,r3
//     vcmp.q      #8,r3
//     bne         .1
// .3:
//     popf
//     rts

/* -- */

enum RS4FuncStat FHR_ParseFile(			enum RS4ErrorCode *errcode, RS4FileHeader *fh );
enum RS4FuncStat FHR_ParseFile_32(		enum RS4ErrorCode *errcode, RS4FileHeader *fh, struct FHR_Header *h );

/* -- */

#if defined(_MSC_VER)
    #pragma pack(pop)
#endif

#endif
