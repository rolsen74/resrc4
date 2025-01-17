
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
// -- Mode 50 - jsr (d16,Ax)

inline static enum RS4DecodeStat MODE_50( enum RS4ErrorCode *errcode, RS4Trace *rt, char *outstr )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
int16_t val;
uint8_t *mem;
char *lvostr;
int pos;

	// --

	ec = RS4ErrStat_Error;
	ds = RS4DecodeStat_Error;

	// --

	mem = rt->rt_CurMemBuf;
	pos = rt->rt_CPU.M68k.mt_ArgSize;

	val = (( mem[pos] << 8 ) | ( mem[pos+1] << 0 ));

	#ifdef SUPPORT_AMIGAOS3
	if	(( rt->rt_CPU.M68k.mt_LibCall )
	&&	 ( rt->rt_CPU.M68k.mt_Registers[ M68KREGT_Ax + rt->rt_CPU.M68k.mt_ArgEReg ].mr_Type1 ) == RRT_Library )
	{
		lvostr = AmigaOS3_FindLibFunc( & ec, rt, val );

		if (( ! lvostr ) && ( ec != RS4ErrStat_Okay ))
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
	#endif
	{
		lvostr = NULL;
	}

	if ( rt->rt_Pass != RS4TracePass_Trace )
	{
		if ( lvostr )
		{
			sprintf( outstr, "(%s,%s)", lvostr, Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ] );
		}
		else
		{
			// Most proberly a Library call, so we use Decimal
			sprintf( outstr, "(%d,%s)", val, Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ] );
		}
	}
	else
	{
		outstr[0] = 0;
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
