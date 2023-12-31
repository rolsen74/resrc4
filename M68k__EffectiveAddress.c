
/*
 * Copyright (c) 2014-2023 Rene W. Olsen < renewolsen @ gmail . com >
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

//#define DOPRINT	1

#if 0

	// AsmPro output

	// 6x = Reg x
	// 73 = PC

	Adr 00030b96, EA 65, BS 0, IS 0, IIS 0, BD 2 - (a5,d0.w*4,$0086.w)

	Adr 00000000, EA 73, BS 0, IS 1, IIS 0, BD 3 - (pc,d0.w*4,$00000086.l)

	Adr 000bf816, EA 67, BS 0, IS 1, IIS 2, BD 2 - ([$0010.w,a7],$0080.w)

	Adr 000c0740, EA 67, BS 0, IS 1, IIS 1, BD 2 - ([$0024.w,a7])

#endif



// --
// -- Mode 60 - Full Extension Word Format

inline static void MODE_60_Full_0( struct M68kStruct *ms, char *outstr )
{
uint8_t *mem;
int SCALE;
int mode;
int REG;
int pos;
int IIS;
int AD;
int WL;
int BS;
int IS;
int BD;

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

	mode	 = BD;
	mode	|= ( IIS ) << 4;
	mode	|= ( IS ) ? 0x0100 : 0x0000 ;
	mode	|= ( BS ) ? 0x1000 : 0x0000 ;

	switch( mode )
	{
		case 0x0002: // BS 0, IS 0, IIS 0, BD 2 - ($0086.w,a5,d0.w*4) ??
		{
			int16_t val = ( mem[ pos + 2 ] << 8 | mem[ pos + 3 ] );

			sprintf( outstr, "(%d.w,%s,%s%s%s)", 
				val,
				Ax_RegNames[ ms->ms_ArgEReg ],
				( AD ) ? Ax_RegNames[ REG ] : Dx_RegNames[ REG ],
				( WL ) ? ".l" : ".w",
				scale_Names[ SCALE ]
			);

			ms->ms_ArgSize += 4;
			break;
		}

		case 0x0003: // BS 0, IS 0, IIS 0, BD 3 - ($0086.l,a5,d0.w*4) ??
		{
			int32_t val = ( mem[ pos + 2 ] << 8 | mem[ pos + 3 ] | mem[ pos + 4 ] | mem[ pos + 5 ] );

			sprintf( outstr, "(%d.l,%s,%s%s%s)", 
				val,
				Ax_RegNames[ ms->ms_ArgEReg ],
				( AD ) ? Ax_RegNames[ REG ] : Dx_RegNames[ REG ],
				( WL ) ? ".l" : ".w",
				scale_Names[ SCALE ]
			);

			ms->ms_ArgSize += 6;
			break;
		}

		default:
		{
			#ifdef DOPRINT
			printf( "\n" );
			printf( "%s:%04d: EA ........ : 60\n", __FILE__, __LINE__ );
			printf( "%s:%04d: MemoryAdr . : %08x\n", __FILE__, __LINE__,  ms->ms_MemoryAdr );
			printf( "%s:%04d: AD ........ : %s\n", __FILE__, __LINE__, (AD)?"Ax":"Dx" );
			printf( "%s:%04d: REG ....... : %d : %s\n", __FILE__, __LINE__, REG,( AD ) ? Ax_RegNames[ REG ] : Dx_RegNames[ REG ] );
			printf( "%s:%04d: WL ........ : %s\n", __FILE__, __LINE__, (WL)?".l":".w" );
			printf( "%s:%04d: SCALE ..... : %d : %s\n", __FILE__, __LINE__, SCALE, scale_Names[SCALE] );
			printf( "%s:%04d: BS ........ : %s\n", __FILE__, __LINE__, (BS)?"Yes":"No" );
			printf( "%s:%04d: IS ........ : %s\n", __FILE__, __LINE__, (IS)?"Yes":"No" );
			printf( "%s:%04d: BD ........ : %d\n", __FILE__, __LINE__, BD );
			printf( "%s:%04d: IIS ....... : %d\n", __FILE__, __LINE__, IIS );
			printf( "%s:%04d: Mode ...... : 0x%04x\n", __FILE__, __LINE__, mode );
			#endif
			ms->ms_DecodeStatus = MSTAT_Error;
			break;
		}
	}
}

// --
// -- Mode 73 - Full Extension Word Format

inline static void MODE_73_Full_0( struct M68kStruct *ms, char *outstr )
{
uint8_t *mem;
//int SCALE;
int mode;
//int REG;
int pos;
int IIS;
//int AD;
//int WL;
int BS;
int IS;
int BD;

	// Full Extension Word Format

	// [A/D] [REG] [W/L] [Scale] [1]  [BS] [IS] [BD SIZE]  [0] [I/IS]
	//   1     3     1      2     1    1    1       2       1    3
	// [  4 bit  ] [     4 bit     ]  [      4 bit      ]  [  4bit  ]

	mem = ms->ms_MemoryBuf;
	pos = ms->ms_ArgSize;

//	AD		= ( mem[ pos + 0 ] & 0x80 );
//	REG		= ( mem[ pos + 0 ] & 0x70 ) >> 4;
//	WL		= ( mem[ pos + 0 ] & 0x08 );
//	SCALE	= ( mem[ pos + 0 ] & 0x06 ) >> 1;
	BS		= ( mem[ pos + 1 ] & 0x80 );
	IS		= ( mem[ pos + 1 ] & 0x40 );
	BD		= ( mem[ pos + 1 ] & 0x30 ) >> 4;
	IIS		= ( mem[ pos + 1 ] & 0x03 );

	mode	 = BD;
	mode	|= ( IIS ) << 4;
	mode	|= ( IS ) ? 0x0100 : 0x0000 ;
	mode	|= ( BS ) ? 0x1000 : 0x0000 ;

	switch( mode )
	{
		#if 0
		case 0x0103: // BS 0, IS 1, IIS 0, BD 3 - (pc,d0.w*4,$00000086.l) ??
		{
			int32_t val = ( mem[ pos + 2 ] << 24 | mem[ pos + 3 ] << 16 | mem[ pos + 4 ] << 8 | mem[ pos + 5 ] );

			sprintf( outstr, "(PC,%s%s%s,%d.l)", 
				( AD ) ? Ax_RegNames[ REG ] : Dx_RegNames[ REG ],
				( WL ) ? ".l" : ".w",
				scale_Names[ SCALE ],
				val
			);

			ms->ms_ArgSize += 6;
			break;
		}
		#endif

		default:
		{
			#ifdef DOPRINT
			printf( "\n" );
			printf( "%s:%04d: EA ........ : 73\n", __FILE__, __LINE__ );
			printf( "%s:%04d: MemoryAdr . : %08x\n", __FILE__, __LINE__,  ms->ms_MemoryAdr );
			printf( "%s:%04d: AD ........ : %s\n", __FILE__, __LINE__, (AD)?"Ax":"Dx" );
			printf( "%s:%04d: REG ....... : %d : %s\n", __FILE__, __LINE__, REG,( AD ) ? Ax_RegNames[ REG ] : Dx_RegNames[ REG ] );
			printf( "%s:%04d: WL ........ : %s\n", __FILE__, __LINE__, (WL)?".l":".w" );
			printf( "%s:%04d: SCALE ..... : %d : %s\n", __FILE__, __LINE__, SCALE, scale_Names[SCALE] );
			printf( "%s:%04d: BS ........ : %s\n", __FILE__, __LINE__, (BS)?"Yes":"No" );
			printf( "%s:%04d: IS ........ : %s\n", __FILE__, __LINE__, (IS)?"Yes":"No" );
			printf( "%s:%04d: BD ........ : %d\n", __FILE__, __LINE__, BD );
			printf( "%s:%04d: IIS ....... : %d\n", __FILE__, __LINE__, IIS );
			printf( "%s:%04d: Mode ...... : 0x%04x\n", __FILE__, __LINE__, mode );
			#endif
			ms->ms_DecodeStatus = MSTAT_Error;
			break;
		}
	}
}

// --

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
// -- Mode 70 - move.l $0004.w

inline static void MODE_70( struct M68kStruct *ms, char *outstr )
{
struct HunkLabel *hl;
int16_t val;
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
			sprintf( outstr, "($%04x).w", val );
		}
	}
	else
	{
		if (( ms->ms_IsPea ) && ( val < 0 ))
		{
			sprintf( outstr, "(%d).w", val );
		}
		else
		{
			sprintf( outstr, "($%04x).w", val );
		}
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
		if ( ms->ms_HunkStruct->hs_PassNr == 1 )
		{
			hl = Hunk_AddLabel( ms->ms_HunkStruct, val, RefType );
		}
		else
		{
			hl = Hunk_FindLabel( ms->ms_HunkStruct, val );

			if ( hl == NULL )
			{
				printf( "%s:%04d: Error finding label at %08x\n", __FILE__, __LINE__, ms->ms_MemoryAdr );
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
			}
			else
			{
				sprintf( outstr, "%s", hl->hl_Label_Name );
			}
		}
		else
		{
			// Not an external as it has a Refs 
			sprintf( outstr, "($%08x).l", val );
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
				sprintf( outstr, "($%08x).l", val );
			}
		}
		else
		{
			sprintf( outstr, "($%08x).l", val );
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

	if ( ms->ms_HunkStruct->hs_PassNr == 1 )
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
			printf( "%s:%04d: Error finding label at %08x\n", __FILE__, __LINE__, ms->ms_MemoryAdr );
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
				sprintf( ms->ms_Buf_Argument, "(%s%d,PC)", parent->hl_Label_Name, off );
			}
			else
			{
				sprintf( ms->ms_Buf_Argument, "(%s+%d,PC)", parent->hl_Label_Name, off );
			}
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
				sprintf( outstr, "(%s%d,PC,%s%s%s)",
					parent->hl_Label_Name,
					off,
					( AD ) ? Ax_RegNames[REG] : Dx_RegNames[REG],
					( WL ) ? ".l" : ".w",
					scale_Names[SCALE]
				);
			}
			else
			{
				sprintf( outstr, "(%s+%d,PC,%s%s%s)",
					parent->hl_Label_Name,
					off,
					( AD ) ? Ax_RegNames[REG] : Dx_RegNames[REG],
					( WL ) ? ".l" : ".w",
					scale_Names[SCALE]
				);
			}
		}
		else
		{
			sprintf( outstr, "(%s,PC,%s%s%s)",
				hl->hl_Label_Name,
				( AD ) ? Ax_RegNames[REG] : Dx_RegNames[REG],
				( WL ) ? ".l" : ".w",
				scale_Names[SCALE]
			);
		}
	}
	else
	{
		sprintf( outstr, "(%d,PC,%s%s%s)",
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
				if ( ms->ms_HunkStruct->hs_PassNr == 1 )
				{
					hl = Hunk_AddLabel( ms->ms_HunkStruct, adr, RefType );
				}
				else
				{
					hl = Hunk_FindLabel( ms->ms_HunkStruct, adr );

					if ( hl == NULL )
					{
						printf( "%s:%04d: Error finding label at %08x\n", __FILE__, __LINE__, ms->ms_MemoryAdr );
						ms->ms_DecodeStatus = MSTAT_Error;
						goto bailout;
					}
				}

				if ( hl == NULL )
				{
					printf( "%s:%04d: Error adding label at %08x\n", __FILE__, __LINE__, ms->ms_MemoryAdr );
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

				if ( ms->ms_CurRegister )
				{
					ms->ms_CurRegister->mr_Type = RT_Address;
					ms->ms_CurRegister->mr_Address = adr;
				}
			}

			ms->ms_ArgSize += 4;
			break;
		}

		case OS_Single:
		{
			// Bits: 1 sign, 8 expo, 23 frac

			sprintf( outstr, "#$" );

			for( int cnt=0 ; cnt<4 ; cnt++ )
			{
				int len = strlen( outstr );

				sprintf( & outstr[len], "%02x", mem[pos+cnt] );
			}

			ms->ms_ArgSize += 4;
			break;
		}

		case OS_Double:
		{
			// Bits: 1 sign, 11 expo, 52 frac

			sprintf( outstr, "#$" );

			for( int cnt=0 ; cnt<8 ; cnt++ )
			{
				int len = strlen( outstr );

				sprintf( & outstr[len], "%02x", mem[pos+cnt] );
			}

			ms->ms_ArgSize += 8;
			break;
		}

		case OS_Extended:
		{
			// Bits: 1 sign, 15 expo, 16 ZERO, 64 mantissa

			sprintf( outstr, "#$" );

			for( int cnt=0 ; cnt<12 ; cnt++ )
			{
				int len = strlen( outstr );

				sprintf( & outstr[len], "%02x", mem[pos+cnt] );
			}

			ms->ms_ArgSize += 12;
			break;
		}

//	OS_Packed,		// fmove.p

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
//					MODE_60_Full_1( ms, outstr );
					printf( "%s:%04d: 60_1 : Unsupported EA mode %02x at %08x\n", __FILE__, __LINE__, mode, ms->ms_MemoryAdr );
					ms->ms_DecodeStatus = MSTAT_Error;
				}
				else
				{
					MODE_60_Full_0( ms, outstr );
//					printf( "%s:%04d: 60_0 : Unsupported EA mode $%02x at %08x\n", __FILE__, __LINE__, mode, ms->ms_MemoryAdr );
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
					printf( "%s:%04d: 73_1 : Unsupported EA mode $%02x at %08x\n", __FILE__, __LINE__, mode, ms->ms_MemoryAdr );
					ms->ms_DecodeStatus = MSTAT_Error;
				}
				else
				{
					MODE_73_Full_0( ms, outstr );
//					printf( "%s:%04d: 73_0 : Unsupported EA mode $%02x at %08x\n", __FILE__, __LINE__, mode, ms->ms_MemoryAdr );
//					ms->ms_DecodeStatus = MSTAT_Error;
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
			ms->ms_DecodeStatus = MSTAT_Error;
			break;
		}
	}

	return( RefHandled );
}
