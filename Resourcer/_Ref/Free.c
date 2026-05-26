
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
RS4FreeRef ( enum RS4ErrorCode * errcode, RS4Ref * rr )
{
	enum RS4ErrorCode ec;
	enum RS4FuncStat  fs;

	ec = RS4ErrStat_Error;
	fs = RS4FuncStat_Error;

	// --

	if ( ! rr )
	{
		goto bailout;
	}

	if ( rr->rr_ID != RS4ID_Ref )
	{
		ec = RS4ErrStat_InvalidStructID;

#ifdef DEBUG
		printf ( "%s:%04d: Error Invalid Struct ID\n", __FILE__, __LINE__ );
#endif

		goto bailout;
	}

	rr->rr_ID = 0;

#ifdef DEBUG
	memset ( rr, 6, sizeof ( RS4Ref ) );
#endif

	// --

	free ( rr );

	// --

	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	return ( fs );
}

// --
