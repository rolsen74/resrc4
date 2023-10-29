
/*
 * Copyright (c) 2014-2023 Rene W. Olsen < renewolsen @ gmail . com >
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

void myAddHead( struct Header *h, void *n2 );
void myAddTail( struct Header *h, void *n2 );
void myAddAfter( struct Header *h, void *curptr, void *newptr );

void *myRemHead( struct Header *h );

void *myGetHead( struct Header *h );
void *myGetTail( struct Header *h );

void *myGetNext( void *nodeptr );
void *myGetPrev( void *nodeptr );

// --

void myAddHead( struct Header *h, void *n2 )
{
struct Node *n = n2;
struct Node *t;

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

void myAddTail( struct Header *h, void *n2 )
{
struct Node *n = n2;
struct Node *t;

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

void myAddAfter( struct Header *h, void *curptr, void *newptr )
{
struct Node *n;	// New Node
struct Node *c;	// Current Node
struct Node *t;	// Temp Node

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

void *myRemHead( struct Header *h )
{
struct Node *next;
struct Node *n;

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

void *myGetHead( struct Header *h )
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

void *myGetTail( struct Header *h )
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

void *myGetNext( void *nodeptr )
{
struct Node *node;
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

void *myGetPrev( void *nodeptr )
{
struct Node *node;
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

