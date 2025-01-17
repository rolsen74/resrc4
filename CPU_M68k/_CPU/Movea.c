
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

enum RS4DecodeStat M68kCmd_MOVEA( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
uint32_t size;
uint8_t *mem;

#ifdef SUPPORT_AMIGAOS3
struct AmigaOS3_Misc_Move_GetSetStruct gss;
#endif

	size = ( rt->rt_CPU.M68k.mt_Opcode & 0x30000000 ) >> 28;

	switch( size )
	{
		case 3:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = ( ShortOpcodes ) ? "Move.w" : "Movea.w" ;
			rt->rt_CPU.M68k.mt_ArgType	= M68KSIZE_Word;
			rt->rt_CPU.M68k.mt_ArgEMode = ( rt->rt_CPU.M68k.mt_Opcode & 0x00380000 ) >> 19;
			rt->rt_CPU.M68k.mt_ArgEReg  = ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;

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

			rt->rt_CPU.M68k.mt_ArgEReg  = ( rt->rt_CPU.M68k.mt_Opcode & 0x0e000000 ) >> 25;	
			rt->rt_CPU.M68k.mt_ArgEMode = ( rt->rt_CPU.M68k.mt_Opcode & 0x01c00000 ) >> 22;

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

			M68k_Set_Cur_to_Unknown( rt );
			rt->rt_CPU.M68k.mt_OpcodeSize = rt->rt_CPU.M68k.mt_ArgSize;
			break;
		}

		case 2:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = ( ShortOpcodes ) ? "Move.l" : "Movea.l" ;
			rt->rt_CPU.M68k.mt_ArgType  = M68KSIZE_Long;
			rt->rt_CPU.M68k.mt_ArgEMode = ( rt->rt_CPU.M68k.mt_Opcode & 0x00380000 ) >> 19;
			rt->rt_CPU.M68k.mt_ArgEReg  = ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;

			// --

			rt->rt_CPU.M68k.mt_CurRegister = & rt->rt_CPU.M68k.mt_SrcRegister;

			#ifdef SUPPORT_AMIGAOS3
			mem = & rt->rt_CurMemBuf[ rt->rt_CPU.M68k.mt_ArgSize ];
			#endif

			ds = M68k_EffectiveAddress( & ec, rt );

			if ( ds != RS4DecodeStat_Okay )
			{
				// ec allready set

				#ifdef DEBUG
				printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
				#endif

				goto bailout;
			}

			#ifdef SUPPORT_AMIGAOS3
			{
				enum RS4FuncStat fs = AmigaOS3_Misc_Move_Get( & ec, rt, rt->rt_CPU.M68k.mt_CurRegister, mem, & gss );

				if ( fs != RS4FuncStat_Okay )
				{
					// ec allready set
					ds = RS4DecodeStat_Error;

					#ifdef DEBUG
					printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
					#endif

					goto bailout;
				}
			}
			#endif

			// --

			rt->rt_CPU.M68k.mt_ArgEReg  = ( rt->rt_CPU.M68k.mt_Opcode & 0x0e000000 ) >> 25;
			rt->rt_CPU.M68k.mt_ArgEMode = ( rt->rt_CPU.M68k.mt_Opcode & 0x01c00000 ) >> 22;

			rt->rt_CPU.M68k.mt_CurRegister = & rt->rt_CPU.M68k.mt_DstRegister;

			mem = & rt->rt_CurMemBuf[ rt->rt_CPU.M68k.mt_ArgSize ];

			ds = M68k_EffectiveAddress( & ec, rt );

			if ( ds != RS4DecodeStat_Okay )
			{
				// ec allready set

				#ifdef DEBUG
				printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
				#endif

				goto bailout;
			}

			#ifdef SUPPORT_AMIGAOS3
			{
				enum RS4FuncStat fs = AmigaOS3_Misc_Move_Set( & ec, rt, rt->rt_CPU.M68k.mt_CurRegister, mem, & gss );

				if ( fs != RS4FuncStat_Okay )
				{
					// ec allready set
					ds = RS4DecodeStat_Error;

					#ifdef DEBUG
					printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
					#endif

					goto bailout;
				}
			}
			#endif

			// --

			rt->rt_CPU.M68k.mt_OpcodeSize = rt->rt_CPU.M68k.mt_ArgSize;
			break;
		}

		default:
		{
			printf( "Unsupported 'Movea' Opcode (Mode: %d)\n", size );
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
