
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
// -- Mode 30 - Clr (Ax)+

inline static enum RS4DecodeStat MODE_30( enum RS4ErrorCode *errcode, RS4Trace *rt, char *outstr )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;

	// --

	ec = RS4ErrStat_Okay;
	ds = RS4DecodeStat_Okay;

	// --

	if ( rt->rt_Pass != RS4TracePass_Trace )
	{
		sprintf( outstr, "(%s)+", Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ] );
	}
	else
	{
		outstr[0] = 0;
	}

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( ds );
}

// --
