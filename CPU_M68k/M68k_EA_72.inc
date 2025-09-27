
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
// -- Mode 72 - Clr (xxxx.w,pc)
// xxxx is Signed

inline static enum RS4DecodeStat MODE_72( enum RS4ErrorCode *errcode, RS4Trace *rt, STR outstr )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Label *rl;
U32 adr;
S16 val;
MEM mem;
CHR labname[ MAX_LabelName + 8 ];
S32 pos;

	// --

	ec	= RS4ErrStat_Error;
	ds	= RS4DecodeStat_Error;
	mem	= rt->rt_CurMemBuf;
	pos	= rt->rt_CPU.M68k.mt_ArgSize;
	val	= (( mem[pos] << 8 ) | ( mem[pos+1] << 0 ));
	adr	= rt->rt_CurMemAdr + 2 + val;

	if ( rt->rt_Pass == RS4TracePass_Trace )
	{
		// We can get away with AddLabel2, as this is a PC function, 
		// so we can handle Labels out side Hunk Memory area
		rl = RS4AddLabel_Sec( NULL, rt->rt_Section, adr, RS4LabelType_Unset );
	}
	else
	{
		rl = RS4FindLabel_File( rt->rt_File, adr );

		if ( ! rl )
		{
			ec = RS4ErrStat_Error;
			ds = RS4DecodeStat_Error;

			#ifdef DEBUG
			printf( "%s:%04d: Error finding label at %08x\n", __FILE__, __LINE__, adr );
			#endif

			goto bailout;
		}
	}

	// --

	if (( rl ) && ( rt->rt_CPU.M68k.mt_DoLabelSize ))
	{
		enum RS4LabelSize rls;

		/**/ if ( rt->rt_CPU.M68k.mt_ArgType == M68KSIZE_Byte )		rls = RS4LABSIZE_Integer8;
		else if ( rt->rt_CPU.M68k.mt_ArgType == M68KSIZE_Word )		rls = RS4LABSIZE_Integer16;
		else if ( rt->rt_CPU.M68k.mt_ArgType == M68KSIZE_Long )		rls = RS4LABSIZE_Integer32;
		else if ( rt->rt_CPU.M68k.mt_ArgType == M68KSIZE_Single )	rls = RS4LABSIZE_Float32;
		else if ( rt->rt_CPU.M68k.mt_ArgType == M68KSIZE_Double )	rls = RS4LABSIZE_Float64;
		else rls = RS4LABSIZE_Unknown;

		if ( rl->rl_Label_RW_Size == RS4LABSIZE_Unset )
		{
			rl->rl_Label_RW_Size = rls;
		}
		else
		{
			if ( rl->rl_Label_RW_Size != rls )
			{
				/**/ if (( rl->rl_Label_RW_Size == RS4LABSIZE_Integer32 ) && ( rls == RS4LABSIZE_Float32 ))
				{
					// Convert Long to Float
					rl->rl_Label_RW_Size = rls;
				}
				else if (( rl->rl_Label_RW_Size == RS4LABSIZE_Float32 ) && ( rls == RS4LABSIZE_Integer32 ))
				{
					// Do not convert Float to Long
					// so do nothing, not an error
				}
				else
				{
					rl->rl_Label_RW_Size = RS4LABSIZE_Unknown;
				}
			}
			else
			{
				rl->rl_Label_RW_Size = rls;
			}
		}
	}

	#if 0
	enum M68KOpcodeSize
	{
		M68KSIZE_Unsized,
		M68KSIZE_Byte,		// move.b
		M68KSIZE_Word,		// move.w
		M68KSIZE_Long,		// move.l
		M68KSIZE_Single,	// fmove.s
		M68KSIZE_Double,	// fmove.d
		M68KSIZE_Extended,	// fmove.x
		M68KSIZE_Packed,	// fmove.p
	};

	enum RS4LabelSize
	{
		RS4LABSIZE_Unset,		//
		RS4LABSIZE_Unknown,		// Diffrent sizes have been used on label
		RS4LABSIZE_Integer8,		// move.b
		RS4LABSIZE_Integer16,		// move.w
		RS4LABSIZE_Integer32,		// move.l
		RS4LABSIZE_Integer64,		// move.q
		RS4LABSIZE_Float,		// move.s
		RS4LABSIZE_Double,		// move.d
	};
	#endif

	// --

	if ( rt->rt_Pass != RS4TracePass_Trace )
	{
		if (( rl ) && ( rl->rl_Name[0] ))
		{
			fs = RS4BuildLabelString( & ec, rl, labname );

			if ( fs != RS4FuncStat_Okay )
			{
				// ec allready set

				#ifdef DEBUG
				printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
				#endif

				goto bailout;
			}

			sprintf( outstr, "(%s,PC)", labname );
		}
		else
		{
			sprintf( outstr, "($%08x,PC)", adr );
		}
	}
	else
	{
		outstr[0] = 0;
	}

	if (( rl ) && ( rt->rt_CPU.M68k.mt_CurRegister ))
	{
		rt->rt_CPU.M68k.mt_CurRegister->mr_Type1 = RRT_Label;
		rt->rt_CPU.M68k.mt_CurRegister->mr_Label = rl;
	}

	rt->rt_CPU.M68k.mt_ArgSize += 2;

	// --

	ec = RS4ErrStat_Okay;
	ds = RS4DecodeStat_Okay;

	// --

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( ds );
}

// --
