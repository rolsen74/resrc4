
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
// The _File version need the Address to be inside Section

RS4Label *RS4AddLabel_File( enum RS4ErrorCode *errcode, RS4FileHeader *fh, S64 addr, enum RS4LabelType type, STR file )
{
enum RS4ErrorCode ec;
RS4FileSection *sec;
RS4Label *new;
S64 memadr;
S32 cnt;

	if ( ! addr )
	{
		return( NULL );
	}

	if ( addr == 0x00081e1a )
	{
printf( "!123\n" );
	}

	// --

	ec	= RS4ErrStat_Error;
	new	= NULL;

	// --
	// First find the Hunk where the label should be

	for( cnt=0 ; cnt<fh->rfh_SecArraySize ; cnt++ )
	{
		memadr = fh->rfh_SecArray[cnt].rsi_MemoryAdr;

		if ( addr < memadr )
		{
			continue;
		}

		memadr += fh->rfh_SecArray[cnt].rsi_MemorySize;

		if ( addr > memadr )
		{
			continue;
		}

		break;
	}

	// Hunk found or is it an address outside our code?

	if ( cnt == fh->rfh_SecArraySize )
	{
		ec = RS4ErrStat_OutOfBounds;

		/**/ if ( DoVerbose > 1 )
		{
			printf( "%s:%04d: AddLabel: External? Addr $%08" PRIx64 " ?? : %s\n", __FILE__, __LINE__, addr, file );
		}
		else if ( DoVerbose > 0 )
		{
			printf( "AddLabel: External? Addr $%08" PRIx64 " ??\n", addr );
		}
		goto bailout;
	}

	// --

	sec = fh->rfh_SecArray[cnt].rsi_Section;
	new = RS4AddLabel_Sec( & ec, sec, addr, type );

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( new );
}

// --
