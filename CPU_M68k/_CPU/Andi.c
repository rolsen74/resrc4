 
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

#include "Resourcer/ReSrc4.h"

// --

enum RS4DecodeStat M68kCmd_ANDI( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
int size;

	size = ( rt->rt_CPU.M68k.mt_Opcode & 0x00c00000 ) >> 22;

	switch( size )
	{
		case 0:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = ( ShortOpcodes ) ? "And.b" : "Andi.b" ;

			// --

			rt->rt_CPU.M68k.mt_ArgType  = M68KSIZE_Byte;
			rt->rt_CPU.M68k.mt_ArgEMode = 7;
			rt->rt_CPU.M68k.mt_ArgEReg  = 4;

			rt->rt_CPU.M68k.mt_CurRegister = & rt->rt_CPU.M68k.mt_SrcRegister;

			ds = M68k_EffectiveAddress( & ec, rt );

			if ( ds != RS4DecodeStat_Okay )
			{
				// ec allready set

				#ifdef DEBUG
				printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
				#endif

				goto bailout;
			}

			// --

			rt->rt_CPU.M68k.mt_ArgEMode = ( rt->rt_CPU.M68k.mt_Opcode & 0x00380000 ) >> 19;
			rt->rt_CPU.M68k.mt_ArgEReg  = ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;

			rt->rt_CPU.M68k.mt_CurRegister = & rt->rt_CPU.M68k.mt_DstRegister;

			ds = M68k_EffectiveAddress( & ec, rt );

			if ( ds != RS4DecodeStat_Okay )
			{
				// ec allready set

				#ifdef DEBUG
				printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
				#endif

				goto bailout;
			}

			// --

			M68k_Set_Cur_to_Unknown( rt );
//			rt->rt_CPU.M68k.mt_CurRegister->mr_Type1 = RRT_Unknown;
			rt->rt_CPU.M68k.mt_OpcodeSize = rt->rt_CPU.M68k.mt_ArgSize;
			break;
		}

		case 1:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = ( ShortOpcodes ) ? "And.w" : "Andi.w" ;

			// --

			rt->rt_CPU.M68k.mt_ArgType  = M68KSIZE_Word;
			rt->rt_CPU.M68k.mt_ArgEMode = 7;
			rt->rt_CPU.M68k.mt_ArgEReg  = 4;

			rt->rt_CPU.M68k.mt_CurRegister = & rt->rt_CPU.M68k.mt_SrcRegister;

			ds = M68k_EffectiveAddress( & ec, rt );

			if ( ds != RS4DecodeStat_Okay )
			{
				// ec allready set

				#ifdef DEBUG
				printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
				#endif

				goto bailout;
			}

			// --

			rt->rt_CPU.M68k.mt_ArgEMode = ( rt->rt_CPU.M68k.mt_Opcode & 0x00380000 ) >> 19;
			rt->rt_CPU.M68k.mt_ArgEReg  = ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;

			rt->rt_CPU.M68k.mt_CurRegister = & rt->rt_CPU.M68k.mt_DstRegister;

			ds = M68k_EffectiveAddress( & ec, rt );

			if ( ds != RS4DecodeStat_Okay )
			{
				// ec allready set

				#ifdef DEBUG
				printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
				#endif

				goto bailout;
			}

			// --

			M68k_Set_Cur_to_Unknown( rt );
//			rt->rt_CPU.M68k.mt_CurRegister->mr_Type1 = RRT_Unknown;
			rt->rt_CPU.M68k.mt_OpcodeSize = rt->rt_CPU.M68k.mt_ArgSize;
			break;
		}

		case 2:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = ( ShortOpcodes ) ? "And.l" : "Andi.l" ;

			// --

			rt->rt_CPU.M68k.mt_ArgType  = M68KSIZE_Long;
			rt->rt_CPU.M68k.mt_ArgEMode = 7;
			rt->rt_CPU.M68k.mt_ArgEReg  = 4;

			rt->rt_CPU.M68k.mt_CurRegister = & rt->rt_CPU.M68k.mt_SrcRegister;

			ds = M68k_EffectiveAddress( & ec, rt );

			if ( ds != RS4DecodeStat_Okay )
			{
				// ec allready set

				#ifdef DEBUG
				printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
				#endif

				goto bailout;
			}

			// --

			rt->rt_CPU.M68k.mt_ArgEMode = ( rt->rt_CPU.M68k.mt_Opcode & 0x00380000 ) >> 19;
			rt->rt_CPU.M68k.mt_ArgEReg  = ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;

			rt->rt_CPU.M68k.mt_CurRegister = & rt->rt_CPU.M68k.mt_DstRegister;

			ds = M68k_EffectiveAddress( & ec, rt );

			if ( ds != RS4DecodeStat_Okay )
			{
				// ec allready set

				#ifdef DEBUG
				printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
				#endif

				goto bailout;
			}

			// --

			M68k_Set_Cur_to_Unknown( rt );
//			rt->rt_CPU.M68k.mt_CurRegister->mr_Type1 = RRT_Unknown;
			rt->rt_CPU.M68k.mt_OpcodeSize = rt->rt_CPU.M68k.mt_ArgSize;
			break;
		}

		default:
		{
			printf( "Unsupported 'Andi' Opcode (Mode: %d)\n", size );
			ds = RS4DecodeStat_Error;
			goto bailout;
		}
	}

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

// --

enum RS4DecodeStat M68kCmd_ANDI2( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
uint8_t val;

	val = ( rt->rt_CPU.M68k.mt_Opcode & 0x000000ff );

	rt->rt_Container.Hunk.ms_Str_Opcode = ( ShortOpcodes ) ? "And.b" : "Andi.b" ;
	rt->rt_CPU.M68k.mt_OpcodeSize = 4;
	rt->rt_CPU.M68k.mt_ArgType = M68KSIZE_Byte;

	sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "#$%02x,CCR", val );

	// --

	ds = RS4DecodeStat_Okay;
	ec = RS4ErrStat_Okay;

	// --

//bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( ds );
}

// --

enum RS4DecodeStat M68kCmd_ANDI3( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
uint16_t val;

	val = ( rt->rt_CPU.M68k.mt_Opcode & 0x0000ffff );

	rt->rt_Container.Hunk.ms_Str_Opcode = ( ShortOpcodes ) ? "And.w" : "Andi.w" ;
	rt->rt_CPU.M68k.mt_OpcodeSize = 4;
	rt->rt_CPU.M68k.mt_ArgType = M68KSIZE_Word;

	sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "#$%04x,SR", val );

	// --

	ds = RS4DecodeStat_Okay;
	ec = RS4ErrStat_Okay;

	// --

//bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( ds );
}
