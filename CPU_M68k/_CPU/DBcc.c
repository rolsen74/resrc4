
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

enum RS4DecodeStat M68kCmd_DBcc( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Label *rl;
uint32_t cond;
uint32_t reg;
int32_t adr;
int16_t offset;
uint8_t *mem;
char labname[ MAX_LabelName + 8 ];

	static const char *dbcc_RegNames[16] =
	{
		"DBt.w",  "DBf.w",  "DBhi.w", "DBls.w",
		"DBcc.w", "DBcs.w", "DBne.w", "DBeq.w",
		"DBvc.w", "DBvs.w", "DBpl.w", "DBmi.w",
		"DBge.w", "DBlt.w", "DBgt.w", "DBle.w"
	};

	cond = ( rt->rt_CPU.M68k.mt_Opcode & 0x0f000000 ) >> 24;

	rt->rt_Container.Hunk.ms_Str_Opcode = dbcc_RegNames[cond];

	reg  = ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;

    // --

	mem = rt->rt_CurMemBuf;

	offset = (( mem[2] << 8 ) | ( mem[3] << 0 ));

	adr = rt->rt_CurMemAdr + 2 + offset;

	// --

//	rl = RS4AddLabel_File( rt->rt_File, adr, RS4LabelType_Unset );
	rl = RS4AddLabel_Sec( & ec, rt->rt_Section, adr, RS4LabelType_Code );

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

		sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "%s,%s", Dx_RegNames[reg], labname );
	}
	else
	{
		sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "%s,$%08x", Dx_RegNames[reg], adr );
	}

	// --

	rt->rt_CPU.M68k.mt_ClearRegMask |= 1U << ( M68KREGT_D0 + reg ); 
	rt->rt_CPU.M68k.mt_OpcodeSize = 4;

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
