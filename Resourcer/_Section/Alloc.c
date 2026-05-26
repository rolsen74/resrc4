
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

S64 RS4CurrentVirtAdr;

// --

enum RS4FuncStat
RS4AllocSection ( enum RS4ErrorCode * errcode, RS4FileSection ** sec_ptr, S32 secnr, S64 memsize )
{
	enum RS4ErrorCode ec;
	enum RS4FuncStat  fs;
	RS4FileSection *  sec;

	//	printf( "RS4AllocSection : SrcNr #%d : MemSize %ld\n", secnr, memsize );

	ec = RS4ErrStat_Error;
	fs = RS4FuncStat_Okay;

	sec = calloc ( 1, sizeof ( RS4FileSection ) );

	if ( ! sec )
	{
		ec = RS4ErrStat_OutOfMemory;

#ifdef DEBUG
		printf ( "%s:%04d: Error allocating memory (%d Bytes)\n", __FILE__, __LINE__, (S32)sizeof ( RS4FileSection ) );
#endif

		goto bailout;
	}

	sec->rfs_ID = RS4ID_FileSection;

	// --

	sec->rfs_MemoryAdr = RS4CurrentVirtAdr;

	RS4CurrentVirtAdr += 0x00010000;
	RS4CurrentVirtAdr += memsize;
	RS4CurrentVirtAdr &= 0xffff0000;

	// --

	sec->rfs_SecNr = secnr;

	if ( memsize )
	{
		// Added +10 for safety

		sec->rfs_MemorySize = memsize;
		sec->rfs_MemoryBuf	= calloc ( 1, memsize + 32 ); // BSS need to be cleared

		if ( ! sec->rfs_MemoryBuf )
		{
			ec = RS4ErrStat_OutOfMemory;

#ifdef DEBUG
			printf ( "%s:%04d: Error allocating memory (%" PRId64 " Bytes)\n", __FILE__, __LINE__, memsize );
#endif

			goto bailout;
		}

		sec->rfs_MemoryType = calloc ( 1, memsize + 32 );

		if ( ! sec->rfs_MemoryType )
		{
			ec = RS4ErrStat_OutOfMemory;

#ifdef DEBUG
			printf ( "%s:%04d: Error allocating memory (%" PRId64 " Bytes)\n", __FILE__, __LINE__, memsize );
#endif

			goto bailout;
		}

		memset ( sec->rfs_MemoryType, RS4MT_Unset, memsize );
	}

	ec = RS4ErrStat_Okay;

	// --

bailout:

	if ( ( sec ) && ( ec != RS4ErrStat_Okay ) )
	{
		RS4FreeSection ( NULL, sec );
		sec = NULL;
	}

	if ( errcode )
	{
		*errcode = ec;
	}

	if ( sec_ptr )
	{
		*sec_ptr = sec;
	}

	return ( fs );
}

// --
