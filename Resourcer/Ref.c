
/*
** Copyright (c) 2014-2025 Rene W. Olsen
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

enum RS4FuncStat RS4FreeRef( enum RS4ErrorCode *errcode, RS4Ref *rr )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;

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
		printf( "%s:%04d: Error Invalid Struct ID\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	rr->rr_ID = 0;

	// --



	free( rr );

	// --

	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fs );
}

// --

enum RS4FuncStat RS4AddRef_Sec( enum RS4ErrorCode *errcode, RS4Ref **ref_ptr, RS4FileSection *sec, S64 offset )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Ref *new;
RS4Ref *rr;
U32 hash;

	ec	= RS4ErrStat_Error;
	fs	= RS4FuncStat_Error;
	rr	= NULL;
	new	= NULL;

	// --

	#ifdef DEBUG
	if ( ! sec )
	{
		ec = RS4ErrStat_Internal;
		printf( "%s:%04d: NULL Pointer\n", __FILE__, __LINE__ );
		goto bailout;
	}
	#endif

	// --

	new = calloc( 1, sizeof( RS4Ref ));

	if ( ! new )
	{
		ec = RS4ErrStat_OutOfMemory;

		#ifdef DEBUG
		printf( "%s:%04d: Error allocating Memory (%d Bytes)\n", __FILE__, __LINE__, (S32) sizeof( RS4Ref ));
		#endif

		goto bailout;
	}

	new->rr_ID		= RS4ID_Ref;
	new->rr_Address = offset + sec->rfs_MemoryAdr;
	new->rr_Offset	= offset;

	if ( DoVerbose > 3 )
	{
		printf( "New Ref   : Hunk #%d : Address $%08" PRIx64 "\n", sec->rfs_SecNr, new->rr_Address );
	}

	if ( 0 > offset )
	{
		printf( "## Warning ## : New Ref 1 : Offset : %" PRId64 "\n", offset );
	}

	if ( sec->rfs_MemorySize < offset )
	{
		printf( "## Warning ## : New Ref 2 : Offset : %" PRId64 "\n", offset );
	}

	// --

	rr = RS4GetTail( & sec->rfs_SecRefs );

	while( rr )
	{
		if ( rr->rr_Offset <= offset )
		{
			break;
		}
		else
		{
			rr = RS4GetPrev( rr );
		}
	}

	if ( rr )
	{
		RS4AddAfter( & sec->rfs_SecRefs, rr, new );
	}
	else
	{
		RS4AddHead( & sec->rfs_SecRefs, new );
	}

	// --
	// -- Insert Hash

	hash = ( (U64) new->rr_Address ) % MAX_REF_HASH;

	new->rr_HashPtr = sec->rfs_SecRefs2[hash];

	sec->rfs_SecRefs2[hash] = new;

	// --

	ec = RS4ErrStat_Okay;
	fs	= RS4FuncStat_Okay;

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	if ( ref_ptr )
	{
		*ref_ptr = new;
	}

	return( fs );
}

// --

enum RS4FuncStat RS4FindRef_Sec( enum RS4ErrorCode *errcode, RS4Ref **rr_ptr, RS4FileSection *sec, S64 addr )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Ref *rr;
U32 hash;

	rr = NULL;
	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

	if ( ! addr )
	{
		goto bailout;
	}

	hash = ( (U64) addr ) % MAX_REF_HASH;

	rr = sec->rfs_SecRefs2[ hash ];

	while( rr )
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
	if (( rr ) && ( rr->rr_ID != RS4ID_Ref ))
	{
		printf( "Invalid Ref ID : %08x\n", rr->rr_ID );
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

	return( fs );
}

// --

enum RS4FuncStat RS4FirstReloc_Sec( enum RS4ErrorCode *errcode, RS4Ref **rr_ptr, RS4FileSection *sec, S64 adr1, S64 adr2 )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Ref *rr;

	rr = NULL;
	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

	#ifdef DEBUG

	if ( ! sec )
	{
		fs = RS4FuncStat_Error;
		printf( "%s:%04d: Error NULL Pointer\n", __FILE__, __LINE__ );
		goto bailout;
	}

	if ( adr1 > adr2 )
	{
		fs = RS4FuncStat_Error;
		printf( "%s:%04d: Error Invalid Addresses $%08" PRIx64 "-$%08" PRIx64 "\n", __FILE__, __LINE__, adr1, adr2 );
		goto bailout;
	}

	#endif

	rr = RS4GetTail( & sec->rfs_SecRefs );

	while( rr )
	{
		if (( rr->rr_Address >= adr1 ) && ( rr->rr_Address <= adr2 ))
		{
			break;
		}
		else
		{
			rr = RS4GetPrev( rr );
		}
	}

	#ifdef DEBUG
	bailout:
	#endif

	if ( rr_ptr )
	{
		*rr_ptr = rr;
	}

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fs );
}

// --
