
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

enum RS4FuncStat RS4FreeBrance( enum RS4ErrorCode *errcode, RS4Brance *bn )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;

	ec = RS4ErrStat_Error;
	fs = RS4FuncStat_Error;

	// --

	if ( ! bn )
	{
		goto bailout;
	}

	if ( bn->rb_ID != RS4ID_Brance )
	{
		ec = RS4ErrStat_InvalidStructID;

		#ifdef DEBUG
		printf( "%s:%04d: Error Invalid Struct ID\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	// --






	free( bn );

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

RS4Brance *RS4AddBrance_File( enum RS4ErrorCode *errcode, RS4FileHeader *fh, int64_t addr )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4FileSection *sec;
RS4Brance *rb;
RS4Label *rl;

	// --

	ec = RS4ErrStat_Error;
	fs = RS4FuncStat_Error;
	rb = NULL;

	// --

	#ifdef DEBUG
	if ( ! fh )
	{
		ec = RS4ErrStat_Internal;
		printf( "%s:%04d: NULL Pointer\n", __FILE__, __LINE__ );
		goto bailout;
	}
	#endif

	// --

	sec = RS4FindSection_File( fh, addr );

	if ( ! sec )
	{
		printf( "Warning: Brancing outside of Section memory not supported ($%08lx)\n", addr );
		goto bailout;
	}

	// --

	rl = RS4AddLabel_Sec( & ec, sec, addr, RS4LabelType_Unset );

	if ( ! rl )
	{
		// ec allready set

		#ifdef DEBUG
		printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	// --

	rb = calloc( 1, sizeof( RS4Brance ));

	if ( ! rb )
	{
		ec = RS4ErrStat_OutOfMemory;

		#ifdef DEBUG
		printf( "%s:%04d: Error allocating memory (%d Bytes)\n", __FILE__, __LINE__, (int) sizeof( RS4Brance ));
		#endif

		goto bailout;
	}

	rb->rb_ID			= RS4ID_Brance;
	rb->rb_Section		= sec;
	rb->rb_Offset		= addr - sec->rfs_MemoryAdr;
	rb->rb_Address		= addr;

	#ifdef DEBUG
	#ifdef SUPPORT_M68K

	for( int cnt=0 ; cnt<8 ; cnt++ )
	{
		rb->rb_Registers[cnt+0].mr_Number = 0xd0 + cnt;
		rb->rb_Registers[cnt+8].mr_Number = 0xa0 + cnt;
	}

	#endif
	#endif

	// --
	// -- hmm should I add it here??
	RS4AddTail( & fh->rfh_BranceList, rb );

	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

bailout:

	if (( rb ) && ( fs != RS4FuncStat_Okay ))
	{
		RS4FreeBrance( NULL, rb );
		rb = NULL;
	}

	if ( errcode )
	{
		*errcode = ec;
	}

	return( rb );
}

// --
