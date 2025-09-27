
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

#include "Resourcer/ReSrc4.h"

// --

enum RS4DecodeStat M68kCmd_Bcc( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Label *rl;
U32 cond;
U32 size;
MEM mem;
S64 adr;

	mem = rt->rt_CurMemBuf;

	rt->rt_CPU.M68k.mt_DoLabelSize	= FALSE;

	cond = ( rt->rt_CPU.M68k.mt_Opcode & 0x0f000000 ) >> 24;
	size = ( rt->rt_CPU.M68k.mt_Opcode & 0x00ff0000 ) >> 16;

	if ( cond == 1 ) // BSR
	{
		rt->rt_CPU.M68k.mt_ClearRegMask = -1;
	}

	switch( size )
	{
		case 0:
		{
			S16 offset;

			static CSTR bcc_RegNames[16] = 
			{
				"Bra.w", "Bsr.w", "Bhi.w", "Bls.w",
				"Bcc.w", "Bcs.w", "Bne.w", "Beq.w",
				"Bvc.w", "Bvs.w", "Bpl.w", "Bmi.w",
				"Bge.w", "Blt.w", "Bgt.w", "Ble.w"
			};

			rt->rt_Container.Hunk.ms_Str_Opcode = bcc_RegNames[cond];

			offset = (( mem[2] << 8 ) | ( mem[3] << 0 ));

			adr = rt->rt_CurMemAdr + 2 + offset;

			rt->rt_CPU.M68k.mt_LastOpcode = ( cond == 0 ) ? TRUE : FALSE;
			rt->rt_CPU.M68k.mt_OpcodeSize = 4;

			rl = RS4AddLabel_Sec( & ec, rt->rt_Section, adr, RS4LabelType_Code );
			break;
		}

		case 255:
		{
			S32 offset;

			static CSTR bcc_RegNames[] = 
			{
				"Bra.l", "Bsr.l", "Bhi.l", "Bls.l",
				"Bcc.l", "Bcs.l", "Bne.l", "Beq.l",
				"Bvc.l", "Bvs.l", "Bpl.l", "Bmi.l",
				"Bge.l", "Blt.l", "Bgt.l", "Ble.l"
			};

			rt->rt_Container.Hunk.ms_Str_Opcode = bcc_RegNames[cond];

			offset = (( mem[2] << 24 ) | ( mem[3] << 16 ) | ( mem[4] << 8 ) | ( mem[5] << 0 ));

			adr = rt->rt_CurMemAdr + 2 + offset;

			rt->rt_CPU.M68k.mt_LastOpcode = ( cond == 0 ) ? TRUE : FALSE;
			rt->rt_CPU.M68k.mt_OpcodeSize = 6;

			rl = RS4AddLabel_File( & ec, rt->rt_File, adr, RS4LabelType_Code );
			break;
		}

		default:
		{
			S8 offset;

			static CSTR bcc_RegNames[] = 
			{
				"Bra.b", "Bsr.b", "Bhi.b", "Bls.b",
				"Bcc.b", "Bcs.b", "Bne.b", "Beq.b",
				"Bvc.b", "Bvs.b", "Bpl.b", "Bmi.b",
				"Bge.b", "Blt.b", "Bgt.b", "Ble.b"
			};

			rt->rt_Container.Hunk.ms_Str_Opcode = bcc_RegNames[cond];

			offset = ( rt->rt_CurMemBuf[1] );

			adr = rt->rt_CurMemAdr + 2 + offset;

			rt->rt_CPU.M68k.mt_LastOpcode = ( cond == 0 ) ? TRUE : FALSE;
			rt->rt_CPU.M68k.mt_OpcodeSize = 2;

			rl = RS4AddLabel_Sec( & ec, rt->rt_Section, adr, RS4LabelType_Code );
			break;
		}
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

	if ( rl )
	{
		fs = RS4BuildLabelString( & ec, rl, rt->rt_Container.Hunk.ms_Buf_Argument );

		if ( fs != RS4FuncStat_Okay )
		{
			// ec allready set
			ds = RS4DecodeStat_Error;

			#ifdef DEBUG
			printf( "%s:%04d: Error '%s'\n", __FILE__, __LINE__, rt->rt_Container.Hunk.ms_Buf_Argument );
			#endif

			goto bailout;
		}
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
