
/*
 * Copyright (c) 2014-2023 Rene W. Olsen < renewolsen @ gmail . com >
 * All rights reserved.
 *
 */

// --

#include "ReSrc4.h"

// --

enum
{
	_Unknown = 0,
	_Reg,
	_Exec,
	_Label,
};

struct _GetSetStruct
{
	int					Type;
	int					LibType;
	struct HunkLabel *	Label;
	struct M68kRegister Reg;
};

inline static void _Get( struct M68kStruct *ms, struct M68kRegister *cur, uint8_t *mem, struct _GetSetStruct *gss )
{
struct HunkLabel *hl;
int32_t val;

	memset( gss, 0, sizeof( struct _GetSetStruct ));

	// --

	/**/ if (( ms->ms_ArgEMode == 7 ) && ( ms->ms_ArgEReg == 0 ))	// Move.w $0004.w
	{
		gss->Type = _Exec;
//printf( "A> %08x : Get Exec.w\n", ms->ms_MemoryAdr );
	}
	else if (( ms->ms_ArgEMode == 7 ) && ( ms->ms_ArgEReg == 1 ))	// Move.l $00000004
	{
		val = (( mem[0] << 24 ) | ( mem[1] << 16 ) | ( mem[2] << 8 ) | ( mem[3] << 0 ));

		if ( val == 4 )
		{
			gss->Type = _Exec;
//printf( "A> %08x : Get Exec.l\n", ms->ms_MemoryAdr );
		}
		else
		{
			hl = Hunk_FindLabel( ms->ms_HunkStruct, val );

			if ( hl )
			{
//printf( "A> %08x : Get Label\n", ms->ms_MemoryAdr );
				gss->Type = _Label;
				gss->Label = hl;
			}
		}
	}
	else
	{
//printf( "A> %08x : Get Reg %d\n", ms->ms_MemoryAdr, cur->mr_Number );
		gss->Type = _Reg;
		gss->Reg = *cur;
	}

	// --
}

inline static void _Set( struct M68kStruct *ms, struct M68kRegister *cur, uint8_t *mem, struct _GetSetStruct *gss )
{
struct HunkLabel *src;
struct HunkLabel *hl;
int32_t val;

	// --

	/**/ if (( ms->ms_ArgEMode == 7 ) && ( ms->ms_ArgEReg == 0 ))	// Move.w $0004.w
	{
		// We don't move to External Address
	}
	else if (( ms->ms_ArgEMode == 7 ) && ( ms->ms_ArgEReg == 1 ))	// Move.l $00000004
	{
		val = (( mem[0] << 24 ) | ( mem[1] << 16 ) | ( mem[2] << 8 ) | ( mem[3] << 0 ));

		if ( val != 4 )
		{
			hl = Hunk_FindLabel( ms->ms_HunkStruct, val );
		}
		else
		{
			hl = NULL;
		}

		if ( hl )
		{
//printf( "A> %08x : Set Label : ", ms->ms_MemoryAdr );

			switch( gss->Type ) // Set xx -> Label
			{
				case _Exec:
				{
//printf( "from _Exec\n" );
					if ( hl->hl_Label_Type == LT_Unknown )
					{
						hl->hl_Label_Type = LT_Pointer;
						hl->hl_Label_SubType = LPT_Library + ( LIBT_ExecBase << 16 );
					}
					else if ( hl->hl_Label_Type == LT_Pointer )
					{
						if ( hl->hl_Label_SubType != LPT_Library + ( LIBT_ExecBase << 16 ) )
						{
							hl->hl_Label_SubType = LPT_Error;
						}
						#if 0
						else
						{
							// Allready Set
						}
						#endif
					}
					else
					{
						printf( "%s:%04d: Not a Pointer Type\n", __FILE__, __LINE__ );
						// Not a Pointer ... 
					}
					break;
				}

				case _Label:
				{
//printf( "from _Label\n" );

					src = gss->Label;

					if ( src == NULL )
					{
						printf( "%s:%04d: NULL Pointer\n", __FILE__, __LINE__ );
						break;
					}

					if ( hl->hl_Label_Type == LT_Unknown )
					{
						hl->hl_Label_Type = src->hl_Label_Type;
						hl->hl_Label_SubType = src->hl_Label_SubType;
					}
					else if ( hl->hl_Label_Type == LT_Pointer )
					{
						if ( hl->hl_Label_SubType != src->hl_Label_SubType )
						{
							hl->hl_Label_SubType = LPT_Error;
						}
						#if 0
						else
						{
							// Allready Set
						}
						#endif
					}
					else
					{
						printf( "%s:%04d: Not a Pointer Type\n", __FILE__, __LINE__ );
						// Not a Pointer ... 
					}
					break;
				}

				case _Reg:
				{
//printf( "from _Reg %d : ", gss->Reg.mr_Number );

					if (( gss->Reg.mr_Type & 0xffff ) == RT_Library )
					{
//printf( " 22\n" );
						hl->hl_Label_Type = LT_Pointer;
						hl->hl_Label_SubType = LPT_Library + ( gss->Reg.mr_Type & 0xffff0000 );
					}
					else
					{
//printf( " 33\n" );
						hl->hl_Label_Type = LT_Pointer;
						hl->hl_Label_SubType = LPT_Error;
					}
					break;
				}

				default:
				{
printf( "from Unknown\n" );
					hl->hl_Label_Type = LT_Pointer;
					hl->hl_Label_SubType = LPT_Error;
					break;
				}
			}
		}
	}
	else
	{
//printf( "A> %08x : Set Reg %d : ", ms->ms_MemoryAdr, cur->mr_Number );

		switch( gss->Type ) // Set xx -> Reg
		{
			case _Reg:
			{
//printf( "from _Reg\n" );
				*cur = gss->Reg;
				break;
			}

			case _Exec:
			{
//printf( "from _Exec\n" );
				cur->mr_Type = RT_Library + ( LIBT_ExecBase << 16 );
				cur->mr_Address = 0;
				cur->mr_LabelNode = NULL;
				break;
			}

			case _Label:
			{
				cur->mr_Type = RT_Unknown;

//printf( "from _Label\n" );

				hl = gss->Label;

				if ( hl )
				{
					if (( hl->hl_Label_Type == LT_Pointer ) 
					&& (( hl->hl_Label_SubType & 0xffff ) == LPT_Library ))
					{
						cur->mr_Type = RT_Library + ( hl->hl_Label_SubType & 0xffff0000 );
						cur->mr_Address = 0;
						cur->mr_LabelNode = NULL;
					}
				}
				break;
			}

			default:
			{
printf( "from Unknown\n" );
				cur->mr_Type = RT_Unknown;
				break;
			}
		}
	}

	// --

//	printf( "\n" );
}

// --

void Cmd_MOVEA( struct M68kStruct *ms )
{
struct _GetSetStruct gss;
struct HunkRef *isRef;
uint32_t size;
uint8_t *mem;

	size = ( ms->ms_Opcode & 0x30000000 ) >> 28;

	switch( size )
	{
		case 3:
		{
			ms->ms_Str_Opcode = ( ShortOpcodes ) ? "Move.w" : "Movea.w" ;
			ms->ms_ArgType	= OS_Word;
			ms->ms_ArgEMode = ( ms->ms_Opcode & 0x00380000 ) >> 19;
			ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x00070000 ) >> 16;

			ms->ms_CurRegister = & ms->ms_SrcRegister;

			isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

			if ( M68k_EffectiveAddress( ms, isRef, 0 ))
			{
				isRef->hr_Used = true;
			}

			ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x0e000000 ) >> 25;	
			ms->ms_ArgEMode = ( ms->ms_Opcode & 0x01c00000 ) >> 22;

			ms->ms_CurRegister = & ms->ms_DstRegister;

			isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

			if ( M68k_EffectiveAddress( ms, isRef, 0 ))
			{
				isRef->hr_Used = true;
			}

			ms->ms_CurRegister->mr_Type = RT_Unknown;
			ms->ms_OpcodeSize = ms->ms_ArgSize;
			break;
		}

		case 2:
		{
			ms->ms_Str_Opcode = ( ShortOpcodes ) ? "Move.l" : "Movea.l" ;
			ms->ms_ArgType  = OS_Long;
			ms->ms_ArgEMode = ( ms->ms_Opcode & 0x00380000 ) >> 19;
			ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x00070000 ) >> 16;

			// --

//			for( int ii=0 ; ii<16 ; ii++ )
//			{
//				ms->ms_Registers[ii].mr_Number = ii;
//			}
//			ms->ms_SrcRegister.mr_Number = 100;
//			ms->ms_DstRegister.mr_Number = 200;
//			ms->ms_JmpRegister.mr_Number = 300;

			// --

			ms->ms_CurRegister = & ms->ms_SrcRegister;

			mem = & ms->ms_MemoryBuf[ ms->ms_ArgSize ];

			isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

			if ( M68k_EffectiveAddress( ms, isRef, 0 ))
			{
				isRef->hr_Used = true;
			}

			_Get( ms, ms->ms_CurRegister, mem, & gss );

			// --

			ms->ms_ArgEReg  = ( ms->ms_Opcode & 0x0e000000 ) >> 25;
			ms->ms_ArgEMode = ( ms->ms_Opcode & 0x01c00000 ) >> 22;

			ms->ms_CurRegister = & ms->ms_DstRegister;

			mem = & ms->ms_MemoryBuf[ ms->ms_ArgSize ];

			isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

			if ( M68k_EffectiveAddress( ms, isRef, 0 ))
			{
				isRef->hr_Used = true;
			}

			_Set( ms, ms->ms_CurRegister, mem, & gss );

			// --

			ms->ms_OpcodeSize = ms->ms_ArgSize;
			break;
		}

		default:
		{
			printf( "Unsupported 'Movea' Opcode (Mode: %d)\n", size );
			ms->ms_DecodeStatus = MSTAT_Error;
			break;
		}
	}
}
