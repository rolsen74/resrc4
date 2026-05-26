
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
RS4RemHead ( RS4Header * h )
{
	RS4Node * next;
	RS4Node * n;

	n = h->Head;

	/**/ if ( ! n )
	{
		h->Tail	 = NULL;
		h->Nodes = 0;
	}
	else if ( h->Tail == n )
	{
		n->Next = NULL;
		n->Prev = NULL;

		h->Head	 = NULL;
		h->Tail	 = NULL;
		h->Nodes = 0;
	}
	else
	{
		next = n->Next;

		next->Prev = NULL;

		n->Next = NULL;
		n->Prev = NULL;

		h->Head = next;
		h->Nodes--;
	}

	return ( n );
}

// --
