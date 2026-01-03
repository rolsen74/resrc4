
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

enum RS4DecodeStat M68kCmd_TRAPcc( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
CHR labname[ MAX_LabelName + 8 ];
MEM mem;
S32 cond;
S32 size;

	ec	= RS4ErrStat_Error;
	ds	= RS4DecodeStat_Error;
	mem = rt->rt_CurMemBuf;

	cond = ( rt->rt_CPU.M68k.mt_Opcode & 0x0f000000 ) >> 24;
	size = ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;

	switch( size )
	{
		case 2: // Trapcc.w
		{
			U16 val;

			val = (( mem[2] << 8 ) | ( mem[3] << 0 ));

			static CSTR trapcc_RegNames[16] =
			{
				"Trapt.w",  "Trapf.w",  "Traphi.w", "Trapls.w",
				"Trapcc.w", "Trapcs.w", "Trapne.w", "Trapeq.w",
				"Trapvc.w", "Trapvs.w", "Trappl.w", "Trapmi.w",
				"Trapge.w", "Traplt.w", "Trapgt.w", "Traple.w"
			};

			rt->rt_Container.Hunk.ms_Str_Opcode = trapcc_RegNames[cond];
			rt->rt_CPU.M68k.mt_OpcodeSize = 4;
			rt->rt_CPU.M68k.mt_ArgType  = M68KSIZE_Word;

			snprintf( rt->rt_Container.Hunk.ms_Buf_Argument, 64, "#$%04x", val );
			break;
		}

		case 3: // Trapcc.l
		{
			U32 val;

			val = (( mem[2] << 24 ) | ( mem[3] << 16 ) | ( mem[4] << 8 ) | ( mem[5] << 0 ));

			static CSTR trapcc_RegNames[16] =
			{
				"Trapt.l",  "Trapf.l",  "Traphi.l", "Trapls.l",
				"Trapcc.l", "Trapcs.l", "Trapne.l", "Trapeq.l",
				"Trapvc.l", "Trapvs.l", "Trappl.l", "Trapmi.l",
				"Trapge.l", "Traplt.l", "Trapgt.l", "Traple.l"
			};

			rt->rt_Container.Hunk.ms_Str_Opcode = trapcc_RegNames[cond];
			rt->rt_CPU.M68k.mt_OpcodeSize = 6;
			rt->rt_CPU.M68k.mt_ArgType  = M68KSIZE_Long;

			ERR_CHK( RS4BuildLabelString2( & ec, rt, labname, rt->rt_CurMemAdr + rt->rt_CPU.M68k.mt_ArgSize, val ))

			snprintf( rt->rt_Container.Hunk.ms_Buf_Argument, 64, "#%s", labname );
			break;
		}

		case 4: // Trapcc
		{
			static CSTR trapcc_RegNames[16] =
			{
				"Trapt",  "Trapf",  "Traphi", "Trapls",
				"Trapcc", "Trapcs", "Trapne", "Trapeq",
				"Trapvc", "Trapvs", "Trappl", "Trapmi",
				"Trapge", "Traplt", "Trapgt", "Traple"
			};

			rt->rt_Container.Hunk.ms_Str_Opcode = trapcc_RegNames[cond];
			rt->rt_CPU.M68k.mt_OpcodeSize = 2;
			break;
		}

		default:
		{
			printf( "Unsupported 'Trapcc' Opcode (Mode: %d)\n", size );
			ds = RS4DecodeStat_Error;
			goto bailout;
		}
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
