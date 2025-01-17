
/*
 * Copyright (c) 2014-2025 Rene W. Olsen < renewolsen @ gmail . com >
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

enum RS4FuncStat RS4FreeLabel( enum RS4ErrorCode *errcode, RS4Label *rl )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;

	ec = RS4ErrStat_Error;
	fs = RS4FuncStat_Error;

	// --

	if ( ! rl )
	{
		goto bailout;
	}

	if ( rl->rl_ID != RS4ID_Label )
	{
		ec = RS4ErrStat_InvalidStructID;

		#ifdef DEBUG
		printf( "%s:%04d: Error Invalid Struct ID\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	rl->rl_ID = 0;

	// --



	free( rl );

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
// The _File version need the Address to be inside Section

RS4Label *RS4AddLabel_File( enum RS4ErrorCode *errcode, RS4FileHeader *fh, int64_t addr, enum RS4LabelType type )
{
enum RS4ErrorCode ec;
RS4FileSection *sec;
RS4Label *new;
int64_t memadr;
int cnt;

	if ( ! addr )
	{
		return( NULL );
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
		printf( "AddLabel: External? Addr $%08" PRIx64 " ??\n", addr );
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
// The _Sec version can handle addesses out side Sections

RS4Label *RS4AddLabel_Sec( enum RS4ErrorCode *errcode, RS4FileSection *sec, int64_t addr, enum RS4LabelType type )
{
enum RS4ErrorCode ec;
RS4FileHeader *fh;
RS4Label *new;
RS4Label *rl;
int hash;

	if ( ! addr )
	{
		return( NULL );
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
			}
			else if ( rl->rl_Type1 != type )
			{
				printf( "%s:%04d: rl->rl_Address $%08" PRIx64 " (Type: %d)\n", __FILE__, __LINE__, rl->rl_Address, type );
				rl->rl_Type1 = RS4LabelType_Unknown;
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
		ec = RS4ErrStat_OutOfMemory;

		#ifdef DEBUG
		printf( "%s:%04d: Error allocating Memory (%d Bytes)\n", __FILE__, __LINE__, (int) sizeof( RS4Label ));
		#endif

		goto bailout;
	}

	new->rl_ID			= RS4ID_Label;
	new->rl_Type1		= type;
	new->rl_Section		= sec;
	new->rl_Address		= addr;
	new->rl_Offset		= addr - sec->rfs_MemoryAdr;
	new->rl_Memory		= & sec->rfs_MemoryBuf[ new->rl_Offset ];

	if ( Verbose > 1 )
	{
		printf( "New Label : Hunk #%d : Address $%08" PRIx64 "\n", sec->rfs_SecNr, addr );
	}

	#if 0
	if ( new->rl_Address < sec->rfs_MemoryAdr )
	{
		printf( "New label before Section start : LabAdr $%08" PRIx64 ", SecAdr $%08" PRIx64 "\n", 
		new->rl_Address, sec->rfs_MemoryAdr );
	}

	if ( sec->rfs_MemorySize < new->rl_Offset )
	{
		printf( "New label after Section end : LabAdr $%08" PRIx64 ", SecAdr $%08" PRIx64 "\n", 
		new->rl_Address, sec->rfs_MemoryAdr+sec->rfs_MemorySize );
	}
	#endif

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

	hash = ( (uint64_t) addr ) % MAX_LAB_HASH;

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

	return(	new );
}

// --

RS4Label *RS4FindLabel_File( RS4FileHeader *fh, int64_t addr )
{
RS4Label *rl;
int hash;

	if ( ! addr )
	{
		return( NULL );
	}

	hash = ( (uint64_t) addr ) % MAX_LAB_HASH;

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

	if ( Verbose > 1 )
	{
		if ( ! rl )
		{
			printf( "Label NOT Found : Address $%08" PRIx64 "\n", addr );
		}
	}

	#ifdef DEBUG
	if (( rl ) && ( rl->rl_ID != RS4ID_Label ))
	{
		printf( "Invalid Label ID : %08x\n", rl->rl_ID );
		rl = NULL;
	}
	#endif

	return(	rl );
}

// --
