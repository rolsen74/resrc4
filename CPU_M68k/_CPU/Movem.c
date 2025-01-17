
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

static enum RS4DecodeStat RegMask( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
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

	reverse = (( rt->rt_CPU.M68k.mt_Opcode & 0x00380000 ) == 0x00200000 ) ? TRUE : FALSE;

	mask = rt->rt_CPU.M68k.mt_Opcode & 0xffff;

	regs = Dx_RegNames;

	start = end = -1;

	reg = M68KREGT_Dx;

	pos = strlen( rt->rt_Container.Hunk.ms_Buf_Argument );

	if ( rt->rt_CPU.M68k.mt_Opcode & 0x04000000 )
	{
		sprintf( & rt->rt_Container.Hunk.ms_Buf_Argument[ pos ], "," );
		pos++;

		ClearReg = TRUE;
	}
	else
	{
		ClearReg = FALSE;
	}

	buf = & rt->rt_Container.Hunk.ms_Buf_Argument[pos];

	bit = ( ! reverse ) ? 0x0001 : 0x8000;

	for( loop=0 ; loop<2 ; loop++ )
	{
		for( cnt=0 ; cnt<9 ; cnt++ )
		{
			if (( mask & bit ) && ( cnt != 8 ))
			{
				if ( ClearReg )
				{
					rt->rt_CPU.M68k.mt_ClearRegMask |= 1U << ( reg + cnt ); 
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
		reg = M68KREGT_Ax;
	}

	// --

	ds = RS4DecodeStat_Okay;
	ec = RS4ErrStat_Okay;

//bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	return( ds );
}

// --

enum RS4DecodeStat M68kCmd_MOVEM( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;

	rt->rt_CPU.M68k.mt_ArgSize 	= 4;
	rt->rt_CPU.M68k.mt_ArgEMode	= ( rt->rt_CPU.M68k.mt_Opcode & 0x00380000 ) >> 19;
	rt->rt_CPU.M68k.mt_ArgEReg	= ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;

	rt->rt_CPU.M68k.mt_CurRegister = & rt->rt_CPU.M68k.mt_SrcRegister;

	if ( rt->rt_CPU.M68k.mt_Opcode & 0x00400000 )
	{
		rt->rt_Container.Hunk.ms_Str_Opcode = "Movem.l";
		rt->rt_CPU.M68k.mt_ArgType = M68KSIZE_Long;
	}
	else
	{
		rt->rt_Container.Hunk.ms_Str_Opcode = "Movem.w";
		rt->rt_CPU.M68k.mt_ArgType = M68KSIZE_Word;
	}

	if ( rt->rt_CPU.M68k.mt_Opcode & 0x04000000 )
	{
		ds = M68k_EffectiveAddress( & ec, rt );

		if ( ds != RS4DecodeStat_Okay )
		{
			// ec allready set

			#ifdef DEBUG
			printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
			#endif

			goto bailout;
		}

		ds = RegMask( & ec, rt );

		if ( ds != RS4DecodeStat_Okay )
		{
			// ec allready set

			#ifdef DEBUG
			printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
			#endif

			goto bailout;
		}
	}
	else
	{
		ds = RegMask( & ec, rt );

		if ( ds != RS4DecodeStat_Okay )
		{
			// ec allready set

			#ifdef DEBUG
			printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
			#endif

			goto bailout;
		}

		ds = M68k_EffectiveAddress( & ec, rt );

		if ( ds != RS4DecodeStat_Okay )
		{
			// ec allready set

			#ifdef DEBUG
			printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
			#endif

			goto bailout;
		}
	}

	rt->rt_CPU.M68k.mt_OpcodeSize = rt->rt_CPU.M68k.mt_ArgSize;

	// --

	ds = RS4DecodeStat_Okay;
	ec = RS4ErrStat_Okay;

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	return( ds );
}
