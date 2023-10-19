
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
// -- Mode 00 - Clr Dx

inline static void MODE_00( struct M68kStruct *ms, char *outstr )
{
	sprintf( outstr, "%s", Dx_RegNames[ ms->ms_ArgEReg ] );

	ms->ms_CurRegister = & ms->ms_Registers[ REG_Dx + ms->ms_ArgEReg ];
}

// --
// -- Mode 10 - Clr Ax

inline static void MODE_10( struct M68kStruct *ms, char *outstr )
{
	sprintf( outstr, "%s", Ax_RegNames[ ms->ms_ArgEReg ] );

	ms->ms_CurRegister = & ms->ms_Registers[ REG_Ax + ms->ms_ArgEReg ];
}

// --
// -- Mode 20 - Clr (An)

inline static void MODE_20( struct M68kStruct *ms, char *outstr )
{
	sprintf( outstr, "(%s)", Ax_RegNames[ ms->ms_ArgEReg ] );

//	if ( ms->ms_Registers[ REG_Ax + ms->ms_ArgEReg ].mr_Type == RT_Label )
//	{
//		ms->ms_Registers[ REG_Ax + ms->ms_ArgEReg ].mr_Data.mr_LabelNode->hl_Label_Type = LT_Unknown;
//	}
}

// --
// -- Mode 30 - Clr (An)+

inline static void MODE_30( struct M68kStruct *ms, char *outstr )
{
	sprintf( outstr, "(%s)+", Ax_RegNames[ ms->ms_ArgEReg ] );
}

// --
// -- Mode 40 - Clr -(An)

inline static void MODE_40( struct M68kStruct *ms, char *outstr )
{
	sprintf( outstr, "-(%s)", Ax_RegNames[ ms->ms_ArgEReg ] );
}

// --
// -- Mode 50 - jsr (d16,An)

inline static void MODE_50( struct M68kStruct *ms, char *outstr )
{
int16_t val;
uint8_t *mem;
char *lvostr;
int pos;

	mem = ms->ms_MemoryBuf;
	pos = ms->ms_ArgSize;

	val = (( mem[pos] << 8 ) + ( mem[pos+1] << 0 ));

	if (( ms->ms_LibCall ) && ( ms->ms_Registers[ REG_Ax + ms->ms_ArgEReg ].mr_Type & 0xffff ) == RT_Library )
	{
		lvostr = M68k_FindLibFunc( ms, val );
	}
	else
	{
		lvostr = NULL;
	}

	if ( lvostr )
	{
		sprintf( outstr, "(%s,%s)", lvostr, Ax_RegNames[ms->ms_ArgEReg] );
	}
	else
	{
		// Most proberly a Library call, so we use Decimal
		sprintf( outstr, "(%d,%s)", val, Ax_RegNames[ ms->ms_ArgEReg ] );
	}

	ms->ms_ArgSize += 2;
}

// --
// -- Mode 60 - Brief Extension Word Format

inline static void MODE_60_Brief( struct M68kStruct *ms, char *outstr )
{
uint8_t *mem;
int8_t Offset;
int SCALE;
int REG;
int pos;
int AD;
int WL;

	// Brief Extension Word Format
	//
	// [A/D] [REG] [W/L] [Scale] [0] [ signed offset ]
	//   1     3     1      2     1          8
	// [  4 bit  ] [     4 bit     ] [     8 bit     ]

	// Lea $00(Ax,Dx.L),A0
	// lea OFFSET ( ArgReg,   AD[REG],   D0.  WL  *SCALE  )  ,  a1

	// ( d8 , An , Xn.SIZE * SCALE )

	mem = ms->ms_MemoryBuf;
	pos = ms->ms_ArgSize;

	AD		= ( mem[ pos + 0 ] & 0x80 );
	REG		= ( mem[ pos + 0 ] & 0x70 ) >> 4;
	WL		= ( mem[ pos + 0 ] & 0x08 );
	SCALE	= ( mem[ pos + 0 ] & 0x06 ) >> 1;
	Offset	= ( mem[ pos + 1 ] );

	sprintf( outstr, "(%d,%s,%s%s%s)",
		Offset,
		Ax_RegNames[ ms->ms_ArgEReg ],
		( AD ) ? Ax_RegNames[REG] : Dx_RegNames[REG],
		( WL ) ? ".l" : ".w",
		scale_Names[SCALE]
	);

	ms->ms_ArgSize += 2;
}

// --
// -- Mode 60 - Full Extension Word Format

inline static void MODE_60_Full_0( struct M68kStruct *ms, char *outstr )
{
uint8_t *mem;
int ismode;
int SCALE;
int REG;
int pos;
int AD;
int WL;
int BS;
int IS;
int BD;
int IIS;

	// Full Extension Word Format

	// [A/D] [REG] [W/L] [Scale] [1]  [BS] [IS] [BD SIZE]  [0] [I/IS]
	//   1     3     1      2     1    1    1       2       1    3
	// [  4 bit  ] [     4 bit     ]  [      4 bit      ]  [  4bit  ]

	mem = ms->ms_MemoryBuf;
	pos = ms->ms_ArgSize;

	AD		= ( mem[ pos + 0 ] & 0x80 );
	REG		= ( mem[ pos + 0 ] & 0x70 ) >> 4;
	WL		= ( mem[ pos + 0 ] & 0x08 );
	SCALE	= ( mem[ pos + 0 ] & 0x06 ) >> 1;
	BS		= ( mem[ pos + 1 ] & 0x80 );
	IS		= ( mem[ pos + 1 ] & 0x40 );
	BD		= ( mem[ pos + 1 ] & 0x30 ) >> 4;
	IIS		= ( mem[ pos + 1 ] & 0x03 );
	ismode	= (( IS >> 3 ) || ( IIS ));

#if 0
	// AsmPro
	// $3635,$3320,$ff12		Move.w (A5,D3.W*2,$FF12.W),D3
	// $4275,$3320,$ff12		clr.w (A5,D3.W*2,$FF12.W)
	// $4275,$3330,$0000,$ff12	clr.w (A5,D3.W*2,$0000FF12.L)


M68k__EffectiveAddress.c:0192: MemoryAdr . : 00028742
M68k__EffectiveAddress.c:0193: Val 1 ..... : 1 -- Check passed
M68k__EffectiveAddress.c:0194: Val 2 ..... : 0 -- Check paased
M68k__EffectiveAddress.c:0195: AD ........ : 0 -- Dx Reg
M68k__EffectiveAddress.c:0196: REG ....... : 3 -- D3 Reg
M68k__EffectiveAddress.c:0197: WL ........ : 0 -- Signed Word
M68k__EffectiveAddress.c:0198: SCALE ..... : 1 -- *2
M68k__EffectiveAddress.c:0199: BS ........ : 0 -- Add Base Reg
M68k__EffectiveAddress.c:0200: IS ........ : 0 -- Add Index Operand
M68k__EffectiveAddress.c:0201: BD ........ : 2 -- Word Displacement
M68k__EffectiveAddress.c:0202: IIS ....... : 0 -- [0][000] No Memory Indirect Action

#endif

	#if 0
	printf( "\n" );
	printf( "%s:%04d: MemoryAdr . : %08x\n", __FILE__, __LINE__,  ms->ms_MemoryAdr );
	printf( "%s:%04d: Val 1 ..... : %d\n", __FILE__, __LINE__, ( mem[ pos + 0 ] & 0x01 ));
	printf( "%s:%04d: Val 2 ..... : %d\n", __FILE__, __LINE__, ( mem[ pos + 1 ] & 0x80 ));
	printf( "%s:%04d: AD ........ : %d\n", __FILE__, __LINE__, AD );
	printf( "%s:%04d: REG ....... : %d\n", __FILE__, __LINE__, REG );
	printf( "%s:%04d: WL ........ : %d\n", __FILE__, __LINE__, WL );
	printf( "%s:%04d: SCALE ..... : %d\n", __FILE__, __LINE__, SCALE );
	printf( "%s:%04d: BS ........ : %d\n", __FILE__, __LINE__, BS );
	printf( "%s:%04d: IS ........ : %d\n", __FILE__, __LINE__, IS );
	printf( "%s:%04d: BD ........ : %d\n", __FILE__, __LINE__, BD );
	printf( "%s:%04d: IIS ....... : %d\n", __FILE__, __LINE__, IIS );
	printf( "%s:%04d: ismode .... : %d\n", __FILE__, __LINE__, ismode );
	#endif

	//
	// (   d8 , An ,   Xn.SIZE * SCALE )
	// (   bd , An ,   Xn.SIZE * SCALE )
	// ( [ bd , An ] , Xn.SIZE * SCALE ,   od )
	// ( [ bd , An ,   Xn.SIZE * SCALE ] , od )
	//
	// NOTES:
	//
	// 	Dn = Data Register, D0-D7
	//
	// 	An = Address Register, A0-A7
	//
	// 	d8, d16 = A twos-complement, or sign-extended displacement; added as part of the effective address calculation; size is 8 or 16 bits
	//   (d16 and d8 are 16- and 8-bit displacements); when omitted, assemblers use a value of zero.
	//
	// 	Xn = Address or data register used as an index register; form is Xn.SIZE*SCALE, where SIZE is .W or .L lindicates index
	//   register size) and SCALE is 1, 2, 4, or 8 lindex register is multiplied by SCALE); use of SIZE and! or SCALE is optional.
	//
	// 	bd = A twos-complement base displacement; when present, size can be 16 or 32 bits.,
	//
	//	od = Outer displacement, added as part of effective address calculation after any memory indirection; use is optional with a size
	//   of 16 or 32 bits ..
	//
	// 	PC = Program Counter
	//
	// 	<data> = Immediate value of 8, 16, or 32 bits
	//
	//	( ) = Effective address
	//
	//	[ ] = Use as indirect address to long word address.
	//

	switch( ismode )
	{
		case 0: // [0][000]
		{
			if ( BD == 2 ) // Word Displacement
			{
				int16_t val = ( mem[ pos + 2 ] << 8 | mem[ pos + 3 ] );
				sprintf( outstr, "(%s,%s%s%s,%d.w)", 
					Ax_RegNames[ ms->ms_ArgEReg ],
					( AD ) ? Ax_RegNames[ REG ] : Dx_RegNames[ REG ],
					( WL ) ? ".l" : ".w",
					scale_Names[ SCALE ],
					val
				);

// printf( "BD 2 : outstr: %s\n", outstr );

				ms->ms_ArgSize += 4;
			}
//			else if ( BD == 3 )	// Long Displacement
//			{
//				
//			}
			else
			{
				printf( "%s:%04d: Unsupported IIS mode %02x\n", __FILE__, __LINE__, ismode );
				ms->ms_DecodeStatus = MSTAT_Error;
			}
			break;
		}

		default:
		{
			printf( "%s:%04d: Unsupported IIS mode %02x\n", __FILE__, __LINE__, ismode );
			ms->ms_DecodeStatus = MSTAT_Error;
			break;
		}
	}
}

// --

//-- MODE_60_Full( ms, outstr );
//--        {
//--	        BOOL    DA  = ( mem[ pos + 0 ] & 0x80 ) ? TRUE : FALSE;
//--	        uint32  REG = ( mem[ pos + 0 ] & 0x70 ) >> 4;
//--	        BOOL    WL  = ( mem[ pos + 0 ] & 0x08 ) ? TRUE : FALSE;
//--	        uint32 SCALE= ( mem[ pos + 0 ] & 0x06 ) >> 1;
//--
//--	        {
//--	            // Full Extension Word Format
//--
//--	            // [A/D] [REG] [W/L] [Scale] [1]  [BS] [IS] [BD SIZE]  [0] [I/IS]
//--	            //   1     3     1      2     1    1    1       2       1    3
//--	            // [  4 bit  ] [     4 bit     ]  [      4 bit      ]  [  4bit  ]
//--
//--//		      BOOL    BS  = ( mem[ pos + 1 ] & 0x80 ) ? TRUE : FALSE;
//--//		      BOOL    IS  = ( mem[ pos + 1 ] & 0x40 ) ? TRUE : FALSE;
//--		    uint32  SIZE= ( mem[ pos + 1 ] & 0x30 ) >> 4;
//--//		      uint32  UNK = ( mem[ pos + 1 ] & 0x08 );
//--		    uint32  IS = (( mem[ pos + 1 ] & 0x07 ) | (( mem[ pos + 1 ] & 0x40 ) >> 4 ));
//--
//--	            switch( SIZE )
//--	            {
//--		        case 0: // Reserved
//--		        default:
//--		        {
//--		            ms->ms_Error = TRUE;
//--		            break;
//--		        }
//--
//--		        case 1: // Null Displacement
//--		        {
//--			    switch( IS )
//--			    {
//--
//--
//--
//--
//--			        default:
//--			        {
//--				    #ifdef DEBUG
//--				    printf( "Null Displacement : Uninplemented mode %02lx at %p\n", IS, ms->ms_Memory );
//--				    #endif
//--				    ms->ms_Error = TRUE;
//--				    break;
//--			        }
//--			    }
//--
//--			    ms->ms_ArgSize += 2;
//--			    break;
//--		        }
//--
//--		        case 2: // Word Displacement
//--		        {
//--			    switch( IS )
//--			    {
//--			        case 0x00: // No Memory Indirect Action
//--			        case 0x04: // No Memory Indirect Action
//--			        {
//--				    // b035 1120 4e71 : cmp.b (a5,d1.w,$4e71.w),d0
//--
//--				    int16 val = (( mem[pos+2] << 8 ) | ( mem[ pos+3 ] << 0 ));
//--
//--				    sprintf( outstr, "(%d,%s,%s%s%s)",
//--				        val,
//--				        Ax_RegNames[ ms->ms_ArgEReg ],
//--				        Dx_RegNames[ REG ],
//--				        ( WL ) ? ".l" : ".w",
//--				        scale_Names[ SCALE ]
//--				    );
//--
//--				    ms->ms_ArgSize += 4;
//--//				      ms->ms_Error = TRUE;
//--				    break;
//--			        }
//--
//--
//--			        case 0x01: // Indirect Preindexed with Null Outer Displacement
//--			        {
//--				    // 2-10
//--			        }
//--
//--			        case 0x02: // Indirect Preindexed with Word Outer Displacement
//--			        {
//--				    // 2-10
//--			        }
//--
//--			        case 0x03: // Indirect Preindexed with Long Outer Displacement
//--			        {
//--				    // 2-10
//--			        }
//--
//--
//--			        case 0x05: // Indirect Postindexed with Null Outer Displacement
//--			        {
//--				    // 2-11
//--			        }
//--
//--			        case 0x06: // Indirect Postindexed with Word Outer Displacement
//--			        {
//--				    // 2-11
//--			        }
//--
//--			        case 0x07: // Indirect Postindexed with Long Outer Displacement
//--			        {
//--				    // 2-11
//--			        }
//--
//--
//--			        case 0x09: // Memory indirect with Null Outer Displacement
//--			        {
//--				    // 2-12
//--			        }
//--
//--			        case 0x0a: // Memory indirect with Word Outer Displacement
//--			        {
//--				    // 2-12
//--			        }
//--
//--			        case 0x0b: // Memory indirect with Long Outer Displacement
//--			        {
//--				    // 2-12
//--			        }
//--
//--			        default:
//--			        {
//--				    #ifdef DEBUG
//--				    printf( "Word Displacement : Uninplemented mode %02lx at %p\n", IS, ms->ms_Memory );
//--				    #endif
//--				    ms->ms_Error = TRUE;
//--				    break;
//--			        }
//--			    }
//--			    break;
//--		        }
//--
//--		        case 3: // Long Displacement
//--		        {
//--			    switch( IS )
//--			    {
//--
//--
//--
//--
//--
//--			        default:
//--			        {
//--				    #ifdef DEBUG
//--				    printf( "Long Displacement : Uninplemented mode %02lx at %p\n", IS, ms->ms_Memory );
//--				    #endif
//--				    ms->ms_Error = TRUE;
//--				    break;
//--			        }
//--			    }
//--
//--			    ms->ms_ArgSize += 6;
//--			    break;
//--		        }
//--	            }
//--            }
//--	        break;
//--        }

// --
// -- Mode 70 - move.l $0004.w

inline static void MODE_70( struct M68kStruct *ms, char *outstr )
{
struct HunkLabel *hl;
uint32_t val;
uint8_t *mem;
int pos;

	mem = ms->ms_MemoryBuf;
	pos = ms->ms_ArgSize;

	val = (( mem[pos] << 8 ) + ( mem[pos+1] << 0 ));

	if ( ms->ms_DoExternal )
	{
		hl = Hunk_AddExtLabel( ms->ms_HunkStruct, val, LT_Unknown );

		if (( hl ) && ( hl->hl_Label_Name[0] ))
		{
			sprintf( outstr, "%s.w", hl->hl_Label_Name );
		}
		else
		{
			sprintf( outstr, "$%04x.w", val );
		}
	}
	else
	{
		sprintf( outstr, "$%04x.w", val );
	}

	if ( ms->ms_CurRegister )
	{
		ms->ms_CurRegister->mr_Type = RT_Address;
		ms->ms_CurRegister->mr_Address = val;
	}

	ms->ms_ArgSize += 2;
}

// --
// -- Mode 71 - move.l $00000004

inline static int MODE_71( struct M68kStruct *ms, char *outstr, int isRef, int RefType )
{
struct HunkLabel *parent;
struct HunkLabel *hl;
uint32_t val;
uint8_t *mem;
int pos;
int RefHandled;

	RefHandled = false;

	mem = ms->ms_MemoryBuf;
	pos = ms->ms_ArgSize;
	val = (( mem[pos] << 24 ) + ( mem[pos+1] << 16 ) + ( mem[pos+2] << 8 ) + ( mem[pos+3] << 0 ));

	if ( isRef )
	{
		if ( ms->ms_TraceMode )
		{
			hl = Hunk_AddLabel( ms->ms_HunkStruct, val, RefType );
		}
		else
		{
			hl = Hunk_FindLabel( ms->ms_HunkStruct, val );

			if ( hl == NULL )
			{
				printf( "%s:%04d: Error finding label\n", __FILE__, __LINE__ );
				ms->ms_DecodeStatus = MSTAT_Error;
				goto bailout;
			}
		}

		if (( hl ) && ( hl->hl_Label_Name[0] ))
		{
			parent = hl->hl_Parent;

			if ( parent )
			{
				int off = hl->hl_Label_Offset - parent->hl_Label_Offset;

				if ( off < 0 )
				{
					sprintf( ms->ms_Buf_Argument, "%s%d", parent->hl_Label_Name, off );
				}
				else
				{
					sprintf( ms->ms_Buf_Argument, "%s+%d", parent->hl_Label_Name, off );
				}

//				sprintf( outstr, "%s+%d", parent->hl_Label_Name, hl->hl_Label_Offset - parent->hl_Label_Offset );
			}
			else
			{
				sprintf( outstr, "%s", hl->hl_Label_Name );
			}
		}
		else
		{
			// Not an external as it has a Refs 
			sprintf( outstr, "$%08x.l!!", val );
		}

		if (( hl ) && ( ms->ms_CurRegister ))
		{
			ms->ms_CurRegister->mr_Type = RT_Label;
			ms->ms_CurRegister->mr_LabelNode = hl;
		}

		RefHandled = true;
	}
	else
	{
		if ( ms->ms_DoExternal )
		{
			hl = Hunk_AddExtLabel( ms->ms_HunkStruct, val, LT_Unknown );

			if (( hl ) && ( hl->hl_Label_Name[0] ))
			{
				sprintf( outstr, "%s", hl->hl_Label_Name );
			}
			else
			{
				sprintf( outstr, "$%08x.l", val );
			}
		}
		else
		{
			sprintf( outstr, "$%08x.l", val );
		}

		if ( ms->ms_CurRegister )
		{
			ms->ms_CurRegister->mr_Type = RT_Address;
			ms->ms_CurRegister->mr_Address = val;
		}
	}

	ms->ms_ArgSize += 4;

bailout:

	return( RefHandled );
}

// --
// -- Mode 72 - Clr (xxxx.w,pc)
// xxxx is Signed

inline static void MODE_72( struct M68kStruct *ms, char *outstr, int RefType )
{
struct HunkLabel *parent;
struct HunkLabel *hl;
uint32_t adr;
int16_t val;
uint8_t *mem;
int pos;

	mem = ms->ms_MemoryBuf;
	pos = ms->ms_ArgSize;

	val = (( mem[pos] << 8 ) + ( mem[pos+1] << 0 ));

	adr = ms->ms_MemoryAdr + 2 + val;

	if ( ms->ms_TraceMode )
	{
		// We can get away with AddLabel2, as this is a PC function, 
		// so we can handle Labels out side Hunk Memory area
		hl = Hunk_AddLabel2( ms->ms_HunkStruct, ms->ms_HunkNode, adr, RefType );
//		hl = Hunk_AddLabel( ms->ms_HunkStruct, adr, RefType );
	}
	else
	{
		hl = Hunk_FindLabel( ms->ms_HunkStruct, adr );

		if ( hl == NULL )
		{
			printf( "%s:%04d: Error finding label\n", __FILE__, __LINE__ );
			ms->ms_DecodeStatus = MSTAT_Error;
			goto bailout;
		}
	}

//	hl = Hunk_FindLabel( ms->ms_HunkStruct, adr );

//	hl = Hunk_AddLabel2( ms->ms_HunkStruct, ms->ms_HunkNode, adr, RefType );

	if (( hl ) && ( hl->hl_Label_Name[0] ))
	{
		parent = hl->hl_Parent;

		if ( parent )
		{
			int off = hl->hl_Label_Offset - parent->hl_Label_Offset;

			if ( off < 0 )
			{
				sprintf( ms->ms_Buf_Argument, "(%s%d,PC)", parent->hl_Label_Name, off );
			}
			else
			{
				sprintf( ms->ms_Buf_Argument, "(%s+%d,PC)", parent->hl_Label_Name, off );
			}

//			sprintf( outstr, "(%s+%d,PC)", parent->hl_Label_Name, hl->hl_Label_Offset - parent->hl_Label_Offset );
		}
		else
		{
			sprintf( outstr, "(%s,PC)", hl->hl_Label_Name );
		}
	}
	else
	{
		sprintf( outstr, "($%08x,PC)", adr );
	}

	if (( hl ) && ( ms->ms_CurRegister ))
	{
		ms->ms_CurRegister->mr_Type = RT_Label;
		ms->ms_CurRegister->mr_LabelNode = hl;
	}

	ms->ms_ArgSize += 2;

bailout:

	return;
}

// --
// -- Mode 73 - Brief Extension Word Format

inline static void MODE_73_Brief( struct M68kStruct *ms, char *outstr )
{
struct HunkLabel *parent;
struct HunkLabel *hl;
uint32_t adr;
uint8_t *mem;
int8_t Offset;
int SCALE;
int REG;
int pos;
int AD;
int WL;

	// Brief Extension Word Format
	//
	// [A/D] [REG] [W/L] [Scale] [0] [ signed offset ]
	//   1     3     1      2     1          8
	// [  4 bit  ] [     4 bit     ] [     8 bit     ]

	// ( d8 , PC , Xn.SIZE * SCALE )

	mem = ms->ms_MemoryBuf;
	pos = ms->ms_ArgSize;

	AD		= ( mem[ pos + 0 ] & 0x80 );
	REG		= ( mem[ pos + 0 ] & 0x70 ) >> 4;
	WL		= ( mem[ pos + 0 ] & 0x08 );
	SCALE	= ( mem[ pos + 0 ] & 0x06 ) >> 1;
	Offset	= ( mem[ pos + 1 ] );

	// --

	adr = ms->ms_MemoryAdr + 2 + Offset;

	hl = Hunk_AddLabel2( ms->ms_HunkStruct, ms->ms_HunkNode, adr, 0 );

	if (( hl ) && ( hl->hl_Label_Name[0] ))
	{
		parent = hl->hl_Parent;

		if ( parent )
		{
			int off = hl->hl_Label_Offset - parent->hl_Label_Offset;

			if ( off < 0 )
			{
				sprintf( outstr, "%s%d(PC,%s%s%s)",
					parent->hl_Label_Name,
					off,
					( AD ) ? Ax_RegNames[REG] : Dx_RegNames[REG],
					( WL ) ? ".l" : ".w",
					scale_Names[SCALE]
				);
			}
			else
			{
				sprintf( outstr, "%s+%d(PC,%s%s%s)",
					parent->hl_Label_Name,
					off,
					( AD ) ? Ax_RegNames[REG] : Dx_RegNames[REG],
					( WL ) ? ".l" : ".w",
					scale_Names[SCALE]
				);
			}

//			sprintf( outstr, "%s+%d(PC,%s%s%s)",
//				parent->hl_Label_Name,
//				hl->hl_Label_Offset - parent->hl_Label_Offset,
//				( AD ) ? Ax_RegNames[REG] : Dx_RegNames[REG],
//				( WL ) ? ".l" : ".w",
//				scale_Names[SCALE]
//			);
		}
		else
		{
			sprintf( outstr, "%s(PC,%s%s%s)",
				hl->hl_Label_Name,
				( AD ) ? Ax_RegNames[REG] : Dx_RegNames[REG],
				( WL ) ? ".l" : ".w",
				scale_Names[SCALE]
			);
		}
	}
	else
	{
		sprintf( outstr, "%d(PC,%s%s%s)",
			Offset,
			( AD ) ? Ax_RegNames[REG] : Dx_RegNames[REG],
			( WL ) ? ".l" : ".w",
			scale_Names[SCALE]
		);
	}

	// --

	ms->ms_ArgSize += 2;
}

// --
// -- Mode 74

inline static int MODE_74( struct M68kStruct *ms, char *outstr, int isRef, int RefType )
{
struct HunkLabel *parent;
struct HunkLabel *hl;
uint8_t *mem;
int RefHandled;
int pos;

	mem = ms->ms_MemoryBuf;
	pos = ms->ms_ArgSize;

	RefHandled = false;

	switch( ms->ms_ArgType )
	{
		case OS_Byte:
		{
			uint8_t val;

			val = mem[pos+1] & 0x00ff;

			/**/ if ( ms->ms_DecMode == 1 )	// Signed
			{
				sprintf( outstr, "#%d", val );
			}
			else if ( ms->ms_DecMode == 2 ) // Unsigned
			{
				sprintf( outstr, "#%u", val );
			}
			else
			{
				sprintf( outstr, "#$%02x", val );
			}

			ms->ms_ArgSize += 2;
			break;
		}

		case OS_Word:
		{
			uint16_t val;

			val = (( mem[pos] << 8 ) + ( mem[pos+1] << 0 ));

			/**/ if ( ms->ms_DecMode == 1 ) // Signed
			{
				sprintf( outstr, "#%d", val );
			}
			else if ( ms->ms_DecMode == 2 ) // Unsigned
			{
				sprintf( outstr, "#%u", val );
			}
			else
			{
				sprintf( outstr, "#$%04x", val );
			}

			ms->ms_ArgSize += 2;
			break;
		}

		case OS_Long:
		{
			uint32_t adr;

			adr = (( mem[pos] << 24 ) + ( mem[pos+1] << 16 ) + ( mem[pos+2] << 8 ) + ( mem[pos+3] << 0 ));

			if ( isRef )
			{
				if ( ms->ms_TraceMode )
				{
					hl = Hunk_AddLabel( ms->ms_HunkStruct, adr, RefType );
				}
				else
				{
					hl = Hunk_FindLabel( ms->ms_HunkStruct, adr );

					if ( hl == NULL )
					{
						printf( "%s:%04d: Error finding label\n", __FILE__, __LINE__ );
						ms->ms_DecodeStatus = MSTAT_Error;
						goto bailout;
					}
				}

//				hl = Hunk_FindLabel( ms->ms_HunkStruct, adr );

//				hl = Hunk_AddLabel2( ms->ms_HunkStruct, ms->ms_HunkNode, adr, RefType );

				if ( hl == NULL )
				{
					printf( "%s:%04d: Error adding label\n", __FILE__, __LINE__ );
					ms->ms_DecodeStatus = MSTAT_Error;
					goto bailout;
				}

				if (( hl ) && ( hl->hl_Label_Name[0] ))
				{
					parent = hl->hl_Parent;

					if ( parent )
					{
						int off = hl->hl_Label_Offset - parent->hl_Label_Offset;

						if ( off < 0 )
						{
							sprintf( ms->ms_Buf_Argument, "#%s%d", parent->hl_Label_Name, off );
						}
						else
						{
							sprintf( ms->ms_Buf_Argument, "#%s+%d", parent->hl_Label_Name, off );
						}

//						sprintf( outstr, "#%s+%d", parent->hl_Label_Name, hl->hl_Label_Offset - parent->hl_Label_Offset );
					}
					else
					{
						sprintf( outstr, "#%s", hl->hl_Label_Name );
					}
				}
				else
				{
					/**/ if ( ms->ms_DecMode == 1 ) // Signed
					{
						sprintf( outstr, "#%d", adr );
					}
					else if ( ms->ms_DecMode == 2 ) // Unsigned
					{
						sprintf( outstr, "#%u", adr );
					}
					else
					{
						sprintf( outstr, "#$%08x", adr );
					}
				}

				if (( hl ) && ( ms->ms_CurRegister ))
				{
					ms->ms_CurRegister->mr_Type = RT_Label;
					ms->ms_CurRegister->mr_LabelNode = hl;
				}

				RefHandled = true;
			}
			else
			{
				sprintf( outstr, "#$%08x", adr );

				if ( ms->ms_CurRegister )
				{
					ms->ms_CurRegister->mr_Type = RT_Address;
					ms->ms_CurRegister->mr_Address = adr;
				}
			}

			ms->ms_ArgSize += 4;
			break;
		}

		default:
		{
			printf( "%s:%04d: Unsupported EA mode at %08x\n", __FILE__, __LINE__, ms->ms_MemoryAdr );
			ms->ms_DecodeStatus = MSTAT_Error;
			break;
		}
	}

bailout:

	return( RefHandled );
}

// --

int M68k_EffectiveAddress( struct M68kStruct *ms, struct HunkRef *isRef, uint32_t RefType )
{
char *outstr;
char *buf;
int RefHandled;
int mode;
int pos;

	RefHandled = false;

	buf = ms->ms_Buf_Argument;
	pos = strlen( buf );

	if ( pos )
	{
		buf[pos++] = ',';
		buf[pos] = 0;
	}

	outstr = & buf[pos];

	mode = (( ms->ms_ArgEMode << 4 ) | ( ms->ms_ArgEReg ));

	switch( mode )
	{
		case 0x00: // 000 REG
		case 0x01:
		case 0x02:
		case 0x03:
		case 0x04:
		case 0x05:
		case 0x06:
		case 0x07:
		{
			// Clr Dn
			MODE_00( ms, outstr );
			break;
		}

		case 0x10: // 001 REG
		case 0x11:
		case 0x12:
		case 0x13:
		case 0x14:
		case 0x15:
		case 0x16:
		case 0x17:
		{
			// Clr An
			MODE_10( ms, outstr );
			break;
		}

		case 0x20: // 010 REG
		case 0x21:
		case 0x22:
		case 0x23:
		case 0x24:
		case 0x25:
		case 0x26:
		case 0x27:
		{
			// Clr (An)
			MODE_20( ms, outstr );
			break;
		}

		case 0x30: // 011 REG
		case 0x31:
		case 0x32:
		case 0x33:
		case 0x34:
		case 0x35:
		case 0x36:
		case 0x37:
		{
			// Clr (An)+
			MODE_30( ms, outstr );
			break;
		}

		case 0x40: // 100 REG
		case 0x41:
		case 0x42:
		case 0x43:
		case 0x44:
		case 0x45:
		case 0x46:
		case 0x47:
		{
			// Clr -(An)
			MODE_40( ms, outstr );
			break;
		}

		case 0x50: // 101 REG
		case 0x51:
		case 0x52:
		case 0x53:
		case 0x54:
		case 0x55:
		case 0x56:
		case 0x57:
		{
			// jsr (d16,An)
			MODE_50( ms, outstr );
			break;
		}

		case 0x60: // 110 REG
		case 0x61:
		case 0x62:
		case 0x63:
		case 0x64:
		case 0x65:
		case 0x66:
		case 0x67:
		{
			// ( d8 , An , Xn.SIZE * SCALE )
			// ( bd , An , Xn.SIZE * SCALE )
			// ( [ bd , An ] , Xn.SIZE * SCALE , od )
			// ( [ bd , An , Xn.SIZE * SCALE ] , od )
			if ( ms->ms_MemoryBuf[ ms->ms_ArgSize ] & 0x01 )
			{
				// Full Extension Word Format
				if ( ms->ms_MemoryBuf[ ms->ms_ArgSize + 1 ] & 0x08 )
				{
					printf( "%s:%04d: Unsupported EA mode %02x at %08x\n", __FILE__, __LINE__, mode, ms->ms_MemoryAdr );
//					sprintf( outstr, "<Mode 1 $%02x: Not implemented>", mode );
//					MODE_60_Full_1( ms, outstr );
					ms->ms_DecodeStatus = MSTAT_Error;
				}
				else
				{
					MODE_60_Full_0( ms, outstr );
//					printf( "%s:%04d: Unsupported EA mode $%02x at %08x\n", __FILE__, __LINE__, mode, ms->ms_MemoryAdr );
//					sprintf( outstr, "<Mode 2 $%02x: Not implemented>", mode );
//					ms->ms_DecodeStatus = MSTAT_Error;
				}
			}
			else
			{
				// Brief Extension Word Format
				MODE_60_Brief( ms, outstr );
			}
			break;
		}

		case 0x70: // 111 000
		{
			// move.l $0004.w
			// ( xxx ).W
			MODE_70( ms, outstr );
			break;
		}

		case 0x71: // 111 001
		{
			// move.l $00000004
			// ( xxx ).L
			RefHandled = MODE_71( ms, outstr, ( isRef ) ? 1 : 0, RefType );
			break;
		}

		case 0x72: // 111 010
		{
			// Clr (xxxx.w,pc)
			// ( d16, PC )
			MODE_72( ms, outstr, RefType );
			break;
		}

		case 0x73: // 110 011
		{
			// ( d8 , PC , Xn.SIZE * SCALE )
			// ( bd , PC , Xn.SIZE * SCALE )
			// ( [ bd , PC ] , Xn.SIZE * SCALE , od )
			// ( [ bd , PC , Xn.SIZE * SCALE ] , od )
			if ( ms->ms_MemoryBuf[ ms->ms_ArgSize ] & 0x01 )
			{
				// Full Extension Word Format
				if ( ms->ms_MemoryBuf[ ms->ms_ArgSize + 1 ] & 0x08 )
				{
					printf( "%s:%04d: Unsupported EA mode $%02x at %08x\n", __FILE__, __LINE__, mode, ms->ms_MemoryAdr );
//					sprintf( outstr, "<Mode 1 $%02x: Not implemented>", mode );
					ms->ms_DecodeStatus = MSTAT_Error;
				}
				else
				{
					printf( "%s:%04d: Unsupported EA mode $%02x at %08x\n", __FILE__, __LINE__, mode, ms->ms_MemoryAdr );
//					sprintf( outstr, "<Mode 2 $%02x: Not implemented>", mode );
					ms->ms_DecodeStatus = MSTAT_Error;
				}
			}
			else
			{
				// Brief Extension Word Format
				MODE_73_Brief( ms, outstr );
			}
			break;
		}

		case 0x74: // 111
		{
			// # <xxx>
			RefHandled = MODE_74( ms, outstr, ( isRef ) ? 1 : 0, RefType );
			break;
		}

		default:
		{
			printf( "%s:%04d: Unsupported EA mode $%02x at %08x\n", __FILE__, __LINE__, mode, ms->ms_MemoryAdr );
//			sprintf( outstr, "<Mode $%02x: Not implemented>", mode );
			ms->ms_DecodeStatus = MSTAT_Error;
			break;
		}
	}

//--bailout:

	return( RefHandled );
}
