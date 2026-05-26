
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
RS4Strdup ( STR * strptr, STR string )
{
	enum RS4FuncStat fs;
	STR				 str;
	S32				 len;

	fs	= RS4FuncStat_Error;
	str = NULL;

	if ( ! string )
	{
		goto bailout;
	}

	len = strlen ( string );

	str = malloc ( len + 1 );

	if ( ! str )
	{
		goto bailout;
	}

	strcpy ( str, string );

	fs = RS4FuncStat_Okay;

bailout:

	if ( strptr )
	{
		*strptr = str;
	}

	return ( fs );
}

// --
