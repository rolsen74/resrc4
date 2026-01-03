
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

enum RS4FuncStat RS4AddLabel_File( enum RS4ErrorCode *errcode, RS4Label **rl_ptr, RS4FileHeader *fh, S64 addr, enum RS4LabelType type, STR file )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4FileSection *sec;
RS4Label *new;
S64 memadr;
S32 cnt;

	fs = RS4FuncStat_Okay;

	if ( ! addr )
	{
		goto bailout;
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
	ERR_CHK( RS4AddLabel_Sec( & ec, & new, sec, addr, type ))

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	if ( rl_ptr )
	{
		*rl_ptr = new;
	}

	return( fs );
}

// --
