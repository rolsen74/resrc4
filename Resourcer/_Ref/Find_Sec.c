
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
RS4FindRef_Sec ( enum RS4ErrorCode * errcode, RS4Ref ** rr_ptr, RS4FileSection * sec, S64 addr )
{
	enum RS4ErrorCode ec;
	enum RS4FuncStat  fs;
	RS4Ref *		  rr;
	U32				  hash;

	rr = NULL;
	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

	if ( ! addr )
	{
		goto bailout;
	}

	hash = ( (U64)addr ) % MAX_REF_HASH;

	rr = sec->rfs_SecRefs2[hash];

	while ( rr )
	{
		if ( rr->rr_Address == addr )
		{
			break;
		}
		else
		{
			rr = rr->rr_HashPtr;
		}
	}

#ifdef DEBUG
	if ( ( rr ) && ( rr->rr_ID != RS4ID_Ref ) )
	{
		printf ( "Invalid Ref ID : %08x\n", rr->rr_ID );
		rr = NULL;
	}
#endif

bailout:

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
