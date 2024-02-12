
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

#include "ReSrc4.h"

// -- Overview

#if 0

-- Table #1 - Lightwave 5.0
	0x0c,0x80,0,0,0,0,		// Cmpi.l		#$00000013,D0			; 0c80 0000 0013
	0x64,0x00,0,0,			// Bcc.w		L005A					; 6400 0598
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

int JumpTable_Rel16( struct HunkStruct *hs, int32_t tabel_adr, int32_t relative_adr, int entries )
{
struct HunkLabel *rel;
struct HunkLabel *hl;
struct HunkNode *hn;
int32_t adr;
int32_t off;
int16_t *offsets;
uint8_t *type;
int16_t r16;
int error;
int cnt;

	error = true;

	// --

	hn = Hunk_FindNode( hs, tabel_adr );

	if ( hn == NULL )
	{
		printf( "%s:%04d: Error finding address %08x\n", __FILE__, __LINE__, tabel_adr );
		goto bailout;
	}

	hl = Hunk_AddLabel2( hs, hn, tabel_adr, LT_RelativeWord );

	if ( hl == NULL )
	{
		printf( "%s:%04d: Error adding label at address %08x\n", __FILE__, __LINE__, tabel_adr );
		goto bailout;
	}

	rel = Hunk_AddLabel2( hs, hn, relative_adr, LT_Unset );

	if ( rel == NULL )
	{
		printf( "%s:%04d: Error adding label at address %08x\n", __FILE__, __LINE__, relative_adr );
		goto bailout;
	}

	hl->hl_Label_Type	= LT_RelativeWord;
	hl->hl_Label_Size	= entries * 2;
	hl->hl_Label_Ref	= rel;

	// --

	type = hn->hn_MemoryType;
	off = tabel_adr - hn->hn_MemoryAdr;

	memset( & type[ off ], MT_Data, entries * 2 );

	// --

	offsets = (void *) & hn->hn_MemoryBuf[ off ];

	for( cnt=0 ; cnt<entries ; cnt++ )
	{
		r16 = SWAP16( offsets[cnt] );
		adr = relative_adr + r16;

		hl = Hunk_AddLabel2( hs, hn, adr, LT_Code );

		if ( hl == NULL )
		{
			printf( "%s:%04d: Error adding label at %08x\n", __FILE__, __LINE__, adr );
			goto bailout;
		}

		if ( Trace_AddBrance( hs, NULL, adr ))
		{
			printf( "%s:%04d: Error adding brance at %08x\n", __FILE__, __LINE__, adr );
			goto bailout;
		}
	}

	// --

	error = false;

bailout:

	return( error );
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

	#if 0
	Cmpi.l		#$00000005,D0				; $000c3492 0c80 0000 0005
	Bge.b		L35A1						; $000c3498 6c20
	Add.w		D0,D0						; $000c349a d040
	Move.w		(L359F,PC,D0.w),D0			; $000c349c 303b 0006
	Jmp			(L35A0,PC,D0.w)				; $000c34a0 4efb 0004
	#endif

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
	NULL,
};

// --

static int check_JumpTable( struct HunkStruct *hs, struct M68kStruct *ms, struct JTable *jt )
{
struct HunkNode *hn;
uint8_t *mem;
int32_t adr;
int32_t off;
int32_t pos;
uint8_t chr;
int entries;
int retval;
int cnt;

	// -1 = Error
	//  0 = Not Found
	// +1 = Found and Handled
	retval = 0;

	hn	= ms->ms_HunkNode;
	mem = hn->hn_MemoryBuf;
	adr = ms->ms_MemoryAdr;
	off = adr - hn->hn_MemoryAdr;

	// Check Data

	pos = off + jt->DataPos;

	for( cnt=0 ; cnt < jt->DataSize ; cnt++ )
	{
		chr = mem[pos+cnt] & jt->Mask[cnt];

		if ( chr != jt->Data[cnt] )
		{
			goto bailout;
		}
	}

	// Found Table

	memset( & hn->hn_MemoryType[pos], MT_Code, jt->DataSize );

//	printf( "Found Jump Table #%d at %08x\n", count, ms->ms_MemoryAdr );

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
			// Error
			retval = -1;
			goto bailout;
		}
	}

	if ( JumpTable_Rel16( hs, adr + jt->TabelPos, adr + jt->RelativePos, entries ))
	{
		// Error
		retval = -1;
	}
	else
	{
		// Found and Handled
		retval = +1;
	}

bailout:

	return( retval );
}

// --

int Trace_JumpTable( struct HunkStruct *hs, struct M68kStruct *ms )
{
struct JTable *jt;
int error;
int stat;
int pos;

	pos = 0;

	while( true )
	{
		jt = JumpTables[pos];

		if ( jt == NULL )
		{
			// Not Found, is not an error
			printf( "Possible new type of Jump Table at %08x\n", ms->ms_MemoryAdr );
			error = false;
			break;
		}

		stat = check_JumpTable( hs, ms, jt );

		if ( stat < 0 )
		{
			// Error
			error = true;
			break;
		}

		if ( stat > 0 )
		{
			// Found
			error = false;
			break;
		}

		pos++;
	}

	return( error );
}

// --
