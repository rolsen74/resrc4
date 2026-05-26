
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
RS4_Find_LabelName ( enum RS4ErrorCode * errcode, RS4FileHeader * fh, RS4Label ** rl_ptr, STR find_name )
{
	enum RS4ErrorCode ec;
	enum RS4FuncStat  fs;
	RS4Label *		  rl;
	U16				  find_hash;
	S32				  find_len;

	rl = NULL;
	ec = RS4ErrStat_Okay;
	fs = RS4FuncStat_Okay;

	if ( ! find_name )
	{
		goto bailout;
	}

	find_len  = strlen ( find_name );
	find_hash = ( (U16)Calc_CRC16 ( (U8 *)find_name, find_len ) ) % MAX_LABNAME_HASH;

	// -- Search

	rl = fh->rfh_Label_Name_Hash[find_hash];

	while ( rl )
	{
		if ( ( rl->rl_NameLen == find_len ) && ( ! memcmp ( rl->rl_Name, find_name, find_len ) ) )
		{
			break;
		}
		else
		{
			rl = rl->rl_Hash_Name_Ptr;
		}
	}

bailout:

	if ( rl_ptr )
	{
		*rl_ptr = rl;
	}

	if ( errcode )
	{
		*errcode = ec;
	}

	return ( fs );
}

// --
