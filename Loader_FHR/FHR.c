
/*
** Copyright (c) 2025 by Rene W. Olsen
**
** SPDX-License-Identifier: GPL-3.0-or-later
**
** This software is released under the GNU General Public License, version 3.
** For the full text of the license, please visit:
** https://www.gnu.org/licenses/gpl-3.0.html
**
** You can also find a copy of the license in the LICENSE file included with this software.
**
*/

// --

#include "Resourcer/ReSrc4.h"
#include "FHR.h"

// --

enum RS4FuncStat FHR_ParseFile( enum RS4ErrorCode *errcode, RS4FileHeader *fh )
{
struct FHR_Header *h;
enum RS4ErrorCode ec;
enum RS4FuncStat fs;

	fs = RS4FuncStat_Error;
	ec = RS4ErrStat_Error;

	// --
	// We have allready checked, 
	// so only fast check for now

	h = (PTR) fh->rfh_FileBuffer;

	if (( SWAP32( h->FHR_ID ) != FHR_HEADER )
	||	( SWAP16( h->FHR_OSType ) != FHR_OS_AmigaOS3 )
	||	( SWAP16( h->FHR_CPUType ) != FHR_CPU_M68k ))
	{
		printf( "This is not an Amiga Executable FHR file\n" );
	}
	else if ( h->FHR_Size == FHR_SIZE_32 )
	{
		fs = FHR_ParseFile_32( & ec, fh, h );
	}
	else
	{
		printf( "Error unsupported FHR Size %d\n", (S32) h->FHR_Size );
	}

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fs );
}

// --
