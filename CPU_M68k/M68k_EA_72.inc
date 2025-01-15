
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
// -- Mode 72 - Clr (xxxx.w,pc)
// xxxx is Signed

inline static enum RS4DecodeStat MODE_72( enum RS4ErrorCode *errcode, RS4Trace *rt, char *outstr )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Label *rl;
uint32_t adr;
int16_t val;
uint8_t *mem;
char labname[ MAX_LabelName + 8 ];
int pos;

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
