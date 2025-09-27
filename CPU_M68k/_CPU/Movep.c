
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

enum RS4DecodeStat M68kCmd_MOVEP( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
S32 mode;
S32 areg;
S32 dreg;

	areg = ( rt->rt_CPU.M68k.mt_Opcode & 0x00070000 ) >> 16;
	dreg = ( rt->rt_CPU.M68k.mt_Opcode & 0x07000000 ) >> 24;
	mode = ( rt->rt_CPU.M68k.mt_Opcode & 0x00e00000 ) >> 21;

	switch( mode )
	{
		case 4:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Movep.w";
			rt->rt_CPU.M68k.mt_ArgType  = M68KSIZE_Word;
	
			rt->rt_CPU.M68k.mt_ArgEReg	= areg;
			rt->rt_CPU.M68k.mt_ArgEMode	= 0x50;
	
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

			rt->rt_CPU.M68k.mt_ArgEReg	= dreg;
			rt->rt_CPU.M68k.mt_ArgEMode	= 0x00;

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
			break;
		}

		case 5:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Movep.l";
			rt->rt_CPU.M68k.mt_ArgType  = M68KSIZE_Long;

			rt->rt_CPU.M68k.mt_ArgEReg	= areg;
			rt->rt_CPU.M68k.mt_ArgEMode	= 0x50;
	
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

			rt->rt_CPU.M68k.mt_ArgEReg	= dreg;
			rt->rt_CPU.M68k.mt_ArgEMode	= 0x00;

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
			break;
		}

		case 6:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Movep.w";
			rt->rt_CPU.M68k.mt_ArgType  = M68KSIZE_Word;

			rt->rt_CPU.M68k.mt_ArgEReg	= dreg;
			rt->rt_CPU.M68k.mt_ArgEMode	= 0x00;
	
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

			rt->rt_CPU.M68k.mt_ArgEReg	= areg;
			rt->rt_CPU.M68k.mt_ArgEMode	= 0x50;

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
			break;
		}

		case 7:
		{
			rt->rt_Container.Hunk.ms_Str_Opcode = "Movep.l";
			rt->rt_CPU.M68k.mt_ArgType  = M68KSIZE_Long;

			rt->rt_CPU.M68k.mt_ArgEReg	= dreg;
			rt->rt_CPU.M68k.mt_ArgEMode	= 0x00;
	
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

			rt->rt_CPU.M68k.mt_ArgEReg	= areg;
			rt->rt_CPU.M68k.mt_ArgEMode	= 0x50;

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
			break;
		}

		default:
		{
			printf( "Unsupported 'Movep' Opcode (Mode: %d)\n", mode );
			ds = RS4DecodeStat_Error;
			goto bailout;
		}
	}

    // --

	rt->rt_CPU.M68k.mt_OpcodeSize = 4;

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
