
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
RS4AddAfter ( RS4Header * h, PTR curptr, PTR newptr )
{
	RS4Node * n; // New Node
	RS4Node * c; // Current Node
	RS4Node * t; // Temp Node

	if ( ! newptr )
	{
		goto bailout;
	}

	n = newptr;
	c = curptr;

	if ( ( ! h->Head ) || ( ! h->Tail ) )
	{
		// Empty List

		h->Head	 = n;
		h->Tail	 = n;
		h->Nodes = 1;

		n->Next = NULL;
		n->Prev = NULL;
	}
	else if ( ( ! c ) || ( ! c->Next ) )
	{
		// Insert Tail

		t = h->Tail;

		n->Next = NULL;
		n->Prev = t;

		t->Next = n;

		h->Tail = n;
		h->Nodes++;
	}
	else
	{
		// Insert Middle
		//
		// Cur
		//  <- New
		// Tmp

		t = c->Next;

		n->Next = t;
		n->Prev = c;

		t->Prev = n;
		c->Next = n;

		h->Nodes++;
	}

bailout:

	return;
}

// --
