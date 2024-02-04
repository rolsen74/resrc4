
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

static void RegMask( struct M68kStruct *ms )
{
const char **regs;
char *buf;
uint32_t mask;
uint32_t pos;
uint32_t bit;
int ClearReg;
int reverse;
int start;
int loop;
int end;
int cnt;
int reg;

	reverse = (( ms->ms_Opcode & 0x00380000 ) == 0x00200000 ) ? true : false;

	mask = ms->ms_Opcode & 0xffff;

	regs = Dx_RegNames;

	start = end = -1;

	reg = REG_Dx;

	pos = strlen( ms->ms_Buf_Argument );

	if ( ms->ms_Opcode & 0x04000000 )
	{
		sprintf( & ms->ms_Buf_Argument[ pos ], "," );
		pos++;

		ClearReg = true;
	}
	else
	{
		ClearReg = false;
	}

	buf = & ms->ms_Buf_Argument[pos];

	bit = ( ! reverse ) ? 0x0001 : 0x8000;

	for( loop=0 ; loop<2 ; loop++ )
	{
		for( cnt=0 ; cnt<9 ; cnt++ )
		{
			if (( mask & bit ) && ( cnt != 8 ))
			{
				if ( ClearReg )
				{
					ms->ms_Registers[ reg + cnt ].mr_Type = RT_Unknown;
				}

				if ( start == -1 )
				{
					start = end = cnt;
				}
				else
				{
					end = cnt;
				}
			}
			else
			{
				if ( start != -1 )	
				{
					pos = strlen( buf );

					if ( pos )
					{
						buf[pos++] = '/';
					}

					if ( start == end )
					{
						sprintf( &buf[pos], "%s", regs[start] );
					}
					else
					{
						sprintf( &buf[pos], "%s-%s", regs[start], regs[end] );
					}

					start = -1;
				}
			}

			if ( cnt != 8 )
			{
				bit = ( ! reverse ) ? bit * 2 : bit / 2;
			}
		}

		regs = Ax_RegNames;
		reg = REG_Ax;
	}
}

// --

void Cmd_MOVEM( struct M68kStruct *ms )
{
struct HunkRef *isRef;

	ms->ms_ArgSize 	= 4;
	ms->ms_ArgEMode	= ( ms->ms_Opcode & 0x00380000 ) >> 19;
	ms->ms_ArgEReg	= ( ms->ms_Opcode & 0x00070000 ) >> 16;

	ms->ms_CurRegister = & ms->ms_SrcRegister;

	if ( ms->ms_Opcode & 0x00400000 )
	{
		ms->ms_Str_Opcode = "Movem.l";
		ms->ms_ArgType = OS_Long;
	}
	else
	{
		ms->ms_Str_Opcode = "Movem.w";
		ms->ms_ArgType = OS_Word;
	}

	if ( ms->ms_Opcode & 0x04000000 )
	{
		isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

		if ( M68k_EffectiveAddress( ms, isRef, 0 ))
		{
			isRef->hr_Used = true;
		}

		RegMask( ms );
	}
	else
	{
		RegMask( ms );

		isRef = Hunk_FindRef( ms->ms_HunkNode, ms->ms_MemoryAdr + ms->ms_ArgSize );

		if ( M68k_EffectiveAddress( ms, isRef, 0 ))
		{
			isRef->hr_Used = true;
		}
	}

	ms->ms_OpcodeSize = ms->ms_ArgSize;
}
