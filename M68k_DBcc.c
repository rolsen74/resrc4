
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

// --

void Cmd_DBcc( struct M68kStruct *ms )
{
struct HunkLabel *parent;
struct HunkLabel *hl;
uint32_t cond;
uint32_t reg;
int32_t adr;
int16_t offset;
uint8_t *mem;

	static const char *dbcc_RegNames[16] =
	{
		"DBt.w",  "DBf.w",  "DBhi.w", "DBls.w",
		"DBcc.w", "DBcs.w", "DBne.w", "DBeq.w",
		"DBvc.w", "DBvs.w", "DBpl.w", "DBmi.w",
		"DBge.w", "DBlt.w", "DBgt.w", "DBle.w"
	};

	cond = ( ms->ms_Opcode & 0x0f000000 ) >> 24;

	ms->ms_Str_Opcode = dbcc_RegNames[cond];

	reg  = ( ms->ms_Opcode & 0x00070000 ) >> 16;

    // --

	mem = ms->ms_MemoryBuf;

	offset = (( mem[2] << 8 ) + ( mem[3] << 0 ));

	adr = ms->ms_MemoryAdr + 2 + offset;

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
				sprintf( ms->ms_Buf_Argument, "%s,%s%d", Dx_RegNames[reg], parent->hl_Label_Name, off );
			}
			else
			{
				sprintf( ms->ms_Buf_Argument, "%s,%s+%d", Dx_RegNames[reg], parent->hl_Label_Name, off );
			}
		}
		else
		{
			sprintf( ms->ms_Buf_Argument, "%s,%s", Dx_RegNames[reg], hl->hl_Label_Name );
		}
	}
	else
	{
		sprintf( ms->ms_Buf_Argument, "%s,$%08x", Dx_RegNames[reg], adr );
	}

	// --

	ms->ms_Registers[REG_Dx+reg].mr_Type = RT_Unknown;
	ms->ms_OpcodeSize = 4;
}
