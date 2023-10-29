
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

void Cmd_Bcc( struct M68kStruct *ms )
{
struct HunkLabel *parent;
struct HunkLabel *hl;
uint32_t cond;
uint32_t size;
uint32_t adr;
uint8_t *mem;

	mem = ms->ms_MemoryBuf;

	cond = ( ms->ms_Opcode & 0x0f000000 ) >> 24;
	size = ( ms->ms_Opcode & 0x00ff0000 ) >> 16;

	if ( cond == 1 ) // BSR
	{
		ms->ms_ClearRegMask = -1;
	}

	switch( size )
	{
		case 0:
		{
			int16_t offset;

			static const char *bcc_RegNames[16] = 
			{
				"Bra.w", "Bsr.w", "Bhi.w", "Bls.w",
				"Bcc.w", "Bcs.w", "Bne.w", "Beq.w",
				"Bvc.w", "Bvs.w", "Bpl.w", "Bmi.w",
				"Bge.w", "Blt.w", "Bgt.w", "Ble.w"
			};

			ms->ms_Str_Opcode = bcc_RegNames[cond];

			offset = (( mem[2] << 8 ) + ( mem[3] << 0 ));

			adr = ms->ms_MemoryAdr + 2 + offset;

			ms->ms_LastOpcode = ( cond == 0 ) ? true : false;
			ms->ms_OpcodeSize = 4;
			break;
		}

		case 255:
		{
			int32_t offset;

			static const char *bcc_RegNames[] = 
			{
				"Bra.l", "Bsr.l", "Bhi.l", "Bls.l",
				"Bcc.l", "Bcs.l", "Bne.l", "Beq.l",
				"Bvc.l", "Bvs.l", "Bpl.l", "Bmi.l",
				"Bge.l", "Blt.l", "Bgt.l", "Ble.l"
			};

			ms->ms_Str_Opcode = bcc_RegNames[cond];

			offset = (( mem[2] << 24 ) + ( mem[3] << 16 )) + (( mem[4] << 8 ) + ( mem[5] << 0 ));

			adr = ms->ms_MemoryAdr + 2 + offset;

			ms->ms_LastOpcode = ( cond == 0 ) ? true : false;
			ms->ms_OpcodeSize = 6;
			break;
		}

		default:
		{
			int8_t offset;

			static const char *bcc_RegNames[] = 
			{
				"Bra.b", "Bsr.b", "Bhi.b", "Bls.b",
				"Bcc.b", "Bcs.b", "Bne.b", "Beq.b",
				"Bvc.b", "Bvs.b", "Bpl.b", "Bmi.b",
				"Bge.b", "Blt.b", "Bgt.b", "Ble.b"
			};

			ms->ms_Str_Opcode = bcc_RegNames[cond];

			offset = ( ms->ms_MemoryBuf[1] );

			adr = ms->ms_MemoryAdr + 2 + offset;

			ms->ms_LastOpcode = ( cond == 0 ) ? true : false;
			ms->ms_OpcodeSize = 2;
			break;
		}
	}

	// --

//	hl = Hunk_AddLabel( ms->ms_HunkStruct, adr, LT_Unknown );
	hl = Hunk_AddLabel2( ms->ms_HunkStruct, ms->ms_HunkNode, adr, LT_Code );

	if ( hl )
	{
		ms->ms_JmpRegister.mr_Type = RT_Label;
		ms->ms_JmpRegister.mr_LabelNode = hl;
	}
	else
	{
		ms->ms_JmpRegister.mr_Type = RT_Address;
		ms->ms_JmpRegister.mr_Address = adr;
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
			sprintf( ms->ms_Buf_Argument, "%s", hl->hl_Label_Name );
		}
	}
	else
	{
		sprintf( ms->ms_Buf_Argument, "$%08x", adr );
	}
}
