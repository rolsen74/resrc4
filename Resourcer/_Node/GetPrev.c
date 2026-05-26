
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

PTR
RS4GetPrev ( PTR nodeptr )
{
	RS4Node * node;
	PTR		  prev;

	node = nodeptr;

	if ( node )
	{
		prev = node->Prev;
	}
	else
	{
		prev = NULL;
	}

	return ( prev );
}

// --
