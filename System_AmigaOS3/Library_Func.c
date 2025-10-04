
/*
** Copyright (c) 2014-2025 Rene W. Olsen
**
** SPDX-License-Identifier: GPL-3.0-or-later
**
** This software is released under the GNU General Public License, version 3.
** For the full text of the license, please visit:
** https://www.gnu.org/licenses/gpl-3.0.html
**
** You can also find a copy of the license in the LICENSE file included with this software.
*/

// --

#include "Resourcer/ReSrc4.h"

// --

enum RS4FuncStat AmigaOS3_FindLibFunc_Func( enum RS4ErrorCode *errcode, RS4Trace *rt, enum RS4DecodeStat (*Func)( enum RS4ErrorCode *errcode, RS4Trace *rt ))
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
enum RS4FuncStat fs;

//	printf( "AmigaOS3_FindLibFunc_Func : \n" );

	fs = RS4FuncStat_Error;
	ec = RS4ErrStat_Okay;

	// --

	if ( ! Func )
	{
		ec = RS4ErrStat_Internal;
		printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
		goto bailout;
	}

	ds = Func( & ec, rt );

	if ( ds != RS4DecodeStat_Okay )
	{
		// ec allready set
		printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
		goto bailout;
	}

	// --

	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fs );
}

// --
