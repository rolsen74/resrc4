 
/*
 * Copyright (c) 2014-2023 Rene W. Olsen <renewolsen@gmail.com>
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

struct Opcodes
{
	int			Offset;
	uint32_t	Mask;
	uint32_t	Value;
};

struct JumpTable
{
	int (*Func )( struct HunkStruct *hs, struct M68kStruct *ms );
	struct Opcodes Opcodes[];
};

// --

static int JumpTable_1( struct HunkStruct *hs, struct M68kStruct *ms );
static int JumpTable_2( struct HunkStruct *hs, struct M68kStruct *ms );
static int JumpTable_3( struct HunkStruct *hs, struct M68kStruct *ms );
static int JumpTable_4( struct HunkStruct *hs, struct M68kStruct *ms );
static int JumpTable_5( struct HunkStruct *hs, struct M68kStruct *ms );
static int JumpTable_6( struct HunkStruct *hs, struct M68kStruct *ms );
static int JumpTable_8( struct HunkStruct *hs, struct M68kStruct *ms );
static int JumpTable_9( struct HunkStruct *hs, struct M68kStruct *ms );

// --

static struct JumpTable Table1 =
{
	JumpTable_1,
{{	-14, 0xffff0000, 0x0c400000 },	// $0c400005	Cmpi.w		#$0005,D0
{	-10, 0xffff0000, 0x64000000 },	// $64000400	Bcc.w		L02B0
{	 -6, 0xffff0000, 0xd0400000 },	// $d040		Add.w		D0,D0
{	 -4, 0xffffffff, 0x303b0006 },	// $303b0006	Move.w		(L02AE,PC,D0),D0
{	  0, 0xffffffff, 0x4efb0004 },	// $4efb0004	Jmp			(L02AF,PC,D0)
{	  0, 0x00000000, 0x00000000 },
}};

static struct JumpTable Table2 =
{
	JumpTable_2,
{{	-12, 0xffff0000, 0x0c400000 },	// $0c400005	Cmpi.w		#$0005,D0
{	 -8, 0xffff0000, 0x64000000 },	// $64000400	Bcc.w		L02B0
{	 -4, 0xffffffff, 0x303b0206 },	// $303b0206	Move.w		(L02AE,PC,D0*2),D0
{	  0, 0xffffffff, 0x4efb0004 },	// $4efb0004	Jmp			(L02AF,PC,D0)
{	  0, 0x00000000, 0000000000 }
}};

static struct JumpTable Table3 =
{
	JumpTable_3,
{{	-10, 0xffff0000, 0x0c400000 },	// $0c400005	Cmpi.w		#$0005,D0
{	 -6, 0xff000000, 0x64000000 },	// $64340400	Bcc.b		L02B0
{	 -4, 0xffffffff, 0x303b0206 },	// $303b0206	Move.w		(L02AE,PC,D0*2),D0
{	  0, 0xffffffff, 0x4efb0004 },	// $4efb0004	Jmp			(L02AF,PC,D0)
{	  0, 0x00000000, 0000000000 }
}};

static struct JumpTable Table4 =
{
	JumpTable_4,
{{	-12, 0xffff0000, 0x0c400000 },	// $0c400005	Cmpi.w		#$0005,D0
{	 -8, 0xff000000, 0x6c000000 },	// $64340400	Bge.b		L02B0
{	 -6, 0xffff0000, 0xd0400000 },	// $64340400	Add.w		D0,D0
{	 -4, 0xffffffff, 0x303b0006 },	// $303b0206	Move.w		(L02AE,PC,D0),D0
{	  0, 0xffffffff, 0x4efb0004 },	// $4efb0004	Jmp			(L02AF,PC,D0)
{	  0, 0x00000000, 0000000000 }
}};

static struct JumpTable Table5 =
{
	JumpTable_5,
{{	-14, 0xffff0000, 0x0c400000 },	// $0c400005	Cmpi.w		#$0005,D0
{	-10, 0xff000000, 0x6c000000 },	// $64340400	Bge.w		L02B0
{	 -6, 0xffff0000, 0xd0400000 },	// $64340400	Add.w		D0,D0
{	 -4, 0xffffffff, 0x303b0006 },	// $303b0206	Move.w		(L02AE,PC,D0),D0
{	  0, 0xffffffff, 0x4efb0004 },	// $4efb0004	Jmp			(L02AF,PC,D0)
{	  0, 0x00000000, 0000000000 }
}};

static struct JumpTable Table6 =
{
	JumpTable_6,
{{	-10, 0xffffffff, 0x0241000f },	// $0241000f	Andi.w		#$000f,D1
{	 -6, 0xffff0000, 0xd2010000 },	// $64340400	Add.b		D1,D1
{	 -4, 0xffffffff, 0x323b1006 },	// $323b1006	Move.w		(L02AE,PC,D1),D1
{	  0, 0xffffffff, 0x4efb1022 },	// $4efb1022	Jmp			(L02AF,PC,D1)
{	  0, 0x00000000, 0000000000 }
}};

static struct JumpTable Table7 =
{
	JumpTable_4,
{{	-12, 0xffff0000, 0x0c400000 },	// $0c400005	Cmpi.w		#$0005,D0
{	 -8, 0xff000000, 0x64000000 },	// $64340400	Bcc.b		L02B0
{	 -6, 0xffff0000, 0xd0400000 },	// $64340400	Add.		D0,D0
{	 -4, 0xffffffff, 0x303b0006 },	// $303b0206	Move.w		(L02AE,PC,D0),D0
{	  0, 0xffffffff, 0x4efb0004 },	// $4efb0004	Jmp			(L02AF,PC,D0)
{	  0, 0x00000000, 0000000000 }
}};

static struct JumpTable Table8 =
{
	JumpTable_8,
{{	-14, 0xffff0000, 0x0c800000 },	// $0c400005	Cmpi.l		#$00000005,D0
{	 -8, 0xff000000, 0x6c000000 },	// $64340400	Bge.b		L02B0
{	 -6, 0xffff0000, 0xd0400000 },	// $64340400	Add.w		D0,D0
{	 -4, 0xffffffff, 0x303b0006 },	// $303b0206	Move.w		(L02AE,PC,D0),D0
{	  0, 0xffffffff, 0x4efb0004 },	// $4efb0004	Jmp			(L02AF,PC,D0)
{	  0, 0x00000000, 0000000000 }
}};

static struct JumpTable Table9 =
{
	JumpTable_9,
{{	-14, 0xffff0000, 0x0c800000 },	// $0c400005	Cmpi.l		#$00000005,D0
{	 -8, 0xff000000, 0x6c000000 },	// $64340400	Bge.b		L02B0
{	 -6, 0xffff0000, 0xd0400000 },	// $64340400	Add.w		D0,D0
{	 -4, 0xffffffff, 0x303b0006 },	// $303b0206	Move.w		(L02AE,PC,D0),D0
{	  0, 0xffffffff, 0x4efb0002 },	// $4efb0002	Jmp			(L02AE,PC,D0)
{	  0, 0x00000000, 0000000000 }
}};

static struct JumpTable *JumpTables[] =
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
	NULL,
};

// --

int Trace_JumpTable( struct HunkStruct *hs, struct M68kStruct *ms )
{
struct JumpTable *jt;
struct Opcodes *oc;
uint32_t opcode;
uint8_t *mem;
int error;
int pos;

	error = true;

	pos = 0;

	mem = ms->ms_MemoryBuf;

// printf( "-- %02x%02x %02x%02x\n", mem[0], mem[1], mem[2], mem[3] );

	while( JumpTables[pos] )
	{
		jt = JumpTables[pos];
		oc = jt->Opcodes;

		while( oc->Mask != 0 )
		{
			opcode = (( mem[ oc->Offset + 0 ] << 24 )
			| ( mem[ oc->Offset + 1 ] << 16 )
			| ( mem[ oc->Offset + 2 ] <<  8 )
			| ( mem[ oc->Offset + 3 ] <<  0 ));

			if (( opcode & oc->Mask ) == oc->Value )
			{
				oc++;
			}
			else
			{
				break;
			}
		}

		if ( oc->Mask == 0 )
		{
	//		printf( "Found Jump Table at %08x\n", ms->ms_MemoryAdr );

			if ( jt->Func( hs, ms ))
			{
				printf( "%s:%04d: Error JumpTable Function failed\n", __FILE__, __LINE__ );
				goto bailout;
			}

			memset( ms->ms_MemoryType, MT_Code, 4 );
			break;
		}

		pos++;
	}

	if ( JumpTables[pos] == NULL )
	{
		printf( "Possible new type of Jump Table at %08x\n", ms->ms_MemoryAdr );
	}

	error = false;

bailout:

	return( error );
}

// --
// Jump Table 1

#if 0

static struct JumpTable Table1 =
{
	JumpTable_1,
{{	-14, 0xffff0000, 0x0c400000 },	// $0c400005	Cmpi.w		#$0005,D0
{	-10, 0xffff0000, 0x64000000 },	// $64000400	Bcc.w		L02B0
{	 -6, 0xffff0000, 0xd0400000 },	// $d040		Add.w		D0,D0
{	 -4, 0xffffffff, 0x303b0006 },	// $303b0006	Move.w		(L02AE,PC,D0),D0
{	  0, 0xffffffff, 0x4efb0004 },	// $4efb0004	Jmp			(L02AF,PC,D0)
{	  0, 0x00000000, 0x00000000 },
}};

#endif

static int JumpTable_1( struct HunkStruct *hs, struct M68kStruct *ms )
{
uint32_t rel_adr;
uint32_t tab_adr;
int entries;
int error;

	entries = (( ms->ms_MemoryBuf[ -12 ] << 8 ) | ( ms->ms_MemoryBuf[ -11 ] << 0 ));
	tab_adr = ms->ms_MemoryAdr + 4;
	rel_adr = ms->ms_MemoryAdr + 6;

//	printf( "tab #1 : %08x\n", tab_adr );

	error	= JumpTable_Rel16( hs, tab_adr, rel_adr, entries );




	#if 0
struct HunkLabel *hl;
uint32_t adr;
int16_t *Offsets;
uint8_t *type;
int16_t off;
int error;
int loop;
int cnt;

	error = true;

	// --

	loop = (( ms->ms_MemoryBuf[ -12 ] << 8 ) | ( ms->ms_MemoryBuf[ -11 ] << 0 ));

	Offsets = (void *) ms->ms_MemoryBuf + 4;

	for( cnt=0 ; cnt<loop ; cnt++ )
	{
		off = SWAP16( Offsets[cnt] );
		adr = ms->ms_MemoryAdr;

// if ( off < 0 )	{ printf( "Table 1 : Nagative %d\n", off ); }

		adr += 6;
		adr += off;

		hl = Hunk_AddLabel( ms->ms_HunkStruct, adr, LT_Code );

		if ( hl == NULL )
		{
			printf( "%s:%04d: Error adding label at %08x\n", __FILE__, __LINE__, adr );
			goto bailout;
		}

		if ( Trace_AddBrance( hs, adr ))
		{
			printf( "%s:%04d: Error adding brance at %08x\n", __FILE__, __LINE__, adr );
			goto bailout;
		}
	}

	// --

	hl = Hunk_AddLabel( ms->ms_HunkStruct, ms->ms_MemoryAdr + 4, LT_RelativeWord );

	if ( hl == NULL )
	{
		printf( "%s:%04d: Error adding label at %08x\n", __FILE__, __LINE__, ms->ms_MemoryAdr + 4 );
		goto bailout;
	}

	hl->hl_Label_Type	= LT_RelativeWord;
	hl->hl_Label_Size	= loop * 2;
	hl->hl_Label_Ref	= Hunk_AddLabel( ms->ms_HunkStruct, ms->ms_MemoryAdr + 6, LT_RelativeWord );

	if ( hl->hl_Label_Ref == NULL )
	{
		printf( "%s:%04d: Error adding label at %08x\n", __FILE__, __LINE__, ms->ms_MemoryAdr + 6 );
		goto bailout;
	}

	// --

	type = hl->hl_HunkNode->hn_MemoryType;

	memset( & type[ hl->hl_Label_Offset ], MT_Data, loop*2 );

	// --

	error = false;

bailout:

	#endif

	return( error );
}

// --
/// Jump Table 2

#if 0

static struct JumpTable Table2 =
{
	JumpTable_2,
{{	-12, 0xffff0000, 0x0c400000 },	// $0c400005	Cmpi.w		#$0005,D0
{	 -8, 0xffff0000, 0x64000000 },	// $64000400	Bcc.w		L02B0
{	 -4, 0xffffffff, 0x303b0206 },	// $303b0206	Move.w		(L02AE,PC,D0*2),D0
{	  0, 0xffffffff, 0x4efb0004 },	// $4efb0004	Jmp			(L02AF,PC,D0)
{	  0, 0x00000000, 0000000000 }
}};

#endif

static int JumpTable_2( struct HunkStruct *hs, struct M68kStruct *ms )
{
uint32_t rel_adr;
uint32_t tab_adr;
int entries;
int error;

	entries = (( ms->ms_MemoryBuf[ -10 ] << 8 ) | ( ms->ms_MemoryBuf[ -9 ] << 0 ));
	tab_adr = ms->ms_MemoryAdr + 4;
	rel_adr = ms->ms_MemoryAdr + 6;

//	printf( "tab #2 : %08x\n", tab_adr );

	error	= JumpTable_Rel16( hs, tab_adr, rel_adr, entries );


	#if 0
struct HunkLabel *hl;
uint32_t adr;
int16_t *Offsets;
uint8_t *type;
int16_t off;
int error;
int loop;
int cnt;

	error = true;

	// --

	loop = (( ms->ms_MemoryBuf[ -10 ] << 8 ) | ( ms->ms_MemoryBuf[ -9 ] << 0 ));

	Offsets = (void *) ms->ms_MemoryBuf + 4;

	for( cnt=0 ; cnt<loop ; cnt++ )
	{
		off = SWAP16( Offsets[cnt] );
		adr = ms->ms_MemoryAdr;

		adr += 6;
		adr += off;

		hl = Hunk_AddLabel( ms->ms_HunkStruct, adr, LT_Code );

		if ( hl == NULL )
		{
			printf( "%s:%04d: Error adding label at %08x\n", __FILE__, __LINE__, adr );
			goto bailout;
		}

		if ( Trace_AddBrance( hs, adr ))
		{
			printf( "%s:%04d: Error adding brance at %08x\n", __FILE__, __LINE__, adr );
			goto bailout;
		}
	}

	// --

	hl = Hunk_AddLabel( ms->ms_HunkStruct, ms->ms_MemoryAdr + 4, LT_RelativeWord );

	if ( hl == NULL )
	{
		printf( "%s:%04d: Error adding label at %08x\n", __FILE__, __LINE__, ms->ms_MemoryAdr + 4 );
		goto bailout;
	}

	hl->hl_Label_Type	= LT_RelativeWord;
	hl->hl_Label_Size	= loop * 2;
	hl->hl_Label_Ref	= Hunk_AddLabel( ms->ms_HunkStruct, ms->ms_MemoryAdr + 6, LT_RelativeWord );

	if ( hl->hl_Label_Ref == NULL )
	{
		printf( "%s:%04d: Error adding label at %08x\n", __FILE__, __LINE__, ms->ms_MemoryAdr + 6 );
		goto bailout;
	}

	// --

	type = hl->hl_HunkNode->hn_MemoryType;

	memset( & type[ hl->hl_Label_Offset ], MT_Data, loop*2 );

	// --

	error = false;

bailout:

	#endif

	return( error );
}

// --
// Jump Table 3

#if 0

static struct JumpTable Table3 =
{
	JumpTable_3,
{{	-10, 0xffff0000, 0x0c400000 },	// $0c400005	Cmpi.w		#$0005,D0
{	 -6, 0xff000000, 0x64000000 },	// $64340400	Bcc.b		L02B0
{	 -4, 0xffffffff, 0x303b0206 },	// $303b0206	Move.w		(L02AE,PC,D0*2),D0
{	  0, 0xffffffff, 0x4efb0004 },	// $4efb0004	Jmp			(L02AF,PC,D0)
{	  0, 0x00000000, 0000000000 }
}};

#endif

static int JumpTable_3( struct HunkStruct *hs, struct M68kStruct *ms )
{
uint32_t rel_adr;
uint32_t tab_adr;
int entries;
int error;

	entries = (( ms->ms_MemoryBuf[ -8 ] << 8 ) | ( ms->ms_MemoryBuf[ -7 ] << 0 ));
	tab_adr = ms->ms_MemoryAdr + 4;
	rel_adr = ms->ms_MemoryAdr + 6;

//	printf( "tab #3 : %08x\n", tab_adr );

	error	= JumpTable_Rel16( hs, tab_adr, rel_adr, entries );

	#if 0
struct HunkLabel *hl;
uint32_t adr;
int16_t *Offsets;
uint8_t *type;
int16_t off;
int error;
int loop;
int cnt;

	error = true;

	// --

	loop = (( ms->ms_MemoryBuf[ -8 ] << 8 ) | ( ms->ms_MemoryBuf[ -7 ] << 0 ));

	Offsets = (void *) ms->ms_MemoryBuf + 4;

	for( cnt=0 ; cnt<loop ; cnt++ )
	{
		off = SWAP16( Offsets[cnt] );
		adr = ms->ms_MemoryAdr;

		adr += 6;
		adr += off;

// if ( off < 0 )	{ printf( "Table 3 : Nagative %d\n", off ); }


		hl = Hunk_AddLabel( ms->ms_HunkStruct, adr, LT_Code );

		if ( hl == NULL )
		{
			printf( "%s:%04d: Error adding label at %08x\n", __FILE__, __LINE__, adr );
			goto bailout;
		}

		if ( Trace_AddBrance( hs, adr ))
		{
			printf( "%s:%04d: Error adding brance at %08x\n", __FILE__, __LINE__, adr );
			goto bailout;
		}
	}

	// --

	hl = Hunk_AddLabel( ms->ms_HunkStruct, ms->ms_MemoryAdr + 4, LT_RelativeWord );

	if ( hl == NULL )
	{
		printf( "%s:%04d: Error adding label at %08x\n", __FILE__, __LINE__, ms->ms_MemoryAdr + 4 );
		goto bailout;
	}

	hl->hl_Label_Type	= LT_RelativeWord;
	hl->hl_Label_Size	= loop * 2;
	hl->hl_Label_Ref	= Hunk_AddLabel( ms->ms_HunkStruct, ms->ms_MemoryAdr + 6, LT_RelativeWord );

	if ( hl->hl_Label_Ref == NULL )
	{
		printf( "%s:%04d: Error adding label at %08x\n", __FILE__, __LINE__, ms->ms_MemoryAdr + 6 );
		goto bailout;
	}

	// --

	type = hl->hl_HunkNode->hn_MemoryType;

	memset( & type[ hl->hl_Label_Offset ], MT_Data, loop*2 );

	// --

	error = false;

bailout:

	#endif

	return( error );
}

// --
// Jump Table 4

#if 0

static struct JumpTable Table4 =
{
	JumpTable_4,
{{	-12, 0xffff0000, 0x0c400000 },	// $0c400005	Cmpi.w		#$0005,D0
{	 -8, 0xff000000, 0x6c000000 },	// $64340400	Bge.b		L02B0
{	 -6, 0xffff0000, 0xd0400000 },	// $64340400	Add.w		D0,D0
{	 -4, 0xffffffff, 0x303b0006 },	// $303b0206	Move.w		(L02AE,PC,D0),D0
{	  0, 0xffffffff, 0x4efb0004 },	// $4efb0004	Jmp			(L02AF,PC,D0)
{	  0, 0x00000000, 0000000000 }
}};

#endif

static int JumpTable_4( struct HunkStruct *hs, struct M68kStruct *ms )
{
uint32_t rel_adr;
uint32_t tab_adr;
int entries;
int error;

	entries = (( ms->ms_MemoryBuf[ -10 ] << 8 ) | ( ms->ms_MemoryBuf[ -9 ] << 0 ));
	tab_adr = ms->ms_MemoryAdr + 4;
	rel_adr = ms->ms_MemoryAdr + 6;

//	printf( "tab #4 : %08x\n", tab_adr );

	error	= JumpTable_Rel16( hs, tab_adr, rel_adr, entries );

	#if 0
struct HunkLabel *hl;
uint32_t adr;
int16_t *Offsets;
uint8_t *type;
int16_t off;
int error;
int loop;
int cnt;

	error = true;

	// --

	loop = (( ms->ms_MemoryBuf[ -10 ] << 8 ) | ( ms->ms_MemoryBuf[ -9 ] << 0 ));

	Offsets = (void *) ms->ms_MemoryBuf + 4;

	for( cnt=0 ; cnt<loop ; cnt++ )
	{
		off = SWAP16( Offsets[cnt] );
		adr = ms->ms_MemoryAdr;

		adr += 6;
		adr += off;

		hl = Hunk_AddLabel( ms->ms_HunkStruct, adr, LT_Code );

		if ( hl == NULL )
		{
			printf( "%s:%04d: Error adding label at %08x\n", __FILE__, __LINE__, adr );
			goto bailout;
		}

		if ( Trace_AddBrance( hs, adr ))
		{
			printf( "%s:%04d: Error adding brance at %08x\n", __FILE__, __LINE__, adr );
			goto bailout;
		}
	}

	// --

	hl = Hunk_AddLabel( ms->ms_HunkStruct, ms->ms_MemoryAdr + 4, LT_RelativeWord );

	if ( hl == NULL )
	{
		printf( "%s:%04d: Error adding label at %08x\n", __FILE__, __LINE__, ms->ms_MemoryAdr + 4 );
		goto bailout;
	}

	hl->hl_Label_Type	= LT_RelativeWord;
	hl->hl_Label_Size	= loop * 2;
	hl->hl_Label_Ref	= Hunk_AddLabel( ms->ms_HunkStruct, ms->ms_MemoryAdr + 6, LT_RelativeWord );

	if ( hl->hl_Label_Ref == NULL )
	{
		printf( "%s:%04d: Error adding label at %08x\n", __FILE__, __LINE__, ms->ms_MemoryAdr + 6 );
		goto bailout;
	}

	// --

	type = hl->hl_HunkNode->hn_MemoryType;

	memset( & type[ hl->hl_Label_Offset ], MT_Data, loop*2 );

	// --

	error = false;

bailout:

	#endif

	return( error );
}

// --
// Jump Table 5

#if 0

static struct JumpTable Table5 =
{
	JumpTable_5,
{{	-14, 0xffff0000, 0x0c400000 },	// $0c400005	Cmpi.w		#$0005,D0
{	-10, 0xff000000, 0x6c000000 },	// $64340400	Bge.w		L02B0
{	 -6, 0xffff0000, 0xd0400000 },	// $64340400	Add.w		D0,D0
{	 -4, 0xffffffff, 0x303b0006 },	// $303b0206	Move.w		(L02AE,PC,D0),D0
{	  0, 0xffffffff, 0x4efb0004 },	// $4efb0004	Jmp			(L02AF,PC,D0)
{	  0, 0x00000000, 0000000000 }
}};

#endif

static int JumpTable_5( struct HunkStruct *hs, struct M68kStruct *ms )
{
uint32_t rel_adr;
uint32_t tab_adr;
int entries;
int error;

	entries = (( ms->ms_MemoryBuf[ -12 ] << 8 ) | ( ms->ms_MemoryBuf[ -11 ] << 0 ));
	tab_adr = ms->ms_MemoryAdr + 4;
	rel_adr = ms->ms_MemoryAdr + 6;

//	printf( "tab #5 : %08x\n", tab_adr );

	error	= JumpTable_Rel16( hs, tab_adr, rel_adr, entries );

	#if 0
struct HunkLabel *hl;
uint32_t adr;
int16_t *Offsets;
uint8_t *type;
int16_t off;
int error;
int loop;
int cnt;

	error = true;

	// --

	loop = (( ms->ms_MemoryBuf[ -12 ] << 8 ) | ( ms->ms_MemoryBuf[ -11 ] << 0 ));

	Offsets = (void *) ms->ms_MemoryBuf + 4;

	for( cnt=0 ; cnt<loop ; cnt++ )
	{
		off = SWAP16( Offsets[cnt] );
		adr = ms->ms_MemoryAdr;

		adr += 6;
		adr += off;

		hl = Hunk_AddLabel( ms->ms_HunkStruct, adr, LT_Code );

		if ( hl == NULL )
		{
			printf( "%s:%04d: Error adding label at %08x\n", __FILE__, __LINE__, adr );
			goto bailout;
		}

		if ( Trace_AddBrance( hs, adr ))
		{
			printf( "%s:%04d: Error adding brance at %08x\n", __FILE__, __LINE__, adr );
			goto bailout;
		}
	}

	// --

	hl = Hunk_AddLabel( ms->ms_HunkStruct, ms->ms_MemoryAdr + 4, LT_RelativeWord );

	if ( hl == NULL )
	{
		printf( "%s:%04d: Error adding label at %08x\n", __FILE__, __LINE__, ms->ms_MemoryAdr + 4 );
		goto bailout;
	}

	hl->hl_Label_Type	= LT_RelativeWord;
	hl->hl_Label_Size	= loop * 2;
	hl->hl_Label_Ref	= Hunk_AddLabel( ms->ms_HunkStruct, ms->ms_MemoryAdr + 6, LT_RelativeWord );

	if ( hl->hl_Label_Ref == NULL )
	{
		printf( "%s:%04d: Error adding label at %08x\n", __FILE__, __LINE__, ms->ms_MemoryAdr + 6 );
		goto bailout;
	}

	// --

	type = hl->hl_HunkNode->hn_MemoryType;

	memset( & type[ hl->hl_Label_Offset ], MT_Data, loop*2 );

	// --

	error = false;

bailout:

	#endif

	return( error );
}

// --
// Jump Table 6

#if 0

static struct JumpTable Table6 =
{
	JumpTable_6,
{{	-10, 0xffffffff, 0x0241000f },	// $0241000f	Andi.w		#$000f,D1
{	 -6, 0xffff0000, 0xd2010000 },	// $64340400	Add.b		D1,D1
{	 -4, 0xffffffff, 0x323b1006 },	// $323b1006	Move.w		(L02AE,PC,D1),D1
{	  0, 0xffffffff, 0x4efb1022 },	// $4efb1022	Jmp			(L02AF,PC,D1)
{	  0, 0x00000000, 0000000000 }
}};

#endif

static int JumpTable_6( struct HunkStruct *hs, struct M68kStruct *ms )
{
uint32_t rel_adr;
uint32_t tab_adr;
int entries;
int error;

	entries = 16;
	tab_adr = ms->ms_MemoryAdr + 4;
	rel_adr = ms->ms_MemoryAdr + 36;

//	printf( "tab #6 : %08x\n", tab_adr );

	error	= JumpTable_Rel16( hs, tab_adr, rel_adr, entries );

	#if 0
struct HunkLabel *hl;
uint32_t adr;
int16_t *Offsets;
uint8_t *type;
int16_t off;
int error;
int loop;
int cnt;

	error = true;

	// --

	loop = 16;

	Offsets = (void *) ms->ms_MemoryBuf + 4;

	for( cnt=0 ; cnt<loop ; cnt++ )
	{
		off = SWAP16( Offsets[cnt] );
		adr = ms->ms_MemoryAdr;

		adr += 36;
		adr += off;

		hl = Hunk_AddLabel( ms->ms_HunkStruct, adr, LT_Code );

		if ( hl == NULL )
		{
			printf( "%s:%04d: Error adding label at %08x\n", __FILE__, __LINE__, adr );
			goto bailout;
		}

		if ( Trace_AddBrance( hs, adr ))
		{
			printf( "%s:%04d: Error adding brance at %08x\n", __FILE__, __LINE__, adr );
			goto bailout;
		}
	}

	// --

	hl = Hunk_AddLabel( ms->ms_HunkStruct, ms->ms_MemoryAdr + 4, LT_RelativeWord );

	if ( hl == NULL )
	{
		printf( "%s:%04d: Error adding label at %08x\n", __FILE__, __LINE__, ms->ms_MemoryAdr + 4 );
		goto bailout;
	}

	hl->hl_Label_Type	= LT_RelativeWord;
	hl->hl_Label_Size	= loop * 2;
	hl->hl_Label_Ref	= Hunk_AddLabel( ms->ms_HunkStruct, ms->ms_MemoryAdr + 36, LT_RelativeWord );

	if ( hl->hl_Label_Ref == NULL )
	{
		printf( "%s:%04d: Error adding label at %08x\n", __FILE__, __LINE__, ms->ms_MemoryAdr + 36 );
		goto bailout;
	}

	// --

	type = hl->hl_HunkNode->hn_MemoryType;

	memset( & type[ hl->hl_Label_Offset ], MT_Data, loop*2 );

	// --

	error = false;

bailout:

	#endif

	return( error );
}

// --
// Jump Table 8

#if 0

static struct JumpTable Table8 =
{
	JumpTable_8,
{{	-14, 0xffff0000, 0x0c800000 },	// $0c400005	Cmpi.l		#$00000005,D0
{	 -8, 0xff000000, 0x6c000000 },	// $64340400	Bge.b		L02B0
{	 -6, 0xffff0000, 0xd0400000 },	// $64340400	Add.w		D0,D0
{	 -4, 0xffffffff, 0x303b0006 },	// $303b0206	Move.w		(L02AE,PC,D0),D0
{	  0, 0xffffffff, 0x4efb0004 },	// $4efb0004	Jmp			(L02AF,PC,D0)
{	  0, 0x00000000, 0000000000 }
}};

#endif

static int JumpTable_8( struct HunkStruct *hs, struct M68kStruct *ms )
{
uint32_t rel_adr;
uint32_t tab_adr;
int entries;
int error;

	entries = (( ms->ms_MemoryBuf[ -12 ] << 24 ) | ( ms->ms_MemoryBuf[ -11 ] << 16 ) | ( ms->ms_MemoryBuf[ -10 ] << 8 ) | ( ms->ms_MemoryBuf[ -9 ] << 0 ));
	tab_adr = ms->ms_MemoryAdr + 4;
	rel_adr = ms->ms_MemoryAdr + 6;

	printf( "tab #8 : %08x\n", tab_adr );

	error	= JumpTable_Rel16( hs, tab_adr, rel_adr, entries );

	#if 0
struct HunkLabel *hl;
uint32_t adr;
int16_t *Offsets;
uint8_t *type;
int16_t off;
int error;
int loop;
int cnt;

	error = true;

	// --

	loop = (( ms->ms_MemoryBuf[ -12 ] << 24 ) | ( ms->ms_MemoryBuf[ -11 ] << 16 ) | ( ms->ms_MemoryBuf[ -10 ] << 8 ) | ( ms->ms_MemoryBuf[ -9 ] << 0 ));

	Offsets = (void *) ms->ms_MemoryBuf + 4;

	for( cnt=0 ; cnt<loop ; cnt++ )
	{
		off = SWAP16( Offsets[cnt] );
		adr = ms->ms_MemoryAdr;

		adr += 6;
		adr += off;

		hl = Hunk_AddLabel( ms->ms_HunkStruct, adr, LT_Code );

		if ( hl == NULL )
		{
			printf( "%s:%04d: Error adding label at %08x\n", __FILE__, __LINE__, adr );
			goto bailout;
		}

		if ( Trace_AddBrance( hs, adr ))
		{
			printf( "%s:%04d: Error adding brance at %08x\n", __FILE__, __LINE__, adr );
			goto bailout;
		}
	}

	// --

//--     hl = Hunk_AddLabel( ms->ms_HunkStruct, (uint32) Offsets, LT_RelativeWord );
//--
//--     if ( hl == NULL )
//--     {
//-- 	     goto bailout;
//--     }
//--
//--     hl->hl_Label_Type   = LT_RelativeWord;
//--     hl->hl_Label_Size   = loop * 2;
//--     hl->hl_Label_Ref    = Hunk_AddLabel( ms->ms_HunkStruct, ( (uint32) Offsets ) + 2, LT_RelativeWord );
//--
//--     if ( hl->hl_Label_Ref == NULL )
//--     {
//-- 	     goto bailout;
//--     }
//--
//--     // --
//--
//--     type = hl->hl_HunkNode->hn_MemoryType;
//--
//--     memset( & type[ hl->hl_Label_Offset ], MT_Data, loop*2 );

	// --

printf( "aac %08x\n", ms->ms_MemoryAdr );

	// --

	hl = Hunk_AddLabel( ms->ms_HunkStruct, ms->ms_MemoryAdr + 4, LT_RelativeWord );

	if ( hl == NULL )
	{
		printf( "%s:%04d: Error adding label at %08x\n", __FILE__, __LINE__, ms->ms_MemoryAdr + 4 );
		goto bailout;
	}

	hl->hl_Label_Type	= LT_RelativeWord;
	hl->hl_Label_Size	= loop * 2;
	hl->hl_Label_Ref	= Hunk_AddLabel( ms->ms_HunkStruct, ms->ms_MemoryAdr + 6, LT_RelativeWord );

	if ( hl->hl_Label_Ref == NULL )
	{
		printf( "%s:%04d: Error adding label at %08x\n", __FILE__, __LINE__, ms->ms_MemoryAdr + 6 );
		goto bailout;
	}

	// --

	type = hl->hl_HunkNode->hn_MemoryType;

	memset( & type[ hl->hl_Label_Offset ], MT_Data, loop*2 );

	// --

	error = false;

bailout:

	#endif

	return( error );
}

// --
// Jump Table 9

#if 0

static struct JumpTable Table9 =
{
	JumpTable_9,
{{	-14, 0xffff0000, 0x0c800000 },	// $0c400005	Cmpi.l		#$00000005,D0
{	 -8, 0xff000000, 0x6c000000 },	// $64340400	Bge.b		L02B0
{	 -6, 0xffff0000, 0xd0400000 },	// $64340400	Add.w		D0,D0
{	 -4, 0xffffffff, 0x303b0006 },	// $303b0206	Move.w		(L02AE,PC,D0),D0
{	  0, 0xffffffff, 0x4efb0002 },	// $4efb0002	Jmp			(L02AE,PC,D0)
{	  0, 0x00000000, 0000000000 }
}};

#endif

static int JumpTable_9( struct HunkStruct *hs, struct M68kStruct *ms )
{
uint32_t rel_adr;
uint32_t tab_adr;
int entries;
int error;

	entries = (( ms->ms_MemoryBuf[ -12 ] << 24 ) | ( ms->ms_MemoryBuf[ -11 ] << 16 ) | ( ms->ms_MemoryBuf[ -10 ] << 8 ) | ( ms->ms_MemoryBuf[ -9 ] << 0 ));
	tab_adr = ms->ms_MemoryAdr + 4;
	rel_adr = ms->ms_MemoryAdr + 4;

	printf( "tab #9 : %08x\n", tab_adr );

	error	= JumpTable_Rel16( hs, tab_adr, rel_adr, entries );

	#if 0
struct HunkLabel *hl;
uint32_t adr;
int16_t *Offsets;
uint8_t *type;
int16_t off;
int error;
int loop;
int cnt;

	error = true;

	// --

	loop = (( ms->ms_MemoryBuf[ -12 ] << 24 ) | ( ms->ms_MemoryBuf[ -11 ] << 16 ) | ( ms->ms_MemoryBuf[ -10 ] << 8 ) | ( ms->ms_MemoryBuf[ -9 ] << 0 ));

	Offsets = (void *) ms->ms_MemoryBuf + 4;

	for( cnt=0 ; cnt<loop ; cnt++ )
	{
		off = SWAP16( Offsets[cnt] );
		adr = ms->ms_MemoryAdr;

		adr += 4;
		adr += off;

// if ( off < 0 )	{ printf( "Table9 : Nagative %d\n", off ); }

		hl = Hunk_AddLabel( ms->ms_HunkStruct, adr, LT_Code );

		if ( hl == NULL )
		{
			printf( "%s:%04d: Error adding label at %08x\n", __FILE__, __LINE__, adr );
			goto bailout;
		}

		if ( Trace_AddBrance( hs, adr ))
		{
			printf( "%s:%04d: Error adding brance at %08x\n", __FILE__, __LINE__, adr );
			goto bailout;
		}
	}

	// --

//--     hl = Hunk_AddLabel( ms->ms_HunkStruct, (uint32) Offsets, LT_RelativeWord );
//--
//--     if ( hl == NULL )
//--     {
//-- 	     goto bailout;
//--     }
//--
//--     hl->hl_Label_Type   = LT_RelativeWord;
//--     hl->hl_Label_Size   = loop * 2;
//--     hl->hl_Label_Ref    = hl;   //Hunk_AddLabel( ms->ms_HunkStruct, ( (uint32) Offsets ) + 2, LT_RelativeWord );
//--
//--     if ( hl->hl_Label_Ref == NULL )
//--     {
//-- 	     goto bailout;
//--     }
//--
//--     // --
//--
//--     type = hl->hl_HunkNode->hn_MemoryType;
//--
//--     memset( & type[ hl->hl_Label_Offset ], MT_Data, loop*2 );

	// --

printf( "aad %08x\n", ms->ms_MemoryAdr );

	// --

	hl = Hunk_AddLabel( ms->ms_HunkStruct, ms->ms_MemoryAdr + 4, LT_RelativeWord );

	if ( hl == NULL )
	{
		printf( "%s:%04d: Error adding label at %08x\n", __FILE__, __LINE__, ms->ms_MemoryAdr + 4 );
		goto bailout;
	}

	hl->hl_Label_Type	= LT_RelativeWord;
	hl->hl_Label_Size	= loop * 2;
	hl->hl_Label_Ref	= Hunk_AddLabel( ms->ms_HunkStruct, ms->ms_MemoryAdr + 6, LT_RelativeWord );

	if ( hl->hl_Label_Ref == NULL )
	{
		printf( "%s:%04d: Error adding label at %08x\n", __FILE__, __LINE__, ms->ms_MemoryAdr + 6 );
		goto bailout;
	}

	// --

	type = hl->hl_HunkNode->hn_MemoryType;

	memset( & type[ hl->hl_Label_Offset ], MT_Data, loop*2 );

	// --

	error = false;

bailout:

	#endif

	return( error );
}

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

	rel = Hunk_AddLabel2( hs, hn, relative_adr, LT_Unknown );

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
