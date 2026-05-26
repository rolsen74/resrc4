
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

void
RS4AddTail ( RS4Header * h, PTR n2 )
{
	RS4Node * n = n2;
	RS4Node * t;

	if ( h->Tail )
	{
		t = h->Tail;

		n->Next = NULL;
		n->Prev = t;

		t->Next = n;
		h->Tail = n;
		h->Nodes += 1;
	}
	else
	{
		n->Next = NULL;
		n->Prev = NULL;

		h->Head	 = n;
		h->Tail	 = n;
		h->Nodes = 1;
	}
}

// --
