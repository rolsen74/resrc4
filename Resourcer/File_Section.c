
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

enum RS4FuncStat
RS4FindSection_File ( enum RS4ErrorCode * errcode, RS4FileSection ** sec_ptr, RS4FileHeader * fh, S64 addr )
{
	enum RS4ErrorCode ec;
	enum RS4FuncStat  fs;
	RS4FileSection *  sec;
	S64				  memadr;
	S32				  cnt;

	ec	= RS4ErrStat_Okay;
	fs	= RS4FuncStat_Okay;
	sec = NULL;

	if ( ! addr )
	{
		goto bailout;
	}

	for ( cnt = 0; cnt < fh->rfh_SecArraySize; cnt++ )
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
		sec = NULL;
	}
	else
	{
		sec = fh->rfh_SecArray[cnt].rsi_Section;
	}

#ifdef DEBUG
	if ( ( sec ) && ( sec->rfs_ID != RS4ID_FileSection ) )
	{
		printf ( "Invalid Section ID : %08x\n", sec->rfs_ID );
		sec = NULL;
	}
#endif

bailout:

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
