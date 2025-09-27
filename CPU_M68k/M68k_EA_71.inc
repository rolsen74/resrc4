
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
// -- Mode 71 - move.l $00000004
extern S32 leatst;

inline static enum RS4DecodeStat MODE_71( enum RS4ErrorCode *errcode, RS4Trace *rt, STR outstr )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Label *rl;
RS4Ref *isRef;
U32 val;
MEM mem;
S32 pos;

	// --

	ec = RS4ErrStat_Error;
	ds = RS4DecodeStat_Error;
	rl = NULL;

	// --

	mem = rt->rt_CurMemBuf;
	pos = rt->rt_CPU.M68k.mt_ArgSize;
	val = (( mem[pos] << 24 ) | ( mem[pos+1] << 16 ) | ( mem[pos+2] << 8 ) | ( mem[pos+3] << 0 ));

	isRef = RS4FindRef_Sec( rt->rt_Section, rt->rt_CurMemAdr + rt->rt_CPU.M68k.mt_ArgSize );

	if ( isRef )
	{
		isRef->rr_Handled = TRUE;

		// if there is a Ref then the a label have been added
		rl = RS4FindLabel_File( rt->rt_File, val );

		if ( ! rl )
		{
			ec = RS4ErrStat_Error;
			ds = RS4DecodeStat_Error;

			#ifdef DEBUG
			printf( "%s:%04d: Error finding label at $%08" PRIx64 " ($%08x)\n", __FILE__, __LINE__, rt->rt_CurMemAdr, val );
			#endif

			goto bailout;
		}

		if ( rt->rt_Pass != RS4TracePass_Trace )
		{
			fs = RS4BuildLabelString( & ec, rl, outstr );

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
		else
		{
			outstr[0] = 0;
		}

		if ( rt->rt_CPU.M68k.mt_CurRegister )
		{
			rt->rt_CPU.M68k.mt_CurRegister->mr_Type1 = RRT_Label;
			rt->rt_CPU.M68k.mt_CurRegister->mr_Label = rl;
		}
	}
	else
	{
		if ( rt->rt_CPU.M68k.mt_DoExternal )
		{
			rl = RS4AddExtLabel( NULL, rt->rt_File, val );

			// rwo: why no check?

			if ( rt->rt_Pass != RS4TracePass_Trace )
			{
				if (( rl ) && ( rl->rl_Name[0] ))
				{
					sprintf( outstr, "%s", rl->rl_Name );
				}
				else
				{
					sprintf( outstr, "($%08x).l", val );
				}
			}
			else
			{
				outstr[0] = 0;
			}
		}
		else
		{
			if ( rt->rt_Pass != RS4TracePass_Trace )
			{
				sprintf( outstr, "($%08x).l", val );
			}
			else
			{
				outstr[0] = 0;
			}
		}

		if ( rt->rt_CPU.M68k.mt_CurRegister )
		{
			rt->rt_CPU.M68k.mt_CurRegister->mr_Type1 = RRT_Address;
			rt->rt_CPU.M68k.mt_CurRegister->mr_Address = val;
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

	// --

	rt->rt_CPU.M68k.mt_ArgSize += 4;

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
