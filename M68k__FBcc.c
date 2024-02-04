
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

//  0 000000 F
//  1 000001 EQ
//  2 000010 OGT
//  3 000011 OGE
//  4 000100 OLT
//  5 000101 OLE
//  6 000110 OGL
//  7 000111 OR
//  8 001000 UN
//  9 001001 UEQ
//  A 001010 UGT
//  B 001011 UGE
//  C 001100 ULT
//  D 001101 ULE
//  E 001110 NE
//  F 001111 T
// 10 010000 SF
// 11 010001 SEQ
// 12 010010 GT
// 13 010011 GE
// 14 010100 LT
// 15 010101 LE
// 16 010110 GL
// 17 010111 GLE
// 18 011000 NGLE
// 19 011001 NGL
// 1A 011010 NLE
// 1B 011011 NLT
// 1C 011100 NGE
// 1D 011101 NGT
// 1E 011110 SNE
// 1F 011111 ST

void Cmd_FBcc( struct M68kStruct *ms )
{
struct HunkLabel *parent;
struct HunkLabel *hl;
int32_t adr;
uint8_t *mem;
int size;
int cond;

	mem  = ms->ms_MemoryBuf;
	size = ( ms->ms_Opcode & 0x00400000 );
	cond = ( ms->ms_Opcode & 0x003f0000 ) >> 16;

	if ( cond > 0x1f )
	{
		printf( "Unsupported 'FBcc' Opcode at %08x\n", ms->ms_MemoryAdr );
		ms->ms_DecodeStatus = MSTAT_Error;
		goto bailout;
	}

	if ( size == 0 )
	{
		int16_t off;

		static const char *fbcc_RegNames[] = 
		{
			"FBf.w",	"FBeq.w",	"FBogt.w",	"FBoge.w",	// 0x00
			"FBolt.w",	"FBole.w",	"FBogl.w",	"FBor.w",	// 0x04
			"FBun.w",	"FBueq.w",	"FBugt.w",	"FBuge.w",	// 0x08
			"FBult.w",	"FBule.w",	"FBne.w",	"FBt.w",	// 0x0c
			"FBf.w",	"FBseq.w",	"FBgt.w",	"FBge.w",	// 0x10
			"FBlt.w",	"FBle.w",	"FBgl.w",	"FBgle.w",	// 0x14
			"FBngle.w",	"FBngl.w",	"FBnle.w",	"FBnlt.w",	// 0x18
			"FBnge.w",	"FBngt.w",	"FBsne.w",	"FBst.w"	// 0x1c
		};

		ms->ms_Str_Opcode = fbcc_RegNames[cond];

		off = (( mem[2] << 8 ) | ( mem[3] << 0 ));

		adr = ms->ms_MemoryAdr + 2 + off;

//		ms->ms_LastOpcode = ( cond == 0 ) ? true : false;
		ms->ms_OpcodeSize = 4;
	}
	else
	{
		int32_t off;

		static const char *fbcc_RegNames[] = 
		{
			"FBf.l",	"FBeq.l",	"FBogt.l",	"FBoge.l",
			"FBolt.l",	"FBole.l",	"FBogl.l",	"FBor.l",
			"FBun.l",	"FBueq.l",	"FBugt.l",	"FBuge.l",
			"FBult.l",	"FBule.l",	"FBne.l",	"FBt.l",
			"FBf.l",	"FBseq.l",	"FBgt.l",	"FBge.l",
			"FBlt.l",	"FBle.l",	"FBgl.l",	"FBgle.l",
			"FBngle.l",	"FBngl.l",	"FBnle.l",	"FBnlt.l",
			"FBnge.l",	"FBngt.l",	"FBsne.l",	"FBst.l"
		};

		ms->ms_Str_Opcode = fbcc_RegNames[cond];

		off = (( mem[2] << 24 ) | ( mem[3] << 16 ) | ( mem[4] << 8 ) | ( mem[5] << 0 ));

		adr = ms->ms_MemoryAdr + 2 + off;

//		ms->ms_LastOpcode = ( cond == 0 ) ? true : false;
		ms->ms_OpcodeSize = 6;
	}

	// --

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

	// --

bailout:

	return;
}

// --
