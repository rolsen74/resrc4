
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
RS4FreeSection ( enum RS4ErrorCode * errcode, RS4FileSection * sec )
{
	enum RS4ErrorCode ec;
	enum RS4FuncStat  fs;
	RS4Source *		  rs;
	RS4Label *		  rl;
	RS4Ref *		  rr;

	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Error;

	if ( sec )
	{
		goto bailout;
	}

	if ( sec->rfs_ID != RS4ID_FileSection )
	{
		ec = RS4ErrStat_InvalidStructID;

#ifdef DEBUG
		printf ( "%s:%04d: Error Invalid Struct ID\n", __FILE__, __LINE__ );
#endif

		goto bailout;
	}

	sec->rfs_ID = 0;

	while ( TRUE )
	{
		rl = RS4RemHead ( &sec->rfs_SecLabels );

		if ( ! rl )
		{
			break;
		}

		RS4FreeLabel ( NULL, rl );
	}

	while ( TRUE )
	{
		rs = RS4RemHead ( &sec->rfs_SourceList );

		if ( ! rs )
		{
			break;
		}

		free ( rs );
	}

	while ( TRUE )
	{
		rr = RS4RemHead ( &sec->rfs_SecRefs );

		if ( ! rr )
		{
			break;
		}

		RS4FreeRef ( NULL, rr );
	}

	// -- free other stuff here, still todo

	if ( sec->rfs_MemoryType )
	{
		free ( sec->rfs_MemoryType );
		sec->rfs_MemoryType = NULL;
	}

	if ( sec->rfs_MemoryBuf )
	{
		free ( sec->rfs_MemoryBuf );
		sec->rfs_MemoryBuf = NULL;
	}

	free ( sec );

	// --

	ec = RS4ErrStat_Okay;

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	return ( fs );
};

// --
