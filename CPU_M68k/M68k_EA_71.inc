
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
// -- Mode 71 - move.l $00000004

inline static enum RS4DecodeStat MODE_71( enum RS4ErrorCode *errcode, RS4Trace *rt, char *outstr )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Label *rl;
RS4Ref *isRef;
uint32_t val;
uint8_t *mem;
int pos;

	// --

	ec = RS4ErrStat_Error;
	ds = RS4DecodeStat_Error;

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
			printf( "%s:%04d: Error finding label at %08lx ($%08x)\n", __FILE__, __LINE__, rt->rt_CurMemAdr, val );
			#endif

			goto bailout;
		}

		if ( rl->rl_Name[0] )
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
			// Not an external as it has a Refs 
			sprintf( outstr, "($%08x).l", val );
		}

		if (( rl ) && ( rt->rt_CPU.M68k.mt_CurRegister ))
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
			sprintf( outstr, "($%08x).l", val );
		}

		if ( rt->rt_CPU.M68k.mt_CurRegister )
		{
			rt->rt_CPU.M68k.mt_CurRegister->mr_Type1 = RRT_Address;
			rt->rt_CPU.M68k.mt_CurRegister->mr_Address = val;
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
