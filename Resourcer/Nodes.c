
/*
 * Copyright (c) 2014-2024 Rene W. Olsen < renewolsen @ gmail . com >
 *
 * This software is released under the GNU General Public License, version 3.
 * For the full text of the license, please visit:
 * https://www.gnu.org/licenses/gpl-3.0.html
 *
 * You can also find a copy of the license in the LICENSE file included with this software.
 */

// --

#include "ReSrc4.h"

// --

void RS4AddHead( RS4Header *h, void *n2 )
{
RS4Node *n = n2;
RS4Node *t;

	if ( h->Head )
	{
		t = h->Head;

		n->Next	    = h->Head;
		n->Prev     = NULL;

		t->Prev		= n;
		h->Head		= n;
		h->Nodes   += 1;
	}
	else
	{
		n->Next		= NULL;
		n->Prev		= NULL;

		h->Head		= n;
		h->Tail		= n;
		h->Nodes	= 1;
	}
}

// --

void RS4AddTail( RS4Header *h, void *n2 )
{
RS4Node *n = n2;
RS4Node *t;

	if ( h->Tail )
	{
		t = h->Tail;

		n->Next	    = NULL;
		n->Prev     = h->Tail;

		t->Next		= n;
		h->Tail		= n;
		h->Nodes   += 1;
	}
	else
	{
		n->Next		= NULL;
		n->Prev		= NULL;

		h->Head		= n;
		h->Tail		= n;
		h->Nodes	= 1;
	}
}

// --

void RS4AddAfter( RS4Header *h, void *curptr, void *newptr )
{
RS4Node *n;	// New Node
RS4Node *c;	// Current Node
RS4Node *t;	// Temp Node

	if ( newptr == NULL )
	{
		goto bailout;
	}

	n = newptr;
	c = curptr;

	if (( h->Head == NULL ) || ( h->Tail == NULL ))
	{
		// Empty List

		h->Head = n;
		h->Tail = n;
		h->Nodes = 1;

		n->Next = NULL;
		n->Prev = NULL;
	}
	else if (( c == NULL ) || ( c->Next == NULL ))
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

void *RS4RemHead( RS4Header *h )
{
RS4Node *next;
RS4Node *n;

	n = h->Head;

	if ( n == NULL )
	{
		h->Tail = NULL;
		h->Nodes = 0;
	}
	else if ( h->Tail == n )
	{
		n->Next = NULL;
		n->Prev = NULL;

		h->Head = NULL;
		h->Tail = NULL;
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

	return(	n );
}

// --

void *RS4GetHead( RS4Header *h )
{
void *head;

	if ( h )
	{
		head = h->Head;
	}
	else
	{
		head = NULL;
	}

	return( head );
}

// --

void *RS4GetTail( RS4Header *h )
{
void *tail;

	if ( h )
	{
		tail = h->Tail;
	}
	else
	{
		tail = NULL;
	}

	return( tail );
}

// --

void *RS4GetNext( void *nodeptr )
{
RS4Node *node;
void *next;

	node = nodeptr;

	if ( node )
	{
		next = node->Next;
	}
	else
	{
		next = NULL;
	}

	return( next );
}

// --

void *RS4GetPrev( void *nodeptr )
{
RS4Node *node;
void *prev;

	node = nodeptr;

	if ( node )
	{
		prev = node->Prev;
	}
	else
	{
		prev = NULL;
	}

	return( prev );
}

// --

