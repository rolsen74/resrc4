 
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

#define DDEBUG(x)

// --
// Overview

/*
 * 1> RS4LabelMagic_File
 * 2> RS4Parser    ( Same as SrcBuild )
 * 2> Handle_Init  ( Label Handler )
 * 2> Handle_Unset ( Label Handler )
 * 2> Handle_Code  ( Label Handler )
 * 2> Handle_Data  ( Label Handler )
 * 2> Handle_Exit  ( Label Handler )
 *
 */

// --

static S32 LabelCount = 0;

static enum RS4FuncStat RS4AdjustLabels( 
	enum RS4ErrorCode *errcode, 
	RS4FileSection *sec )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Label *SecLabel;
RS4Label *EndLabel;
RS4Label *CurLabel;
STR lab2;
S64 size;
S64 adr;
STR fmt;

	// --

	ec		= RS4ErrStat_Error;
	fs		= RS4FuncStat_Error;
	adr 	= sec->rfs_MemoryAdr;
	size	= sec->rfs_MemorySize;

	// -- We allways add a Start Label for Sections

	ERR_CHK( RS4AddLabel_Sec( & ec, & SecLabel, sec, sec->rfs_MemoryAdr, RS4LabelType_Unset ))

	if ( ! SecLabel )
	{
		// ec allready set

		#ifdef DEBUG
		printf( "%s:%04d: Error adding label\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	if ( SecLabel->rl_Name[0] == 0 )
	{
		snprintf( SecLabel->rl_Name, MAX_LabelName - 1, "%s%04X", SecNames, sec->rfs_SecNr );
		SecLabel->rl_Name[ MAX_LabelName - 1 ] = 0;
	}

	// -- Now Parse all labels in Section

	CurLabel = RS4GetHead( & sec->rfs_SecLabels );

	// Process Labels BEFORE Section Memory
	while(( CurLabel ) && ( CurLabel->rl_Offset < 0 ))
	{
		CurLabel->rl_Parent = SecLabel;

		#ifdef DEBUG
		// Label should not be shown, print dummy name
		snprintf( CurLabel->rl_Name, MAX_LabelName - 1, "!ERROR1!" );
		#endif

		CurLabel = RS4GetNext( CurLabel );
	}

	// Process Labels INSIDE Section Memory Area
	while(( CurLabel ) && ( CurLabel->rl_Offset <= size ))
	{
		if ( CurLabel->rl_Name[0] == 0 )
		{
			if ( CurLabel->rl_Parent )
			{
				#ifdef DEBUG
				// Label should not be shown, print dummy name
				snprintf( CurLabel->rl_Name, MAX_LabelName - 1, "!ERROR2!" );
				#endif
			}
			else
			{
				if ( CurLabel->rl_Type1 == RS4LabelType_String )
				{
					lab2 = "Str_";
				}
				else if ( CurLabel->rl_Label_RW_Size == RS4LABSIZE_Integer8 )
				{
					lab2 = "i08_L";
				}
				else if ( CurLabel->rl_Label_RW_Size == RS4LABSIZE_Integer16 )
				{
					lab2 = "i16_L";
				}
				else if ( CurLabel->rl_Label_RW_Size == RS4LABSIZE_Integer32 )
				{
					lab2 = "i32_L";
				}
				else if ( CurLabel->rl_Label_RW_Size == RS4LABSIZE_Float32 )
				{
					lab2 = "f32_L";
				}
				else if ( CurLabel->rl_Label_RW_Size == RS4LABSIZE_Float64 )
				{
					lab2 = "f64_L";
				}
				else
				{
					lab2 = LabNames;
				}

				if (( Sec_xDef ) && ( CurLabel->rl_xDef ))
				{
					fmt = "_%s%04X";
				}
				else
				{
					fmt = "%s%04X";
				}

				LabelCount++;
				snprintf( CurLabel->rl_Name, MAX_LabelName - 1, fmt, lab2, LabelCount );
				CurLabel->rl_Name[ MAX_LabelName - 1 ] = 0;
			}
		}

		CurLabel = RS4GetNext( CurLabel );
	}

	// Process Label AFTER Hunk Memory
	if (( CurLabel ) && ( CurLabel->rl_Offset > size ))
	{
		ERR_CHK( RS4AddLabel_Sec( & ec, & EndLabel, sec, adr+size, RS4LabelType_Unset ))

		if ( ! EndLabel )
		{ 
			// ec allready set

			#ifdef DEBUG
			printf( "%s:%04d: Error adding label\n", __FILE__, __LINE__ );
			#endif

			goto bailout;
		}

		if ( EndLabel->rl_Name[0] == 0 )
		{
			snprintf( EndLabel->rl_Name, MAX_LabelName - 1, "%s%04X", LabNames, ++LabelCount );
			EndLabel->rl_Name[ MAX_LabelName - 1 ] = 0;
		}

		while( CurLabel )
		{
			CurLabel->rl_Parent = EndLabel;

			#ifdef DEBUG
			// Label should not be shown, print dummy name
			snprintf( CurLabel->rl_Name, MAX_LabelName - 1, "!ERROR3!" );
			#endif

			CurLabel = RS4GetNext( CurLabel );
		}
	}

	// --

	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

	// --

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fs );
}

// --

static enum RS4FuncStat RS4AdjustRefDef( 
	enum RS4ErrorCode *errcode UNUSED,
	RS4FileSection *sec )
{
RS4Label *parent;
RS4Label *last;
RS4Label *rl;

	// --
	// Walk throu all section labels and make 
	// sure parent have correct xDef / xRef
	//

	rl = RS4GetHead( & sec->rfs_SecLabels );

	while( rl )
	{
		last = rl;
		parent = rl->rl_Parent;

		while( parent )
		{
			if ( last->rl_xDef )
			{
				parent->rl_xDef = TRUE;
			}

			if ( last->rl_xRef )
			{
				parent->rl_xRef = TRUE;
			}

			last = parent;
			parent = RS4GetNext( parent );
		}

		rl = RS4GetNext( rl );
	}

	return( RS4FuncStat_Okay );
}

// --

static enum RS4FuncStat RS4LabAdjust( 
	enum RS4ErrorCode *errcode, 
	RS4Trace *rt UNUSED, 
	RS4Label *rl, 
	S64 adr, 
	S64 size, 
	enum RS4LabelType type )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
//RS4FileHeader *fh;
RS4Label *parent;

	DDEBUG( printf( "\nRS4LabAdjust : 0 : RL %p, Adr $%08" PRIx64 "-$%08" PRIx64 "\n", rl, adr, adr+size-1 ); )

	ec = RS4ErrStat_Okay;
	fs = RS4FuncStat_Okay;
//	fh = rt->rt_File;

	while(( rl ) && ( rl->rl_Address < adr ))
	{
		DDEBUG( printf( "RS4LabAdjust : 1 : rl %p\n", rl ); )

		rl = RS4GetNext( rl );
	}

	if ( ! rl )
	{
		DDEBUG( printf( "RS4LabAdjust : 2 : \n" ); )
		// Not an error
		goto bailout;
	}

	/**/ if ( rl->rl_Address == adr )
	{
		parent = rl;

		DDEBUG( printf( "RS4LabAdjust : 3 : \n" ); )

		rl = RS4GetNext( rl );
	}
	else if ( rl->rl_Address < adr + size )
	{
		// Do we need a NEW Parent Label
		ERR_CHK( RS4AddLabel_Sec( & ec, & parent, rl->rl_Section, adr, type ))

		DDEBUG( printf( "RS4LabAdjust : 4 : Parent %p : Adr $%08" PRIx64 "\n", parent, adr ); )

		if ( ! parent )
		{
			fs = RS4FuncStat_Error;
			// ec allready set

			#ifdef DEBUG
			printf( "%s:%04d: Error adding label at address $%08" PRIx64 "\n", __FILE__, __LINE__, adr );
			#endif

			goto bailout;
		}
	}
	else
	{
		DDEBUG( printf( "RS4LabAdjust : 5 : \n" ); )

		// Not an error
		goto bailout;
	}

	while(( rl ) && ( rl->rl_Address < adr + size ))
	{
		DDEBUG( printf( "RS4LabAdjust : 6 : Link rl %p -> Parent %p\n", rl, parent ); )

		rl->rl_Parent = parent;
		rl = RS4GetNext( rl );
	}

	// --

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fs );
}

// --

static enum RS4FuncStat RS4Decode_Struct( 
	enum RS4ErrorCode *errcode, 
	RS4Trace *rt UNUSED,
	RS4Label *rl, 
	S64 *l )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;

	DDEBUG( printf( "Label: RS4Decode_Struct\n" ); )

	ec = RS4ErrStat_Error;
	fs = RS4FuncStat_Error;

	if ( rl->rl_Type1 != RS4LabelType_Struct )
	{
		ec = RS4ErrStat_Internal;
		fs = RS4FuncStat_Error;

		#ifdef DEBUG
		printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	ERR_CHK( RS4LabAdjust( & ec, rt, rl, rt->rt_CurMemAdr, rl->rl_Size, RS4LabelType_Unset ))

	// --

	*l = rl->rl_Size;
	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

	// --

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fs );
}

// --

static enum RS4FuncStat RS4Decode_String( 
	enum RS4ErrorCode *errcode, 
	RS4Trace *rt UNUSED,
	RS4Label *rl, 
	S64 max, 
	S64 *l )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;

	ec = RS4ErrStat_Error;
	fs = RS4FuncStat_Error;

	if ( rl->rl_Type1 != RS4LabelType_String )
	{
		ec = RS4ErrStat_Internal;
		fs = RS4FuncStat_Error;

		#ifdef DEBUG
		printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	if ( rl->rl_Size > max )
	{
		ec = RS4ErrStat_Internal;
		fs = RS4FuncStat_Error;

		#ifdef DEBUG
		printf( "%s:%04d: Error : Addr $%08" PRIx64 " (Size %" PRId64 ", Max: %" PRId64 ")\n", __FILE__, __LINE__, rl->rl_Address, rl->rl_Size, max );
		#endif

		goto bailout;
	}

	// --

	*l = rl->rl_Size;
	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

	// --

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fs );
}

// --

static enum RS4FuncStat RS4Decode_RelativeWord( 
	enum RS4ErrorCode *errcode,
	RS4Trace *rt,
	RS4Label *rl,
	S64 pos,
	S64 *l )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Label *brance;
RS4Label *rel;
S64 adr;
S64 tableadr;
S16 *tablemem;
S16 off;
S32 size;
S32 len;
S32 cnt;

	ec	= RS4ErrStat_Error;
	fs	= RS4FuncStat_Error;
	rel = rl->rl_Ref;
	len = 0;

	if ( ! rel )
	{
		ec = RS4ErrStat_Internal;
		fs = RS4FuncStat_Error;

		#ifdef DEBUG
		printf( "%s:%04d: Decode_RelativeWord Error\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	tablemem = (PTR) rl->rl_Memory;
	tableadr = rl->rl_Address;

	size = rl->rl_Size/2;

	for( cnt=0 ; cnt < size ; cnt++ )
	{
		// --

		while(( rl ) && ( rl->rl_Offset > pos ))
		{
			rl = RS4GetNext( rl );
		}

		// --

		off = SWAP16( tablemem[cnt] );
		adr = rel->rl_Address;
		adr += off;

		// --

		ERR_CHK( RS4FindLabel_File( & ec, rt->rt_File, & brance, adr, __FILE__ ))

		if ( ! brance )
		{
			ec = RS4ErrStat_Internal;
			fs = RS4FuncStat_Error;

			#ifdef DEBUG
			printf( "%s:%04d: Decode_RelativeWord Error\n", __FILE__, __LINE__ );
			#endif

			goto bailout;
		}

		// --

		ERR_CHK( RS4LabAdjust( & ec, rt, rl, tableadr, 2, RS4LabelType_Unset ))

		// --
	
		tableadr += 2;
		len += 2;
		pos += 2;
	}

	*l	= len;
	ec	= RS4ErrStat_Okay;
	fs	= RS4FuncStat_Okay;

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fs );
}

// --

static enum RS4FuncStat RS4Decode_Code(
	enum RS4ErrorCode *errcode,
	RS4Trace *rt,
	RS4Label *rl,
	S64 size,
	S64 pos,
	MEM type,
	S64 *len_ptr )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4FileSection *sec;
S32 len;
S32 cnt;

	DDEBUG( printf( "RS4Decode_Code\n" ); )

	len	= 0;
	ec	= RS4ErrStat_Error;
	fs	= RS4FuncStat_Error;
	sec	= rt->rt_Section;

	if (( rl ) && ( rl->rl_Offset == pos ))
	{
		memcpy( & rt->rt_CPU.M68k.mt_Registers, & rl->rl_Registers, 16 * sizeof( struct M68kRegister ));
	}
	else
	{
		for( cnt=0 ; cnt<16 ; cnt++ )
		{
			rt->rt_CPU.M68k.mt_Registers[cnt].mr_Type1 = RRT_Unknown;
		}
	}

	while( TRUE )
	{
		if ( type[pos] != RS4MT_Code )
		{
			break;
		}

		if ( pos >= size )
		{
			break;
		}

		while(( rl ) && ( rl->rl_Offset < pos ))
		{
			rl = RS4GetNext( rl );
		}

		rt->rt_CurMemAdr	=   sec->rfs_MemoryAdr + pos  ;
		rt->rt_CurMemBuf	= & sec->rfs_MemoryBuf [ pos ];
		rt->rt_CurMemType	= & sec->rfs_MemoryType[ pos ];

		ds = rt->rt_Decoder( & ec, rt );

		if ( ds != RS4DecodeStat_Okay )
		{
			// ec allready set

			#ifdef DEBUG
			printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
			#endif

			goto bailout;
		}

		// --

		if ( rt->rt_CPU.M68k.mt_ClearRegMask )
		{
			M68k_ClearRegs( rt, rt->rt_CPU.M68k.mt_ClearRegMask );
			rt->rt_CPU.M68k.mt_ClearRegMask = 0;
		}

		// --

		ERR_CHK( RS4LabAdjust( & ec, rt, rl, rt->rt_CurMemAdr, rt->rt_CPU.M68k.mt_OpcodeSize, RS4LabelType_Code ))

		// --

		pos += rt->rt_CPU.M68k.mt_OpcodeSize;
		len += rt->rt_CPU.M68k.mt_OpcodeSize;

		if ( rt->rt_CPU.M68k.mt_LastOpcode )
		{
			break;
		}
	}

	// --

	*len_ptr = len;

	ec	= RS4ErrStat_Okay;
	fs	= RS4FuncStat_Okay;

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fs );
}

// --

static enum RS4FuncStat RS4Decode_Data( 
	enum RS4ErrorCode *errcode,
	RS4Trace *rt,
	RS4Label *rl,
	RS4Ref **rr_ptr,
	S64 max,
	S64 *len_ptr )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Ref *rr;
S64 size;
S32 loop;
S32 mm;
S32 m;

	// --

	ec	= RS4ErrStat_Error;
	fs	= RS4FuncStat_Error;
	rr	= *rr_ptr;

	if ( rr )
	{
		if ( rr->rr_Address == rt->rt_CurMemAdr )
		{
			max = 4;			
		}
		else
		{
			m = rr->rr_Address - rt->rt_CurMemAdr;

			mm = MIN( max, m );

			if ( mm >= 4 )
			{
				max  = mm / 4;
				max *= 4;
			}
			else if (( m == 1 ) || ( m == 2 ) || ( m == 3 ))
			{
				max = mm;
			}
		}
	}

	if ( max > 3 )
	{
		// 1 Long or more

		if (( rr ) && ( rr->rr_Address == rt->rt_CurMemAdr ))
		{
			size = 4;

			ERR_CHK( RS4LabAdjust( & ec, rt, rl, rt->rt_CurMemAdr, size, RS4LabelType_Unset ))
		}
		else
		{
			loop = ( max >= 16 ) ? 4 : max / 4;

			size = loop * 4;
		}
	}
	else if ( max > 1 )
	{
		// 1 Word

		size = 2;
	}
	else
	{
		// 1 Byte

		size = 1;
	}

	*len_ptr = size;

	// --

	fs	= RS4FuncStat_Okay;
	ec	= RS4ErrStat_Okay;

	// --

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fs );
}

// --
// Section Init : Header

static enum RS4FuncStat Handle_Init( enum RS4ErrorCode *errcode UNUSED, RS4Trace *rt UNUSED )
{
	*errcode = RS4ErrStat_Okay;

	return( RS4FuncStat_Okay );
}

// --
// Section Exit : Bottom

static enum RS4FuncStat Handle_Exit( 	
		enum RS4ErrorCode *errcode, 
UNUSED	RS4Trace *rt,		// Trace info
UNUSED	RS4Label **rlptr,	// Current Label
UNUSED	RS4Ref **rrptr,		// Current Ref
UNUSED	MEM type,			// MemType
UNUSED	S64 size,			// MemSize
UNUSED	S64 max,			// max bytes ( too .. next label, next ref, sec end, type change )
UNUSED	S64 pos )			// mem/type pos
{
	*errcode = RS4ErrStat_Okay;

	return( RS4FuncStat_Okay );
}

// --
// len == 0 is Error

static S64 Handle_Unset( 
		enum RS4ErrorCode *errcode,	 
		RS4Trace *rt,		// Trace info	
		RS4Label **rlptr,	// Current Label
		RS4Ref **rrptr,		// Current Ref
UNUSED	MEM type,			// MemType
UNUSED	S64 size,			// MemSize
		S64 max,			// max bytes ( too .. next label, next ref, sec end, type change )
		S64 pos )			// mem/type pos
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4FileSection *sec;
RS4Label *rl;
S64 len;
S64 l;

	l	= 0;
	ec	= RS4ErrStat_Error;
	sec	= rt->rt_Section;
	rl	= *rlptr;

	// --

	if (( sec->rfs_SecType == RS4ST_BSS )
	||	( sec->rfs_DataSize <= pos ))
	{
		// Handle BSS 
		len = max;
	}
	else
	{
		ERR_CHK( RS4Decode_Data( & ec, rt, rl, rrptr, max, & len ))
	}

	// --

	ec	= RS4ErrStat_Okay;
	l	= len;

	// --

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( l );
}

// --

static S64 Handle_Code( 
		enum RS4ErrorCode *errcode, 
UNUSED	RS4Trace *rt,		// Trace info
UNUSED	RS4Label **rlptr,	// Current Label
UNUSED	RS4Ref **rrptr,		// Current Ref
UNUSED	MEM type,		// MemType
UNUSED	S64 size,		// MemSize
UNUSED	S64 max,		// max bytes ( too .. next label, next ref, sec end, type change )
UNUSED	S64 pos )		// mem/type pos
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Label *rl;
S64 len;
S64 l;

	DDEBUG( printf( "Handle_Code\n" ); )

	l	= 0;
	ec	= RS4ErrStat_Error;
	rl	= *rlptr;
	len	= 0;

	// --

	ERR_CHK( RS4Decode_Code( & ec, rt, rl, size, pos, type, & len ))

	// --

	ec	= RS4ErrStat_Okay;
	l	= len;

	// --

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( l );
}

// --

static S64 Handle_Data( 
	enum RS4ErrorCode *errcode, 
	RS4Trace *rt,		// Trace info
	RS4Label **rlptr,	// Current Label
	RS4Ref **rrptr,		// Current Ref
	S64 max,			// max bytes ( too .. next label, next ref, sec end, type change )
	S64 pos )			// mem/type pos
{
enum RS4LabelType t;
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Label *rl;
S64 len;
S64 l;

	l	= 0;
	ec	= RS4ErrStat_Error;

	// --

	rl = *rlptr;

	t = (( rl ) && ( rl->rl_Offset == pos )) ? rl->rl_Type1 : RS4LabelType_Unknown ;

	switch( t )
	{
		case RS4LabelType_RelativeWord:
		{
			ERR_CHK( RS4Decode_RelativeWord( & ec, rt, rl, pos, & len ))
			break;
		}

		case RS4LabelType_String:
		{
			ERR_CHK( RS4Decode_String( & ec, rt, rl, max, & len ))
			break;
		}

		case RS4LabelType_Struct:
		{
			ERR_CHK( RS4Decode_Struct( & ec, rt, rl, & len ))
			break;
		}

		case RS4LabelType_Unknown:
		{
			ERR_CHK( RS4Decode_Data( & ec, rt, rl, rrptr, max, & len ))
			break;
		}
	
		default:
		{
			ec = RS4ErrStat_Internal;
			fs = RS4FuncStat_Error;

			#ifdef DEBUG
			printf( "%s:%04d: Error : Addr $%08" PRIx64 " (%d)\n", __FILE__, __LINE__, rl->rl_Address, rl->rl_Type1 );
			#endif

			goto bailout;
		}
	}

	// --

	ec	= RS4ErrStat_Okay;
	l	= len;

	// --

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( l );
}

// --

static enum RS4FuncStat RS4Parser( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4FileSection *sec;
RS4Label *nextrl;
RS4Label *rl;
RS4Ref *nextrr;
RS4Ref *rr;
MEM type;
MEM mem;
S64 size;
S64 pos;
S64 len;
S64 max;
S64 cnt;
S64 mlen;
U8 mtyp;

	// --

	DDEBUG( printf( "RS4Parser\n" ); )

	ERR_CHK( Handle_Init( & ec, rt ))

	// --

	sec		= rt->rt_Section;
	rl		= RS4GetHead( & sec->rfs_SecLabels );
	rr		= RS4GetHead( & sec->rfs_SecRefs );
	size	= sec->rfs_MemorySize;
	type	= sec->rfs_MemoryType;
	mem		= sec->rfs_MemoryBuf;
	pos		= 0;

	while( TRUE )
	{
		// --

		if ( pos >= size )
		{
			break;
		}

		// --

		while(( rl ) && ( rl->rl_Offset < pos ))
		{
			rl = RS4GetNext( rl );
		}

		while(( rr ) && ( rr->rr_Offset < pos ))
		{
			rr = RS4GetNext( rr );
		}

		// -- Set Label

		if (( rl ) && ( rl->rl_Offset == pos ))
		{
			rt->rt_Container.Hunk.ms_Ptr_Label = rl;
		}
		else
		{
			rt->rt_Container.Hunk.ms_Ptr_Label = NULL;
		}

		// ----- Calc max Bytes
		// -- Either to 'End of Section', 'Next Label' or next Data Type Change

		max = size - pos;

		// -- Labels

		if ( rl )
		{
			if ( rl->rl_Offset > pos )
			{
				max = MIN( max, rl->rl_Offset - pos );
			}
			else
			{
				nextrl = RS4GetNext( rl );

				if ( nextrl )
				{
					max = MIN( max, nextrl->rl_Offset - pos );
				}
			}
		}

		// -- Refs

		if ( rr )
		{
			if ( rr->rr_Offset > pos )
			{
				max = MIN( max, rr->rr_Offset - pos );
			}
			else
			{
				nextrr = RS4GetNext( rr );

				if ( nextrr )
				{
					max = MIN( max, nextrr->rr_Offset - pos );
				}
			}
		}

		// -- Memory Type

		mlen = 0;
		mtyp = type[ pos ];

		for( cnt=pos ; cnt<pos+max ; cnt++ )
		{
			if ( type[ cnt ] != mtyp )
			{
				max = mlen;
				break;
			}
			else
			{
				mlen++;
			}
		}

		// --

		#ifdef DEBUG
		rt->rt_Container.Hunk.ms_StartAddr = -1ULL;
		#endif

		rt->rt_CurMemAdr	= sec->rfs_MemoryAdr + pos;
		rt->rt_CurMemBuf	= & mem[ pos ];
		rt->rt_CurMemType	= & type[ pos ];

		// --

		len = 0;
		fs = RS4FuncStat_Error;

		switch( type[pos] )
		{
			case RS4MT_Unset:
			{
				len = Handle_Unset( & ec, rt, & rl, & rr, type, size, max, pos );

				#ifdef DEBUG
				if ( len <= 0 )
				{
					printf( "%s:%04d: Length error (%" PRId64 ")\n", __FILE__, __LINE__, len );
					goto bailout;
				}
				#endif

				break;
			}

			case RS4MT_Code:
			{
				len = Handle_Code( & ec, rt, & rl, & rr, type, size, max, pos );

				#ifdef DEBUG
				if ( len <= 0 )
				{
					printf( "%s:%04d: Length error (%" PRId64 ")\n", __FILE__, __LINE__, len );
					goto bailout;
				}
				#endif

				break;
			}

			case RS4MT_Data:
			{
				len = Handle_Data( & ec, rt, & rl, & rr, max, pos );

				#ifdef DEBUG
				if ( len <= 0 )
				{
					printf( "%s:%04d: Length error (%" PRId64 ")\n", __FILE__, __LINE__, len );
					goto bailout;
				}
				#endif

				break;
			}

			default:
			{
				ec = RS4ErrStat_Internal;
				#ifdef DEBUG
				printf( "%s:%04d: Unknown Memory Type (%d)\n", __FILE__, __LINE__, type[ pos ] );
				#endif
				break;
			}
		}

		if ( len <= 0 )
		{
			// fs allready set
			// ec allready set

			#ifdef DEBUG
			printf( "%s:%04d: Length error (%" PRId64 ")\n", __FILE__, __LINE__, len );
			#endif

			goto bailout;
		}

		pos += len;

		// --
	}

	// --

	ERR_CHK( Handle_Exit( & ec, rt, & rl, & rr, type, size, max, pos ))

	// --

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fs );
}

// --

enum RS4FuncStat RS4LabelMagic_File( enum RS4ErrorCode *errcode, RS4FileHeader *fh )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4FileSection *sec;
RS4Label *rl;
RS4Trace rt;
CHR argbuf[256];
S32 cnt;

	// --

	ec = RS4ErrStat_Error;
	fs = RS4FuncStat_Error;

	// --

	DDEBUG( printf( "RS4LabelMagic_File\n" ); )

	ERR_CHK( RS4InitTrace( & ec, & rt, fh, RS4TracePass_Label ))

	rt.rt_Container.Hunk.ms_Buf_Argument = & argbuf[0];

	// --
	// -- Pass all the other Sections

	for( cnt=0 ; cnt < fh->rfh_SecArraySize ; cnt++ )
	{
		sec = fh->rfh_SecArray[ cnt ].rsi_Section;

		rt.rt_Section = sec;

		ERR_CHK( RS4Parser( & ec, & rt ))
		ERR_CHK( RS4AdjustRefDef( & ec, sec ))
	}

	// --
	// -- Create External Label Names

	rl = RS4GetHead( & fh->rfh_ExtLabelList );

	while( rl )
	{
		/**/ if ( rl->rl_Offset < 0x00010000 )
		{
			snprintf( rl->rl_Name, MAX_LabelName - 1, "%s%04" PRIX64 "", ExtNames, rl->rl_Offset );
		}
		else if ( rl->rl_Offset < 0x01000000 )
		{
			snprintf( rl->rl_Name, MAX_LabelName - 1, "%s%06" PRIX64 "", ExtNames, rl->rl_Offset );
		}
		else
		{
			snprintf( rl->rl_Name, MAX_LabelName - 1, "%s%08" PRIX64 "", ExtNames, rl->rl_Offset );
		}

		rl = RS4GetNext( rl );
	}

	// --
	// -- Create Hunk Label Names

	for( cnt=0 ; cnt < fh->rfh_SecArraySize ; cnt++ )
	{
		sec = fh->rfh_SecArray[ cnt ].rsi_Section;

		ERR_CHK( RS4AdjustLabels( & ec, sec ))
	}

	// --

	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

	// --

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fs );
}

// --
