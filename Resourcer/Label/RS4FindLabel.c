
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

enum RS4FuncStat RS4FindLabel_File( enum RS4ErrorCode *errcode, RS4FileHeader *fh, RS4Label **rl_ptr, S64 addr, STR file )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Label *rl;
U32 hash;

	rl = NULL;

	ec = RS4ErrStat_Okay;
	fs = RS4FuncStat_Okay;

	if ( ! addr )
	{
		goto bailout;
	}

	hash = ( (U64) addr ) % MAX_LAB_HASH;

	rl = fh->rfh_LabelHash[ hash ];

	while( rl )
	{
		if ( rl->rl_Address == addr )
		{
			break;
		}
		else
		{
			rl = rl->rl_HashPtr;
		}
	}

	if ( DoVerbose > 2 )
	{
		if ( ! rl )
		{
			printf( "Label NOT Found : Address $%08" PRIx64 ": File '%s'\n", addr, file );
		}
	}

	#ifdef DEBUG
	if (( rl ) && ( rl->rl_ID != RS4ID_Label ))
	{
		printf( "Invalid Label ID : %08x\n", rl->rl_ID );
		rl = NULL;
	}
	#endif

bailout:

	if ( rl_ptr )
	{
		*rl_ptr = rl;
	}

	if ( errcode )
	{
		*errcode = ec;
	}

	return(	fs );
}

// --
