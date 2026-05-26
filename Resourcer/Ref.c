
/*
** Copyright (c) 2014-2026 Rene W. Olsen
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

#include "ReSrc4.h"

// --

enum RS4FuncStat
RS4FirstReloc_Sec ( enum RS4ErrorCode * errcode, RS4Ref ** rr_ptr, RS4FileSection * sec, S64 adr1, S64 adr2 )
{
	enum RS4ErrorCode ec;
	enum RS4FuncStat  fs;
	RS4Ref *		  rr;

	rr = NULL;
	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

#ifdef DEBUG

	if ( ! sec )
	{
		fs = RS4FuncStat_Error;
		printf ( "%s:%04d: Error NULL Pointer\n", __FILE__, __LINE__ );
		goto bailout;
	}

	if ( adr1 > adr2 )
	{
		fs = RS4FuncStat_Error;
		printf ( "%s:%04d: Error Invalid Addresses $%08" PRIx64 "-$%08" PRIx64 "\n", __FILE__, __LINE__, adr1, adr2 );
		goto bailout;
	}

#endif

	rr = RS4GetTail ( &sec->rfs_SecRefs );

	while ( rr )
	{
		if ( ( rr->rr_Address >= adr1 ) && ( rr->rr_Address <= adr2 ) )
		{
			break;
		}
		else
		{
			rr = RS4GetPrev ( rr );
		}
	}

#ifdef DEBUG
bailout:
#endif

	if ( rr_ptr )
	{
		*rr_ptr = rr;
	}

	if ( errcode )
	{
		*errcode = ec;
	}

	return ( fs );
}

// --
