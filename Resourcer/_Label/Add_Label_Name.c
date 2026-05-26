
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
RS4_Add_LabelName ( enum RS4ErrorCode * errcode, RS4FileHeader * fh, RS4Label * rl, STR name )
{
	enum RS4ErrorCode ec;
	enum RS4FuncStat  fs;
	U16				  hash;
	U32				  len;

	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Error;

	if ( ! rl )
	{
		fs = RS4FuncStat_Error;
		printf ( "label null pointer\n" );
		goto bailout;
	}

	if ( rl->rl_Name[0] )
	{
		// should proberly do a remove and then set again, so user config can overwrite
		printf ( "Label name already set : Addr $%08" PRIX64 " : Old '%s' : New '%s' :\n", rl->rl_Address, rl->rl_Name, name );
		goto bailout;
	}

	if ( ! name )
	{
		fs = RS4FuncStat_Error;
		printf ( "Label name null pointer\n" );
		goto bailout;
	}

	len = strlen ( name );

	if ( MAX_LabelName - 1 < len )
	{
		printf ( "Label name too long : '%s'\n", name );
		goto bailout;
	}

	// -- Insert Hash
	// Add into File Header

	hash = ( (U16)Calc_CRC16 ( (U8 *)name, len ) ) % MAX_LABNAME_HASH;

	rl->rl_Hash_Name_Ptr = fh->rfh_Label_Name_Hash[hash];

	fh->rfh_Label_Name_Hash[hash] = rl;

	// --

	rl->rl_NameLen = len;
	memcpy ( rl->rl_Name, name, len + 1 );

	// --

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return ( fs );
}

// --
