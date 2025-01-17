
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


// -- Overview

#if 0

-- Table #1 - Lightwave 5.0
	0x0c,0x80,0,0,0,0,		// Cmpi.l		#$00000013,D0			; 0c80 0000 0013
	0x6c,0x00,0,0,			// Bge.w		L005A					; 6c00 36e6
	0x30,0x3b,0x02,0x06,	// Move.w		L0058(PC,D0.w*2),D0		; 303b 0206
	0x4e,0xfb,0x00,0x04		// Jmp			L0059(PC,D0.w)			; 4efb 0004

-- Table #2 - Lightwave 5.0
	0x0c,0x80,0,0,0,0,		// Cmpi.l		#$00000013,D0			; 0c80 0000 0013
	0x64,0x00,0,0,			// Bcc.w		L005A					; 6400 0598
	0x30,0x3b,0x02,0x06,	// Move.w		L0058(PC,D0.w*2),D0		; 303b 0206
	0x4e,0xfb,0x00,0x04		// Jmp			L0059(PC,D0.w)			; 4efb 0004

-- Table #3 - Lightwave 5.0
	0x0c,0x80,0,0,0,0,		// Cmpi.l		#$00000013,D0			; 0c80 0000 0013
	0x64,0x00,				// Bcc.b		L005A					; 645e 
	0x30,0x3b,0x02,0x06,	// Move.w		L0058(PC,D0.w*2),D0		; 303b 0206
	0x4e,0xfb,0x00,0x04		// Jmp			L0059(PC,D0.w)			; 4efb 0004

-- Table #4 - Civilization ECS+AGA
	0x0c,0x40,0,0,			// Cmpi.w		#$0005,D0				; 0c40 0005
	0x64,0x00,0,0,			// Bcc.w		L02F6					; 6400 0400
	0xd0,0x40,				// Add.w		D0,D0					; d040
	0x30,0x3b,0x00,0x06,	// Move.w		L02F4(PC,D0.w),D0		; 303b 0006
	0x4e,0xfb,0x00,0x04		// Jmp			L02F5(PC,D0.w)			; 4efb 0004

-- Table #5 - Civilization ECS+AGA
	0x02,0x41,0,0,			// Andi.w		#$000f,D1				; 0241 000f
	0xd2,0x01,				// Add.b		D1,D1					; d201
	0x32,0x3b,0x10,0x06,	// Move.w		L253B(PC,D1.w),D1		; 323b 1006
	0x4e,0xfb,0x10,0x22		// Jmp			L253C(PC,D1.w)			; 4efb 1022

-- Table #6 - Civilization ECS
	0x0c,0x40,0,0,			// Cmpi.w		#$0005,D0				; 0c40 0005
	0x64,0,					// Bcc.b		L1163					; 6436
	0xd0,0x40,				// Add.w		D0,D0					; d040
	0x30,0x3b,0x00,0x06,	// Move.w		L1161(PC,D0.w),D0		; 303b 0006
	0x4e,0xfb,0x00,0x04		// Jmp			L1162(PC,D0.w)			; 4efb 0004

-- Table #7 - Civilization ECS+AGA
	0x0c,0x40,0,0,			// Cmpi.w		#$0008,D0				; 0c40 0008
	0x6c,0x00,0,0,			// Bge.w		L19E0					; 6c00 0092
	0xd0,0x40,				// Add.w		D0,D0					; d040
	0x30,0x3b,0x00,0x06,	// Move.w		L19DC(PC,D0.w),D0		; 303b 0006
	0x4e,0xfb,0x00,0x04		// Jmp			L19DD(PC,D0.w)			; 4efb 0004

-- Table #8 - Civilization AGA
	0x0c,0x40,0,0,			// Cmpi.w		#$0009,D0				; 0c40 0009
	0x6c,0,					// Bge.b		L19F0					; 6c6e 
	0xd0,0x40,				// Add.w		D0,D0					; d040
	0x30,0x3b,0x00,0x06,	// Move.w		L19EE(PC,D0.w),D0		; 303b 0006
	0x4e,0xfb,0x00,0x04		// Jmp			L19EF(PC,D0.w)			; 4efb 0004

-- Table #9 - Civilization AGA
	0x0c,0x40,0,0,			// Cmpi.w		#$0009,D0				; 0c40 0005
	0x64,0x00,0,0,			// Bcc.w		L06C6					; 6400 00f8
	0x30,0x3b,0x02,0x06,	// Move.w		L06C4(PC,D0.w*2),D0		; 303b 0206
	0x4e,0xfb,0x00,0x04		// Jmp			L19EF(PC,D0.w)			; 4efb 0004

-- Table #10 - Civilization AGA
	0x0c,0x40,0,0,			// Cmpi.w		#$0005,D0				; 0c40 0005
	0x64,0,					// Bcc.b		L0FC4					; 6434
	0x30,0x3b,0x02,0x06,	// Move.w		L0FC2(PC,D0.w*2),D0		; 303b 0206
	0x4e,0xfb,0x00,0x04		// Jmp			L19EF(PC,D0.w)			; 4efb 0004

-- Table #11 - Lightwave 5.0
	0x0c,0x80,0,0,0,0,		// Cmpi.l		#$00000011,D0			; 0c80 0000 0011
	0x6c,0,					// Bge.b		L33AD					; 6c5c
	0x30,0x3b,0x02,0x06,	// Move.w		L33AA(PC,D0.w*2),D0		; 303b 0206
	0x4e,0xfb,0x00,0x04		// Jmp			L33AB(PC,D0.w)			; 4efb 0004

-- Table #12 - Lightwave 5.0
	0x0c,0x81,0,0,0,0,		// Cmpi.l		#$00000034,D1			; 0c81 0000 0034
	0x6c,0,0,0,				// Bge.w		L33C3					; 6c00 0242
	0x32,0x3b,0x12,0x06,	// Move.w		L33C1(PC,D1.w*2),D1		; 323b 1206
	0x4e,0xfb,0x10,0x04		// Jmp			L33C2(PC,D1.w)			; 4efb 1004

-- Table #13 - Lightwave 5.0
	0x0c,0x80,0,0,0,0,		// Cmpi.l		#$00000005,D0			; 0c80 0000 0005
	0x6c,0,					// Bge.b		L35A1					; 6c00
	0xd0,0x40,				// Add.w		D0,D0					; d040
	0x30,0x3b,0x00,0x06,	// Move.w		(L359F,PC,D0.w),D0		; 303b 0006
	0x4e,0xfb,0x00,0x04		// Jmp			(L35A9,PC,D0.w)			; 4efb 1004

-- Table #14 - Lightwave 5.0
	0xb0,0xbc,0,0,0,0,		// Cmp.l		#$00000013,D0			; 0c80 0000 0013
	0x6c,0x00,0,0,			// Bge.w		L005A					; 6c00 36e6
	0x30,0x3b,0x02,0x06,	// Move.w		L0058(PC,D0.w*2),D0		; 303b 0206
	0x4e,0xfb,0x00,0x04		// Jmp			L0059(PC,D0.w)			; 4efb 0004

-- Table #15 - Lightwave 5.0
	0xb0,0xbc,0,0,0,0,		// Cmp.l		#$00000013,D0			; b0bc 0000 0013
	0x64,0x00,0,0,			// Bcc.w		L005A					; 6400 0598
	0x30,0x3b,0x02,0x06,	// Move.w		L0058(PC,D0.w*2),D0		; 303b 0206
	0x4e,0xfb,0x00,0x04		// Jmp			L0059(PC,D0.w)			; 4efb 0004

-- Table #16 - Lightwave 5.0
	0xb0,0xbc,0,0,0,0,		// Cmp.l		#$00000013,D0			; b0bc 0000 0013
	0x64,0x00,				// Bcc.b		L005A					; 645e 
	0x30,0x3b,0x02,0x06,	// Move.w		L0058(PC,D0.w*2),D0		; 303b 0206
	0x4e,0xfb,0x00,0x04		// Jmp			L0059(PC,D0.w)			; 4efb 0004

-- Table #17 - Lightwave 5.0
	0xb0,0xbc,0,0,0,0,		// Cmp.l		#$00000011,D0			; b0bc 0000 0011
	0x6c,0,					// Bge.b		L33AD					; 6c5c
	0x30,0x3b,0x02,0x06,	// Move.w		L33AA(PC,D0.w*2),D0		; 303b 0206
	0x4e,0xfb,0x00,0x04		// Jmp			L33AB(PC,D0.w)			; 4efb 0004

-- Table #18 - Lightwave 5.0
	0xb0,0xbc,0,0,0,0,		// Cmp.l		#$00000005,D0			; b0bc 0000 0005
	0x6c,0,					// Bge.b		L35A1					; 6c00
	0xd0,0x40,				// Add.w		D0,D0					; d040
	0x30,0x3b,0x00,0x06,	// Move.w		(L359F,PC,D0.w),D0		; 303b 0006
	0x4e,0xfb,0x00,0x04		// Jmp			(L35A9,PC,D0.w)			; 4efb 1004

-- Table #19 - Lightwave 5.0
	0xb2,0xbc,0,0,0,0,		// Cmp.l		#$00000034,D1			; b2bc 0000 0034
	0x6c,0,0,0,				// Bge.w		L33C3					; 6c00 0242
	0x32,0x3b,0x12,0x06,	// Move.w		L33C1(PC,D1.w*2),D1		; 323b 1206
	0x4e,0xfb,0x10,0x04		// Jmp			L33C2(PC,D1.w)			; 4efb 1004

-- Table #20 - Civilization ECS+AGA
	0xb0,0x7c,0,0,			// Cmp.w		#$0005,D0				; b07c 0005
	0x64,0x00,0,0,			// Bcc.w		L02F6					; 6400 0400
	0xd0,0x40,				// Add.w		D0,D0					; d040
	0x30,0x3b,0x00,0x06,	// Move.w		L02F4(PC,D0.w),D0		; 303b 0006
	0x4e,0xfb,0x00,0x04		// Jmp			L02F5(PC,D0.w)			; 4efb 0004

-- Table #21 - Civilization ECS
	0xb0,0x7c,0,0,			// Cmp.w		#$0005,D0				; b07c 0005
	0x64,0,					// Bcc.b		L1163					; 6436
	0xd0,0x40,				// Add.w		D0,D0					; d040
	0x30,0x3b,0x00,0x06,	// Move.w		L1161(PC,D0.w),D0		; 303b 0006
	0x4e,0xfb,0x00,0x04		// Jmp			L1162(PC,D0.w)			; 4efb 0004

-- Table #22 - Civilization ECS+AGA
	0xb0,0x7c,0,0,			// Cmp.w		#$0008,D0				; b07c 0008
	0x6c,0x00,0,0,			// Bge.w		L19E0					; 6c00 0092
	0xd0,0x40,				// Add.w		D0,D0					; d040
	0x30,0x3b,0x00,0x06,	// Move.w		L19DC(PC,D0.w),D0		; 303b 0006
	0x4e,0xfb,0x00,0x04		// Jmp			L19DD(PC,D0.w)			; 4efb 0004

-- Table #23 - Civilization AGA
	0xb0,0x7c,0,0,			// Cmp.w		#$0009,D0				; b07c 0009
	0x6c,0,					// Bge.b		L19F0					; 6c6e 
	0xd0,0x40,				// Add.w		D0,D0					; d040
	0x30,0x3b,0x00,0x06,	// Move.w		L19EE(PC,D0.w),D0		; 303b 0006
	0x4e,0xfb,0x00,0x04		// Jmp			L19EF(PC,D0.w)			; 4efb 0004

-- Table #24 - Civilization AGA
	0xb0,0x7c,0,0,			// Cmp.w		#$0009,D0				; b07c 0005
	0x64,0x00,0,0,			// Bcc.w		L06C6					; 6400 00f8
	0x30,0x3b,0x02,0x06,	// Move.w		L06C4(PC,D0.w*2),D0		; 303b 0206
	0x4e,0xfb,0x00,0x04		// Jmp			L19EF(PC,D0.w)			; 4efb 0004

-- Table #25 - Civilization AGA
	0xb0,0x7c,0,0,			// Cmp.w		#$0005,D0				; b07c 0005
	0x64,0,					// Bcc.b		L0FC4					; 6434
	0x30,0x3b,0x02,0x06,	// Move.w		L0FC2(PC,D0.w*2),D0		; 303b 0206
	0x4e,0xfb,0x00,0x04		// Jmp			L19EF(PC,D0.w)			; 4efb 0004

#endif

// --

enum EntryType
{
	ET_Fixed,
	ET_Pos16,
	ET_Pos32,
};

struct JTable
{
	int			TabelPos;

	int			RelativePos;

	int			EntryPos;
	int			EntryType;

	int			DataPos;
	int			DataSize;

	uint8_t		Data[32];
	uint8_t		Mask[32];
};

// --

enum RS4JumpStat RS4JumpTable_Rel16( enum RS4ErrorCode *errcode, RS4FileHeader *fh, int64_t tabel_adr, int64_t relative_adr, int32_t entries )
{
enum RS4ErrorCode ec;
enum RS4JumpStat js;
RS4FileSection *sec;
RS4Brance *rb;
RS4Label *rel;
RS4Label *rl;
int16_t *offsets;
uint8_t *type;
int64_t adr;
int32_t off;
int16_t r16;
int cnt;

	ec	= RS4ErrStat_Error;
	js	= RS4JumpStat_Error;

	// --

	sec = RS4FindSection_File( fh, tabel_adr );

	if ( ! sec )
	{
		#ifdef DEBUG
		printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
		#endif
		goto bailout;
	}

	// --

	rl = RS4AddLabel_Sec( & ec, sec, tabel_adr, RS4LabelType_RelativeWord );

	if ( ! rl )
	{
		#ifdef DEBUG
		printf( "%s:%04d: Error adding label at address $%08" PRIx64 "\n", __FILE__, __LINE__, tabel_adr );
		#endif
		goto bailout;
	}

	// --

	rel = RS4AddLabel_Sec( & ec, sec, relative_adr, RS4LabelType_Unset );

	if ( ! rel )
	{
		#ifdef DEBUG
		printf( "%s:%04d: Error adding label at address $%08" PRIx64 "\n", __FILE__, __LINE__, relative_adr );
		#endif
		goto bailout;
	}

	// --

	rl->rl_Type1	= RS4LabelType_RelativeWord;
	rl->rl_Size		= entries * 2;
	rl->rl_Ref		= rel;

	// --

	type	= sec->rfs_MemoryType;
	off		= tabel_adr - sec->rfs_MemoryAdr;

	memset( & type[ off ], RS4MT_Data, entries * 2 );

	// --

	offsets = (void *) & sec->rfs_MemoryBuf[ off ];

	for( cnt=0 ; cnt<entries ; cnt++ )
	{
		r16 = SWAP16( offsets[cnt] );
		adr = relative_adr + r16;

		rl = RS4AddLabel_Sec( & ec, sec, adr, RS4LabelType_Code );

		if ( ! rl )
		{
			#ifdef DEBUG
			printf( "%s:%04d: Error adding label at $%08" PRIx64 "\n", __FILE__, __LINE__, adr );
			#endif
			goto bailout;
		}

		rb = RS4AddBrance_File( & ec, fh, adr );

		if ( ! rb )
		{
			#ifdef DEBUG
			printf( "%s:%04d: Error adding brance at $%08" PRIx64 "\n", __FILE__, __LINE__, adr );
			#endif
			goto bailout;
		}
	}

	// --

	js = RS4JumpStat_Handled;
	ec = RS4ErrStat_Okay;

	// --

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( js );
}

// --

static struct JTable Table1 =
{
	+4,			// Tabel Pos

	+6,			// Relative Pos

	-12,		// EntryPos
	ET_Pos32,	// EntryType

	-14,		// Data Pos
	18,			// Data Size
{
	0x0c,0x80,0,0,0,0,		// Cmpi.l		#$00000013,D0			; 0c80 0000 0013
	0x6c,0x00,0,0,			// Bge.w		L005A					; 6c00 36e6
	0x30,0x3b,0x02,0x06,	// Move.w		L0058(PC,D0.w*2),D0		; 303b 0206
	0x4e,0xfb,0x00,0x04		// Jmp			L0059(PC,D0.w)			; 4efb 0004
},
{
	0xff,0xff,0x00,0x00,0x00,0x00,
	0xff,0xff,0x00,0x00,
	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff
},
};

// --

static struct JTable Table2 =
{
	+4,			// Tabel Pos

	+6,			// Relative Pos

	-12,		// EntryPos
	ET_Pos32,	// EntryType

	-14,		// Data Pos
	18,			// Data Size
{
	0x0c,0x80,0,0,0,0,		// Cmpi.l		#$00000013,D0			; 0c80 0000 0013
	0x64,0x00,0,0,			// Bcc.w		L005A					; 6400 0598
	0x30,0x3b,0x02,0x06,	// Move.w		L0058(PC,D0.w*2),D0		; 303b 0206
	0x4e,0xfb,0x00,0x04		// Jmp			L0059(PC,D0.w)			; 4efb 0004
},
{
	0xff,0xff,0x00,0x00,0x00,0x00,
	0xff,0xff,0x00,0x00,
	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff
},
};

// --

static struct JTable Table3 =
{
	+4,			// Tabel Pos

	+6,			// Relative Pos

	-10,		// EntryPos
	ET_Pos32,	// EntryType

	-12,		// Data Pos
	16,			// Data Size
{
	0x0c,0x80,0,0,0,0,		// Cmpi.l		#$00000013,D0			; 0c80 0000 0013
	0x64,0x00,				// Bcc.b		L005A					; 645e 
	0x30,0x3b,0x02,0x06,	// Move.w		L0058(PC,D0.w*2),D0		; 303b 0206
	0x4e,0xfb,0x00,0x04		// Jmp			L0059(PC,D0.w)			; 4efb 0004
},
{
	0xff,0xff,0x00,0x00,0x00,0x00,
	0xff,0x00,
	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff
},
};

// --

static struct JTable Table4 =
{
	+4,			// Tabel Pos

	+6,			// Relative Pos

	-12,		// EntryPos
	ET_Pos16,	// EntryType

	-14,		// Data Pos
	18,			// Data Size
{
	0x0c,0x40,0,0,			// Cmpi.w		#$0005,D0				; 0c40 0005
	0x64,0x00,0,0,			// Bcc.w		L02F6					; 6400 0400
	0xd0,0x40,				// Add.w		D0,D0					; d040
	0x30,0x3b,0x00,0x06,	// Move.w		L02F4(PC,D0.w),D0		; 303b 0006
	0x4e,0xfb,0x00,0x04		// Jmp			L02F5(PC,D0.w)			; 4efb 0004
},
{
	0xff,0xff,0x00,0x00,
	0xff,0xff,0x00,0x00,
	0xff,0xff,
	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff
},
};

// --

static struct JTable Table5 =
{
	+4,			// Tabel Pos

	+36,		// Relative Pos

	16,			// EntryPos
	ET_Fixed,	// EntryType

	-10,		// Data Pos
	14,			// Data Size
{
	0x02,0x41,0x00,0x0f,	// Andi.w		#$000f,D1				; 0241 000f
	0xd2,0x01,				// Add.b		D1,D1					; d201
	0x32,0x3b,0x10,0x06,	// Move.w		L253B(PC,D1.w),D1		; 323b 1006
	0x4e,0xfb,0x10,0x22		// Jmp			L253C(PC,D1.w)			; 4efb 1022
},
{
	0xff,0xff,0xff,0xff,
	0xff,0xff,
	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff
},
};

// --

static struct JTable Table6 =
{
	+4,			// Tabel Pos

	+6,			// Relative Pos

	-10,		// EntryPos
	ET_Pos16,	// EntryType

	-12,		// Data Pos
	16,			// Data Size
{
	0x0c,0x40,0,0,			// Cmpi.w		#$0005,D0				; 0c40 0005
	0x64,0,					// Bcc.b		L1163					; 6436
	0xd0,0x40,				// Add.w		D0,D0					; d040
	0x30,0x3b,0x00,0x06,	// Move.w		L1161(PC,D0.w),D0		; 303b 0006
	0x4e,0xfb,0x00,0x04		// Jmp			L1162(PC,D0.w)			; 4efb 0004
},
{
	0xff,0xff,0x00,0x00,
	0xff,0,
	0xff,0xff,
	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff
},
};

// --

static struct JTable Table7 =
{
	+4,			// Tabel Pos

	+6,			// Relative Pos

	-12,		// EntryPos
	ET_Pos16,	// EntryType

	-14,		// Data Pos
	18,			// Data Size
{
	0x0c,0x40,0,0,			// Cmpi.w		#$0008,D0				; 0c40 0008
	0x6c,0x00,0,0,			// Bge.w		L19E0					; 6c00 0092
	0xd0,0x40,				// Add.w		D0,D0					; d040
	0x30,0x3b,0x00,0x06,	// Move.w		L19DC(PC,D0.w),D0		; 303b 0006
	0x4e,0xfb,0x00,0x04		// Jmp			L19DD(PC,D0.w)			; 4efb 0004
},
{
	0xff,0xff,0x00,0x00,
	0xff,0xff,0x00,0x00,
	0xff,0xff,
	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff
},
};

// --

static struct JTable Table8 =
{
	+4,			// Tabel Pos

	+6,			// Relative Pos

	-10,		// EntryPos
	ET_Pos16,	// EntryType

	-12,		// Data Pos
	16,			// Data Size
{
	0x0c,0x40,0,0,			// Cmpi.w		#$0009,D0				; 0c40 0009
	0x6c,0,					// Bge.b		L19F0					; 6c6e 
	0xd0,0x40,				// Add.w		D0,D0					; d040
	0x30,0x3b,0x00,0x06,	// Move.w		L19EE(PC,D0.w),D0		; 303b 0006
	0x4e,0xfb,0x00,0x04		// Jmp			L19EF(PC,D0.w)			; 4efb 0004
},
{
	0xff,0xff,0x00,0x00,
	0xff,0x00,
	0xff,0xff,
	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff
},
};

// --

static struct JTable Table9 =
{
	+4,			// Tabel Pos

	+6,			// Relative Pos

	-10,		// EntryPos
	ET_Pos16,	// EntryType

	-12,		// Data Pos
	16,			// Data Size
{
	0x0c,0x40,0,0,			// Cmpi.w		#$0009,D0				; 0c40 0005
	0x64,0x00,0,0,			// Bcc.w		L06C6					; 6400 00f8
	0x30,0x3b,0x02,0x06,	// Move.w		L06C4(PC,D0.w*2),D0		; 303b 0206
	0x4e,0xfb,0x00,0x04		// Jmp			L19EF(PC,D0.w)			; 4efb 0004
},
{
	0xff,0xff,0x00,0x00,
	0xff,0xff,0x00,0x00,
	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff
},
};

// --

static struct JTable Table10 =
{
	+4,			// Tabel Pos

	+6,			// Relative Pos

	-8,			// EntryPos
	ET_Pos16,	// EntryType

	-10,		// Data Pos
	14,			// Data Size
{
	0x0c,0x40,0,0,			// Cmpi.w		#$0005,D0				; 0c40 0005
	0x64,0,					// Bcc.b		L0FC4					; 6434
	0x30,0x3b,0x02,0x06,	// Move.w		L0FC2(PC,D0.w*2),D0		; 303b 0206
	0x4e,0xfb,0x00,0x04		// Jmp			L19EF(PC,D0.w)			; 4efb 0004
},
{
	0xff,0xff,0x00,0x00,
	0xff,0x00,
	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff
},
};

// --

static struct JTable Table11 =
{
	+4,			// Tabel Pos

	+6,			// Relative Pos

	-10,		// EntryPos
	ET_Pos32,	// EntryType

	-12,		// Data Pos
	16,			// Data Size
{
	0x0c,0x80,0,0,0,0,		// Cmpi.l		#$00000011,D0			; 0c80 0000 0011
	0x6c,0,					// Bge.b		L33AD					; 6c5c
	0x30,0x3b,0x02,0x06,	// Move.w		L33AA(PC,D0.w*2),D0		; 303b 0206
	0x4e,0xfb,0x00,0x04		// Jmp			L33AB(PC,D0.w)			; 4efb 0004
},
{
	0xff,0xff,0x00,0x00,0x00,0x00,
	0xff,0x00,
	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff
},
};

// --

static struct JTable Table12 =
{
	+4,			// Tabel Pos

	+6,			// Relative Pos

	-12,		// EntryPos
	ET_Pos32,	// EntryType

	-14,		// Data Pos
	18,			// Data Size
{
	0x0c,0x81,0,0,0,0,		// Cmpi.l		#$00000034,D1			; 0c81 0000 0034
	0x6c,0,0,0,				// Bge.w		L33C3					; 6c00 0242
	0x32,0x3b,0x12,0x06,	// Move.w		L33C1(PC,D1.w*2),D1		; 323b 1206
	0x4e,0xfb,0x10,0x04		// Jmp			L33C2(PC,D1.w)			; 4efb 1004
},
{
	0xff,0xff,0x00,0x00,0x00,0x00,
	0xff,0x00,0x00,0x00,
	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff
},
};

// --

static struct JTable Table13 =
{
	+4,			// Tabel Pos

	+6,			// Relative Pos

	-12,		// EntryPos
	ET_Pos32,	// EntryType

	-14,		// Data Pos
	18,			// Data Size
{
	0x0c,0x80,0,0,0,0,		// Cmpi.l		#$00000005,D0			; 0c80 0000 0005
	0x6c,0,					// Bge.b		L35A1					; 6c00
	0xd0,0x40,				// Add.w		D0,D0					; d040
	0x30,0x3b,0x00,0x06,	// Move.w		(L359F,PC,D0.w),D0		; 303b 0006
	0x4e,0xfb,0x00,0x04		// Jmp			(L35A9,PC,D0.w)			; 4efb 1004
},
{
	0xff,0xff,0x00,0x00,0x00,0x00,
	0xff,0x00,
	0xff,0xff,
	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff
},
};

// --

static struct JTable Table14 =
{
	+4,			// Tabel Pos

	+6,			// Relative Pos

	-12,		// EntryPos
	ET_Pos32,	// EntryType

	-14,		// Data Pos
	18,			// Data Size
{
	0xb0,0xbc,0,0,0,0,		// Cmp.l		#$00000013,D0			; b0bc 0000 0013
	0x6c,0x00,0,0,			// Bge.w		L005A					; 6c00 36e6
	0x30,0x3b,0x02,0x06,	// Move.w		L0058(PC,D0.w*2),D0		; 303b 0206
	0x4e,0xfb,0x00,0x04		// Jmp			L0059(PC,D0.w)			; 4efb 0004
},
{
	0xff,0xff,0x00,0x00,0x00,0x00,
	0xff,0xff,0x00,0x00,
	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff
},
};

// --

static struct JTable Table15 =
{
	+4,			// Tabel Pos

	+6,			// Relative Pos

	-12,		// EntryPos
	ET_Pos32,	// EntryType

	-14,		// Data Pos
	18,			// Data Size
{
	0xb0,0xbc,0,0,0,0,		// Cmp.l		#$00000013,D0			; b0bc 0000 0013
	0x64,0x00,0,0,			// Bcc.w		L005A					; 6400 0598
	0x30,0x3b,0x02,0x06,	// Move.w		L0058(PC,D0.w*2),D0		; 303b 0206
	0x4e,0xfb,0x00,0x04		// Jmp			L0059(PC,D0.w)			; 4efb 0004
},
{
	0xff,0xff,0x00,0x00,0x00,0x00,
	0xff,0xff,0x00,0x00,
	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff
},
};

// --

static struct JTable Table16 =
{
	+4,			// Tabel Pos

	+6,			// Relative Pos

	-10,		// EntryPos
	ET_Pos32,	// EntryType

	-12,		// Data Pos
	16,			// Data Size
{
	0xb0,0xbc,0,0,0,0,		// Cmp.l		#$00000013,D0			; b0bc 0000 0013
	0x64,0x00,				// Bcc.b		L005A					; 645e 
	0x30,0x3b,0x02,0x06,	// Move.w		L0058(PC,D0.w*2),D0		; 303b 0206
	0x4e,0xfb,0x00,0x04		// Jmp			L0059(PC,D0.w)			; 4efb 0004
},
{
	0xff,0xff,0x00,0x00,0x00,0x00,
	0xff,0x00,
	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff
},
};

// --

static struct JTable Table17 =
{
	+4,			// Tabel Pos

	+6,			// Relative Pos

	-10,		// EntryPos
	ET_Pos32,	// EntryType

	-12,		// Data Pos
	16,			// Data Size
{
	0xb0,0xbc,0,0,0,0,		// Cmp.l		#$00000011,D0			; b0bc 0000 0011
	0x6c,0,					// Bge.b		L33AD					; 6c5c
	0x30,0x3b,0x02,0x06,	// Move.w		L33AA(PC,D0.w*2),D0		; 303b 0206
	0x4e,0xfb,0x00,0x04		// Jmp			L33AB(PC,D0.w)			; 4efb 0004
},
{
	0xff,0xff,0x00,0x00,0x00,0x00,
	0xff,0x00,
	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff
},
};

// --

static struct JTable Table18 =
{
	+4,			// Tabel Pos

	+6,			// Relative Pos

	-12,		// EntryPos
	ET_Pos32,	// EntryType

	-14,		// Data Pos
	18,			// Data Size
{
	0xb0,0xbc,0,0,0,0,		// Cmp.l		#$00000005,D0			; b0bc 0000 0005
	0x6c,0,					// Bge.b		L35A1					; 6c00
	0xd0,0x40,				// Add.w		D0,D0					; d040
	0x30,0x3b,0x00,0x06,	// Move.w		(L359F,PC,D0.w),D0		; 303b 0006
	0x4e,0xfb,0x00,0x04		// Jmp			(L35A9,PC,D0.w)			; 4efb 1004
},
{
	0xff,0xff,0x00,0x00,0x00,0x00,
	0xff,0x00,
	0xff,0xff,
	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff
},
};

// --

static struct JTable Table19 =
{
	+4,			// Tabel Pos

	+6,			// Relative Pos

	-12,		// EntryPos
	ET_Pos32,	// EntryType

	-14,		// Data Pos
	18,			// Data Size
{
	0xb2,0xbc,0,0,0,0,		// Cmp.l		#$00000034,D1			; b2bc 0000 0034
	0x6c,0,0,0,				// Bge.w		L33C3					; 6c00 0242
	0x32,0x3b,0x12,0x06,	// Move.w		L33C1(PC,D1.w*2),D1		; 323b 1206
	0x4e,0xfb,0x10,0x04		// Jmp			L33C2(PC,D1.w)			; 4efb 1004
},
{
	0xff,0xff,0x00,0x00,0x00,0x00,
	0xff,0x00,0x00,0x00,
	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff
},
};

// --

static struct JTable Table20 =
{
	+4,			// Tabel Pos

	+6,			// Relative Pos

	-12,		// EntryPos
	ET_Pos16,	// EntryType

	-14,		// Data Pos
	18,			// Data Size
{
	0xb0,0x7c,0,0,			// Cmp.w		#$0005,D0				; 0c40 0005
	0x64,0x00,0,0,			// Bcc.w		L02F6					; 6400 0400
	0xd0,0x40,				// Add.w		D0,D0					; d040
	0x30,0x3b,0x00,0x06,	// Move.w		L02F4(PC,D0.w),D0		; 303b 0006
	0x4e,0xfb,0x00,0x04		// Jmp			L02F5(PC,D0.w)			; 4efb 0004
},
{
	0xff,0xff,0x00,0x00,
	0xff,0xff,0x00,0x00,
	0xff,0xff,
	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff
},
};

// --

static struct JTable Table21 =
{
	+4,			// Tabel Pos

	+6,			// Relative Pos

	-10,		// EntryPos
	ET_Pos16,	// EntryType

	-12,		// Data Pos
	16,			// Data Size
{
	0xb0,0x7c,0,0,			// Cmp.w		#$0005,D0				; 0c40 0005
	0x64,0,					// Bcc.b		L1163					; 6436
	0xd0,0x40,				// Add.w		D0,D0					; d040
	0x30,0x3b,0x00,0x06,	// Move.w		L1161(PC,D0.w),D0		; 303b 0006
	0x4e,0xfb,0x00,0x04		// Jmp			L1162(PC,D0.w)			; 4efb 0004
},
{
	0xff,0xff,0x00,0x00,
	0xff,0,
	0xff,0xff,
	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff
},
};

// --

static struct JTable Table22 =
{
	+4,			// Tabel Pos

	+6,			// Relative Pos

	-12,		// EntryPos
	ET_Pos16,	// EntryType

	-14,		// Data Pos
	18,			// Data Size
{
	0xb0,0x7c,0,0,			// Cmp.w		#$0008,D0				; b07c 0008
	0x6c,0x00,0,0,			// Bge.w		L19E0					; 6c00 0092
	0xd0,0x40,				// Add.w		D0,D0					; d040
	0x30,0x3b,0x00,0x06,	// Move.w		L19DC(PC,D0.w),D0		; 303b 0006
	0x4e,0xfb,0x00,0x04		// Jmp			L19DD(PC,D0.w)			; 4efb 0004
},
{
	0xff,0xff,0x00,0x00,
	0xff,0xff,0x00,0x00,
	0xff,0xff,
	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff
},
};

// --

static struct JTable Table23 =
{
	+4,			// Tabel Pos

	+6,			// Relative Pos

	-10,		// EntryPos
	ET_Pos16,	// EntryType

	-12,		// Data Pos
	16,			// Data Size
{
	0xb0,0x7c,0,0,			// Cmp.w		#$0009,D0				; b07c 0009
	0x6c,0,					// Bge.b		L19F0					; 6c6e 
	0xd0,0x40,				// Add.w		D0,D0					; d040
	0x30,0x3b,0x00,0x06,	// Move.w		L19EE(PC,D0.w),D0		; 303b 0006
	0x4e,0xfb,0x00,0x04		// Jmp			L19EF(PC,D0.w)			; 4efb 0004
},
{
	0xff,0xff,0x00,0x00,
	0xff,0x00,
	0xff,0xff,
	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff
},
};

// --

static struct JTable Table24 =
{
	+4,			// Tabel Pos

	+6,			// Relative Pos

	-10,		// EntryPos
	ET_Pos16,	// EntryType

	-12,		// Data Pos
	16,			// Data Size
{
	0xb0,0x7c,0,0,			// Cmp.w		#$0009,D0				; b07c 0005
	0x64,0x00,0,0,			// Bcc.w		L06C6					; 6400 00f8
	0x30,0x3b,0x02,0x06,	// Move.w		L06C4(PC,D0.w*2),D0		; 303b 0206
	0x4e,0xfb,0x00,0x04		// Jmp			L19EF(PC,D0.w)			; 4efb 0004
},
{
	0xff,0xff,0x00,0x00,
	0xff,0xff,0x00,0x00,
	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff
},
};

// --

static struct JTable Table25 =
{
	+4,			// Tabel Pos

	+6,			// Relative Pos

	-8,			// EntryPos
	ET_Pos16,	// EntryType

	-10,		// Data Pos
	14,			// Data Size
{
	0xb0,0x7c,0,0,			// Cmp.w		#$0005,D0				; b07c 0005
	0x64,0,					// Bcc.b		L0FC4					; 6434
	0x30,0x3b,0x02,0x06,	// Move.w		L0FC2(PC,D0.w*2),D0		; 303b 0206
	0x4e,0xfb,0x00,0x04		// Jmp			L19EF(PC,D0.w)			; 4efb 0004
},
{
	0xff,0xff,0x00,0x00,
	0xff,0x00,
	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff
},
};

// --

static struct JTable *JumpTables[] =
{
	& Table1,
	& Table2,
	& Table3,
	& Table4,
	& Table5,
	& Table6,
	& Table7,
	& Table8,
	& Table9,
	& Table10,
	& Table11,
	& Table12,
	& Table13,
	& Table14,
	& Table15,
	& Table16,
	& Table17,
	& Table18,
	& Table19,
	& Table20,
	& Table21,
	& Table22,
	& Table23,
	& Table24,
	& Table25,
	NULL,
};

// --

static enum RS4JumpStat check_JumpTable( enum RS4ErrorCode *errcode, RS4Trace *rt, struct JTable *jt, int count )
{
enum RS4ErrorCode ec;
enum RS4JumpStat js;
RS4FileSection *sec;
uint8_t *type;
uint8_t *mem;
int32_t entries;
int64_t adr;
int64_t off;
int64_t pos;
uint8_t chr;
int cnt;

	ec		= RS4ErrStat_Error;
	js		= RS4JumpStat_Error;
	sec		= rt->rt_Section;
	adr		= rt->rt_CurMemAdr;
	type	= /* */ sec->rfs_MemoryType;
	mem		= /* */ sec->rfs_MemoryBuf;
	off		= adr - sec->rfs_MemoryAdr;
	pos		= off + jt->DataPos;

#if 0
{
	printf( "jt : %p\n", jt );
	printf( "Sec: $%08" PRIx64 " #%d\n", sec->rfs_MemoryAdr, sec->rfs_SecNr );
	printf( "Adr: $%08" PRIx64 "\n", adr );
	printf( "Off: $%08" PRIx64 "\n", off );
	printf( "Pos: $%08" PRIx64 "\n", pos );
}
#endif

	// Check Data

	for( cnt=0 ; cnt < jt->DataSize ; cnt++ )
	{
		chr = mem[pos+cnt] & jt->Mask[cnt];

		if ( chr != jt->Data[cnt] )
		{
			ec = RS4ErrStat_Okay;
			js = RS4JumpStat_Skip;
			goto bailout;
		}
	}

	// Found Table

	memset( & type[pos], RS4MT_Code, jt->DataSize );

	if ( Verbose )
	{
		printf( "Found Jump Table #%d at $%08" PRIx64 "\n", count, rt->rt_CurMemAdr );
	}

	switch( jt->EntryType )
	{
		case ET_Fixed:
		{
			entries = jt->EntryPos;
			break;
		}

		case ET_Pos16:
		{
			pos = off + jt->EntryPos;
			entries = (( mem[pos+0] << 8 ) | ( mem[pos+1] << 0 ));
			break;
		}

		case ET_Pos32:
		{
			pos = off + jt->EntryPos;
			entries = (( mem[pos+0] << 24 ) | ( mem[pos+1] << 16 ) | ( mem[pos+2] << 8 ) | ( mem[pos+3] << 0 ));
			break;
		}

		default:
		{
			ec = RS4ErrStat_Error;
			js = RS4JumpStat_Error;
			#ifdef DEBUG
			printf( "%s:%04d: Error, Type #%d\n", __FILE__, __LINE__, jt->EntryType );
			#endif
			goto bailout;
		}
	}

	// --

	js = RS4JumpTable_Rel16( & ec, rt->rt_File, adr + jt->TabelPos, adr + jt->RelativePos, entries );

//	js = RS4JumpStat_Skip;
//	ec = RS4ErrStat_Okay;

	// --

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( js );
}

// --

enum RS4FuncStat RS4Trace_JumpTable( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
enum RS4JumpStat js;
struct JTable *jt;
int pos;

	ec	= RS4ErrStat_Error;
	fs	= RS4FuncStat_Error;
	pos	= 0;

	while( TRUE )
	{
		jt = JumpTables[pos];

		if ( ! jt )
		{
			// Not Found, is not an error
			printf( "Possible new type of Jump Table at $%08" PRIx64 "\n", rt->rt_CurMemAdr );
			break;
		}

		js = check_JumpTable( & ec, rt, jt, pos+1 );

		/**/ if ( js == RS4JumpStat_Error )
		{
			// ec allready set

			#ifdef DEBUG
			printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
			#endif

			goto bailout;
		}
		else if ( js == RS4JumpStat_Handled )
		{
			// Done, found and handled
			break;
		}
		#ifdef DEBUG
		else if ( js != RS4JumpStat_Skip )
		{
			// ec allready set

			#ifdef DEBUG
			printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
			#endif

			goto bailout;
		}
		#endif

		pos++;
	}

	// --

	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

	// --

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fs );
}

// --
