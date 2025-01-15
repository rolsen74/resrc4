
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
// -- Mode 20 - Clr (Ax)

inline static enum RS4DecodeStat MODE_20( enum RS4ErrorCode *errcode, RS4Trace *rt, char *outstr )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;

	// --

	ec = RS4ErrStat_Okay;
	ds = RS4DecodeStat_Okay;

	// --

	sprintf( outstr, "(%s)", Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ] );

//	if ( rt->rt_CPU.M68k.mt_Registers[ M68KREGT_Ax + rt->rt_CPU.M68k.mt_ArgEReg ].mr_Type == RRT_Label )
//	{
//		rt->rt_CPU.M68k.mt_Registers[ M68KREGT_Ax + rt->rt_CPU.M68k.mt_ArgEReg ].mr_Data.mr_Label->rl_Type = RS4LabelType_Unset;
//	}

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( ds );
}

// --
