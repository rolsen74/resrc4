
/*
** Copyright (c) 2014-2025 Rene W. Olsen
**
** SPDX-License-Identifier: GPL-3.0-or-later
**
** This software is released under the GNU General Public License, version 3.
** For the full text of the license, please visit:
** https://www.gnu.org/licenses/gpl-3.0.html
**
** You can also find a copy of the license in the LICENSE file included with this software.
*/

// --

#include "Resourcer/ReSrc4.h"

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

enum RS4DecodeStat M68kCmd_FBcc( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Label *rl;
S64 adr;
MEM mem;
CHR labname[ MAX_LabelName + 8 ];
S32 size;
S32 cond;

	mem  = rt->rt_CurMemBuf;
	size = ( rt->rt_CPU.M68k.mt_Opcode & 0x00400000 );
	cond = ( rt->rt_CPU.M68k.mt_Opcode & 0x003f0000 ) >> 16;

	if ( cond > 0x1f )
	{
		printf( "Unsupported 'FBcc' Opcode at $%08" PRIx64 "\n", rt->rt_CurMemAdr );
		ds = RS4DecodeStat_Error;
		goto bailout;
	}

	if ( size == 0 )
	{
		S16 off;

		static CSTR fbcc_RegNames[] = 
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

		rt->rt_Container.Hunk.ms_Str_Opcode = fbcc_RegNames[cond];

		off = (( mem[2] << 8 ) | ( mem[3] << 0 ));

		adr = rt->rt_CurMemAdr + 2 + off;

		rt->rt_CPU.M68k.mt_LastOpcode = ( cond == 0 ) ? TRUE : FALSE;
		rt->rt_CPU.M68k.mt_OpcodeSize = 4;

		rl = RS4AddLabel_Sec( & ec, rt->rt_Section, adr, RS4LabelType_Code );
	}
	else
	{
		S32 off;

		static CSTR fbcc_RegNames[] = 
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

		rt->rt_Container.Hunk.ms_Str_Opcode = fbcc_RegNames[cond];

		off = (( mem[2] << 24 ) | ( mem[3] << 16 ) | ( mem[4] << 8 ) | ( mem[5] << 0 ));

		adr = rt->rt_CurMemAdr + 2 + off;

		rt->rt_CPU.M68k.mt_LastOpcode = ( cond == 0 ) ? TRUE : FALSE;
		rt->rt_CPU.M68k.mt_OpcodeSize = 6;

		rl = RS4AddLabel_File( & ec, rt->rt_File, adr, RS4LabelType_Code, __FILE__ );
	}

	// --

	if ( rl )
	{
		rt->rt_CPU.M68k.mt_JmpRegister.mr_Type1 = RRT_Label;
		rt->rt_CPU.M68k.mt_JmpRegister.mr_Label = rl;
	}
	else
	{
		rt->rt_CPU.M68k.mt_JmpRegister.mr_Type1 = RRT_Address;
		rt->rt_CPU.M68k.mt_JmpRegister.mr_Address = adr;
	}

//	if (( rl ) && ( rl->rl_Name[0] ))
	if ( rl )
	{
		fs = RS4BuildLabelString( & ec, rl, labname );

		if ( fs != RS4FuncStat_Okay )
		{
			// ec allready set
			ds = RS4DecodeStat_Error;

			#ifdef DEBUG
			printf( "%s:%04d: Error '%s'\n", __FILE__, __LINE__, labname );
			#endif

			goto bailout;
		}

		sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "%s", labname );
	}
	else
	{
		sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "$%08" PRIx64, adr );
	}

	// --

	ds = RS4DecodeStat_Okay;
	ec = RS4ErrStat_Okay;

	// --

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( ds );
}
