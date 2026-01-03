
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
// The _Sec version can handle addesses out side Sections

enum RS4FuncStat RS4AddLabel_Sec( enum RS4ErrorCode *errcode, RS4Label **rl_ptr, RS4FileSection *sec, S64 addr, enum RS4LabelType type )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4FileHeader *fh;
RS4Label *new;
RS4Label *rl;
U32 hash;

	fs = RS4FuncStat_Okay;

	new = NULL;

	if ( ! addr )
	{
		goto bailout;
	}

	new = NULL;
	ec	= RS4ErrStat_Error;
	fh	= sec->rfs_File;

	// Search Label list, travling from bottom and up
	// The list is sorted so we can stop when address become smaller
	rl = RS4GetTail( & sec->rfs_SecLabels );

	while( rl )
	{
		if ( rl->rl_Address <= addr )
		{
			break;
		}
		else
		{
			rl = RS4GetPrev( rl );
		}
	}

	// Is there already a label with this address?
	if (( rl ) && ( rl->rl_Address == addr ))
	{
		if ( type != RS4LabelType_Unset )
		{
			/**/ if ( rl->rl_Type1 == RS4LabelType_Unset )
			{
				// Set type
				rl->rl_Type1 = type;
				rl->rl_Type2 = 0;
				rl->rl_Type3 = 0;
			}
			else if ( rl->rl_Type1 != type )
			{
//				printf( "%s:%04d: rl->rl_Address $%08" PRIx64 " (Type: %d)\n", __FILE__, __LINE__, rl->rl_Address, type );
				rl->rl_Type1 = RS4LabelType_Unknown;
				rl->rl_Type2 = 0;
				rl->rl_Type3 = 0;
				// we have multible types
			}
		}

		new = rl;
		ec	= RS4ErrStat_Okay;
		goto bailout;
	}

	// Create new label

	new = calloc( 1, sizeof( RS4Label ));

	if ( ! new )
	{
		fs = RS4FuncStat_Error;
		ec = RS4ErrStat_OutOfMemory;

		#ifdef DEBUG
		printf( "%s:%04d: Error allocating Memory (%d Bytes)\n", __FILE__, __LINE__, (S32) sizeof( RS4Label ));
		#endif

		goto bailout;
	}

	new->rl_ID			= RS4ID_Label;
	new->rl_Type1		= type;
	new->rl_Section		= sec;
	new->rl_Address		= addr;
	new->rl_Offset		= addr - sec->rfs_MemoryAdr;
	new->rl_Memory		= & sec->rfs_MemoryBuf[ new->rl_Offset ];
	new->rl_Label_RW_Size = RS4LABSIZE_Unset;

	if ( DoVerbose > 3 )
	{
		printf( "New Label : Hunk #%d : Address $%08" PRIx64 "\n", sec->rfs_SecNr, addr );
	}

	// Insert node at correct place in the sorted list
	// Add into File Section

	if ( rl )
	{
		RS4AddAfter( & sec->rfs_SecLabels, rl, new );
	}
	else
	{
		RS4AddHead( & sec->rfs_SecLabels, new );
	}

	// -- Insert Hash
	// Add into File Header

	hash = ( (U64) addr ) % MAX_LAB_HASH;

	new->rl_HashPtr = fh->rfh_LabelHash[hash];

	fh->rfh_LabelHash[hash] = new;

	// --

	ec	= RS4ErrStat_Okay;

	// --

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

	return(	fs );
}

// --
