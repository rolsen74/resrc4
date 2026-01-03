 
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
 * 2> RS4Parser    ( Same as Label )
 * 2> Handle_Init  ( Source Handler )
 * 2> Handle_Unset ( Source Handler )
 * 2> Handle_Code  ( Source Handler )
 * 2> Handle_Data  ( Source Handler )
 * 2> Handle_Exit  ( Source Handler )
 *
 */

// --

static enum RS4FuncStat RS4AddSourceLine( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
struct _SrcCode *sc;
RS4Source *rs;
STR str;
S32 comlen;
S32 len;

	DDEBUG( { printf( "%s:%04d: RS4AddSourceLine\n", __FILE__, __LINE__ ); } )

	ec = RS4ErrStat_Error;
	fs = RS4FuncStat_Error;

	#ifdef DEBUG
	if (( ! rt ) 
	||	( ! rt->rt_Container.Hunk.ms_Str_Opcode )
	||	( ! rt->rt_Container.Hunk.ms_Buf_Argument ))
	{
		ec = RS4ErrStat_Internal;
		printf( "%s:%04d: NULL Pointer\n", __FILE__, __LINE__ );
		printf( "rt ................. : %p\n", rt );
		printf( "ms_Str_Opcode ...... : %p\n", rt->rt_Container.Hunk.ms_Str_Opcode );
		printf( "ms_Buf_Argument .... : %p\n", rt->rt_Container.Hunk.ms_Buf_Argument );
		goto bailout;
	}
	#endif

 	len = strlen( rt->rt_Container.Hunk.ms_Buf_Argument ) + 1;

 	if ( rt->rt_Container.Hunk.ms_Buf_Comment )
 	{
 		comlen = strlen( rt->rt_Container.Hunk.ms_Buf_Comment ) + 1;
 	}
 	else
 	{
 		comlen = 1;
 	}

 	len += comlen;

 	rs = calloc( 1, sizeof( RS4Source ) + len );

 	if ( ! rs )
 	{
		ec = RS4ErrStat_OutOfMemory;

		#ifdef DEBUG
 		printf( "%s:%04d: Error allocating memory (%d)\n", __FILE__, __LINE__, (S32) sizeof( RS4Source ) + len );
		#endif

 		goto bailout;
 	}

	str = (STR )( rs + 1 );

	// -- Init Code

	sc = & rs->rs_Data.rs_Code;

 	sc->rs_Address	= rt->rt_Container.Hunk.ms_StartAddr;
 	sc->rs_Opcode	= rt->rt_Container.Hunk.ms_Str_Opcode;
 	sc->rs_Label	= rt->rt_Container.Hunk.ms_Ptr_Label;
 	sc->rs_Comment	= str;
 	sc->rs_Argument = str + comlen;

 	if ( rt->rt_Container.Hunk.ms_Buf_Comment )
 	{
 		strcpy( sc->rs_Comment, rt->rt_Container.Hunk.ms_Buf_Comment );
 	}

 	strcpy( sc->rs_Argument, rt->rt_Container.Hunk.ms_Buf_Argument );

	// --

	rs->rs_Type = RS4SourceType_Code;
 	RS4AddTail( & rt->rt_Section->rfs_SourceList, rs );

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

static enum RS4FuncStat RS4AddSourceString( enum RS4ErrorCode *errcode, RS4Trace *rt, CSTR fmt, ... )
{
struct _SrcString *ss;
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Source *rs;
va_list args;
STR str;
S32 len;

	fs = RS4FuncStat_Error;
	ec = RS4ErrStat_Error;

	// -- Get Size

	va_start( args, fmt );
	len = vsnprintf( NULL, 0, fmt, args );
	va_end( args );

	// --

	rs = calloc( 1, sizeof( RS4Source ) + len + 1 );

	if ( ! rs )
	{
		ec = RS4ErrStat_OutOfMemory;

		#ifdef DEBUG
		printf( "%s:%04d: Error allocating memory (%d)\n", __FILE__, __LINE__, (S32) sizeof( RS4Source ) + len );
		#endif

		goto bailout;
	}

	// -- Init Struct

	str = (STR )( rs + 1 );

	ss = & rs->rs_Data.rs_String;

	ss->rs_String = str;

	// -- Fill String Buffer

	va_start( args, fmt );
	vsnprintf( str, len + 1, fmt, args );
	va_end( args );

	// -- Add Node to Header

	rs->rs_Type = RS4SourceType_String;
	RS4AddTail( & rt->rt_Section->rfs_SourceList, rs );

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

static enum RS4FuncStat RS4AddEmptyLine( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;

	DDEBUG( { printf( "%s:%04d: RS4AddEmptyLine\n", __FILE__, __LINE__ ); } )

	ec	= RS4ErrStat_Error;
	fs	= RS4FuncStat_Error;

	#ifdef DEBUG
	if (( ! rt ) 
	||	( ! rt->rt_Container.Hunk.ms_Buf_Argument ))
	{
		ec = RS4ErrStat_Internal;
		printf( "%s:%04d: NULL Pointer\n", __FILE__, __LINE__ );
		printf( "rt ................. : %p\n", rt );
		printf( "ms_Buf_Argument .... : %p\n", rt->rt_Container.Hunk.ms_Buf_Argument );
		goto bailout;
	}
	#endif

	rt->rt_Container.Hunk.ms_StartAddr = 0;
	rt->rt_Container.Hunk.ms_Ptr_Label = NULL;
	rt->rt_Container.Hunk.ms_Str_Opcode = "";
	rt->rt_Container.Hunk.ms_Buf_Argument[0] = 0;

	ERR_CHK( RS4AddSourceLine( & ec, rt ))

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fs );
}

// --

static enum RS4FuncStat __Struct_Ptr( enum RS4ErrorCode *errcode, struct DataStructNode *dsn, RS4Trace *rt )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4FileSection *sec;
RS4Label *rl;
S64 val;
STR buf;
S32 len;

	fs	= RS4FuncStat_Error;
	ec	= RS4ErrStat_Error;

	buf = rt->rt_Container.Hunk.ms_Buf_Argument;

	val	= ( (U64) rt->rt_CurMemBuf[ 0 ] << 24 | 
			(U64) rt->rt_CurMemBuf[ 1 ] << 16 | 
			(U64) rt->rt_CurMemBuf[ 2 ] <<  8 | 
			(U64) rt->rt_CurMemBuf[ 3 ] <<  0 );

	sec = rt->rt_Section;

	ERR_CHK( RS4FindLabel_File( & ec, rt->rt_File, & rl, val, __FILE__ ))

	if ( sec->rfs_SecType == RS4ST_BSS )
	{
		if ( rl )
		{
			fs	= RS4FuncStat_Error;
			ec	= RS4ErrStat_Error;

			#ifdef DEBUG
			printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
			#endif

			goto bailout;
		}

		sprintf( buf, "4" );
	}
	else
	{
		if ( rl )
		{
			ERR_CHK( RS4BuildLabelString( & ec, rl, buf ))
		}
		else
		{
			sprintf( buf, "$%08" PRIx64 "", val );
		}
	}

	// --

	len = strlen( buf );

	if ( len < 8 )
	{
		sprintf( & buf[len], "\t\t; %s", dsn->dsn_Name );
	}
	else 
	{
		sprintf( & buf[len], "\t; %s", dsn->dsn_Name );
	}

	// --

	sec = rt->rt_Section;

	if ( sec->rfs_SecType == RS4ST_BSS )
	{
		rt->rt_Container.Hunk.ms_Str_Opcode = "ds.b";
	}
	else
	{
		rt->rt_Container.Hunk.ms_Str_Opcode = "dc.l";
	}

	// --

	rt->rt_Container.Hunk.ms_StartAddr = rt->rt_CurMemAdr;

	ERR_CHK( RS4AddSourceLine( & ec, rt ))

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fs );
}

// --

static enum RS4FuncStat __Struct_Byte( enum RS4ErrorCode *errcode, struct DataStructNode *dsn, RS4Trace *rt )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4FileSection *sec;
S64 val;
STR buf;
S32 len;

	buf = rt->rt_Container.Hunk.ms_Buf_Argument;

	val	= ( (U64) rt->rt_CurMemBuf[ 0 ] );

	sec = rt->rt_Section;

	if ( sec->rfs_SecType == RS4ST_BSS )
	{
		sprintf( buf, "1" );
	}
	else
	{
		sprintf( buf, "$%02" PRIx64 "", val );
	}

	len = strlen( buf );

	if ( len < 8 )
	{
		sprintf( & buf[len], "\t\t; %s", dsn->dsn_Name );
	}
	else 
	{
		sprintf( & buf[len], "\t; %s", dsn->dsn_Name );
	}

	// --

	if ( sec->rfs_SecType == RS4ST_BSS )
	{
		rt->rt_Container.Hunk.ms_Str_Opcode = "ds.b";
	}
	else
	{
		rt->rt_Container.Hunk.ms_Str_Opcode = "dc.b";
	}

	// --

	rt->rt_Container.Hunk.ms_StartAddr = rt->rt_CurMemAdr;

	ERR_CHK( RS4AddSourceLine( & ec, rt ))

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fs );
}

// --

static enum RS4FuncStat __Struct_Word( enum RS4ErrorCode *errcode, struct DataStructNode *dsn, RS4Trace *rt )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4FileSection *sec;
S64 val;
STR buf;
S32 len;

	buf = rt->rt_Container.Hunk.ms_Buf_Argument;

	val	= ( (U64) rt->rt_CurMemBuf[ 0 ] << 8 | 
			(U64) rt->rt_CurMemBuf[ 1 ] << 0 );

	sec = rt->rt_Section;

	if ( sec->rfs_SecType == RS4ST_BSS )
	{
		sprintf( buf, "2" );
	}
	else
	{
		sprintf( buf, "$%04" PRIx64 "", val );
	}

	len = strlen( buf );

	if ( len < 8 )
	{
		sprintf( & buf[len], "\t\t; %s", dsn->dsn_Name );
	}
	else 
	{
		sprintf( & buf[len], "\t; %s", dsn->dsn_Name );
	}

	// --

	if ( sec->rfs_SecType == RS4ST_BSS )
	{
		rt->rt_Container.Hunk.ms_Str_Opcode = "ds.b";
	}
	else
	{
		rt->rt_Container.Hunk.ms_Str_Opcode = "dc.w";
	}

	// --

	rt->rt_Container.Hunk.ms_StartAddr = rt->rt_CurMemAdr;

	ERR_CHK( RS4AddSourceLine( & ec, rt ))

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fs );
}

// --

static enum RS4FuncStat __Struct_Long( enum RS4ErrorCode *errcode, struct DataStructNode *dsn, RS4Trace *rt )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4FileSection *sec;
S64 val;
STR buf;
S32 len;

	buf = rt->rt_Container.Hunk.ms_Buf_Argument;

	val	= ( (U64) rt->rt_CurMemBuf[ 0 ] << 24 | 
			(U64) rt->rt_CurMemBuf[ 1 ] << 16 |
			(U64) rt->rt_CurMemBuf[ 2 ] <<  8 |
			(U64) rt->rt_CurMemBuf[ 3 ] <<  0 );

	sec = rt->rt_Section;

	if ( sec->rfs_SecType == RS4ST_BSS )
	{
		sprintf( buf, "4" );
	}
	else
	{
		sprintf( buf, "$%08" PRIx64 "", val );
	}

	len = strlen( buf );

	if ( len < 8 )
	{
		sprintf( & buf[len], "\t\t; %s", dsn->dsn_Name );
	}
	else 
	{
		sprintf( & buf[len], "\t; %s", dsn->dsn_Name );
	}

	// --

	if ( sec->rfs_SecType == RS4ST_BSS )
	{
		rt->rt_Container.Hunk.ms_Str_Opcode = "ds.b";
	}
	else
	{
		rt->rt_Container.Hunk.ms_Str_Opcode = "dc.l";
	}

	// --

	rt->rt_Container.Hunk.ms_StartAddr = rt->rt_CurMemAdr;

	ERR_CHK( RS4AddSourceLine( & ec, rt ))

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fs );
}

// --

static enum RS4FuncStat RS4Decode_Struct( enum RS4ErrorCode *errcode, RS4Trace *rt, RS4Label *rl, S64 off, S64 *lenptr )
{
struct DataStructHeader *dsh;
struct DataStructNode *dsn;
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4FileSection *sec;
MEM mem;
S64 adr;
S32 cnt;
S32 len;
S32 id;

	ec	= RS4ErrStat_Error;
	fs	= RS4FuncStat_Error;
	sec	= rl->rl_Section;
	id	= rl->rl_Type2;
	adr	= sec->rfs_MemoryAdr;
	mem	= sec->rfs_MemoryBuf;
	dsh	= DataStructTable[id];
	len = 0;

//	printf( "Build: RS4Decode_Struct : StructID %d : %s\n", id, (dsh)?dsh->dsh_Title:"" );

	if ( rl->rl_Type1 != RS4LabelType_Struct )
	{
		ec = RS4ErrStat_Internal;
		fs = RS4FuncStat_Error;

		#ifdef DEBUG
		printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	// --

	#ifdef DEBUG
	if ( rl->rl_Size != dsh->dsh_Size )
	{
		ec = RS4ErrStat_Internal;
		fs = RS4FuncStat_Error;

		#ifdef DEBUG
		printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
		#endif
		goto bailout;
	}
	#endif

	// --

	ERR_CHK( RS4AddSourceString( & ec, rt, "\n; %s\n", dsh->dsh_Title ) )

	// --

	for( cnt=0 ; cnt < dsh->dsh_Entries ; cnt++ )
	{
		dsn = & dsh->dsh_Data[cnt];

//	printf( "--> %s\n", dsn->dsn_Name );

		rt->rt_CurMemAdr = adr + off;
		rt->rt_CurMemBuf = & mem[ off ];

		switch( dsn->dsn_Type )
		{
			#if 0
			case DST_Data:
			{
				fs = __Struct_Data( & ec, rt, off );

				if ( fs != RS4FuncStat_Okay )
				{
					#ifdef DEBUG
					printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
					#endif
					goto bailout;
				}

				off += dsn->dsn_Size;
				break;
			}
			#endif

			case DST_Byte:
			{
				ERR_CHK( __Struct_Byte( & ec, dsn, rt ))
				break;
			}

			case DST_Word:
			{
				ERR_CHK( __Struct_Word( & ec, dsn, rt ))
				break;
			}

			case DST_Long:
			{
				ERR_CHK( __Struct_Long( & ec, dsn, rt ))
				break;
			}

			case DST_String:	// Pointers
			case DST_Struct:
			case DST_Pointer:
			{
				ERR_CHK( __Struct_Ptr( & ec, dsn, rt ))
				break;
			}

			default:
			{
				#ifdef DEBUG
				printf( "%s:%04d: Error : Type %d, Cnt %d\n", __FILE__, __LINE__, dsn->dsn_Type, cnt );
				#endif

				goto bailout;
			}
		}

		// Only first entry can have a Label
		rt->rt_Container.Hunk.ms_Ptr_Label = NULL;
		off += dsn->dsn_Size;
	}

	// --

	len	= dsh->dsh_Size;
	fs	= RS4FuncStat_Okay;
	ec	= RS4ErrStat_Okay;

	// --

bailout:

	// --

	*lenptr = len;

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
RS4FileSection *sec;
RS4FileHeader *fh;
RS4Label *parent;
RS4Label *brance;
RS4Label *rel;
CHR buf[32];
S16 *data;
S32 size;
S64 adr;
S16 off;
S32 len;
S32 cnt;
S32 p;

	DDEBUG( { printf( "%s:%04d: RS4Decode_RelativeWord\n", __FILE__, __LINE__ ); } )

	ec	= RS4ErrStat_Error;
	fs	= RS4FuncStat_Error;

	#ifdef DEBUG
	if (( ! rt ) || ( ! rl ))
	{
		ec = RS4ErrStat_Internal;
		printf( "%s:%04d: NULL Pointer\n", __FILE__, __LINE__ );
		printf( "rt ................. : %p\n", rt );
		printf( "rl ................. : %p\n", rl );
		goto bailout;
	}
	#endif

	fh	= rt->rt_File;
	sec = rt->rt_Section;
	rel = rl->rl_Ref;
	len = 0;

	if ( ! rel )
	{
		ec	= RS4ErrStat_Error;
		fs	= RS4FuncStat_Error;

		#ifdef DEBUG
		printf( "%s:%04d: Decode_RelativeWord Error\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	data = (PTR) rl->rl_Memory;
	size = rl->rl_Size/2;

	rt->rt_Container.Hunk.ms_Buf_Comment = buf;

	for( cnt=0 ; cnt < size ; cnt++ )
	{
		// --

		while(( rl ) && ( rl->rl_Offset > pos ))
		{
			rl = RS4GetNext( rl );
		}

		// --

		off = SWAP16( data[cnt] );
		adr = rel->rl_Address;
		adr += off;

		ERR_CHK( RS4FindLabel_File( & ec, fh, & brance, adr, __FILE__ ))

		rt->rt_Container.Hunk.ms_StartAddr = sec->rfs_MemoryAdr + pos;
		rt->rt_Container.Hunk.ms_Str_Opcode = "dc.w";

		// --

		parent = brance->rl_Parent;

		if ( parent )
		{
			S32 off = brance->rl_Offset - parent->rl_Offset;

			if ( off < 0 )
			{
				sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "(%s%d", parent->rl_Name, off );
			}
			else
			{
				sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "(%s+%d", parent->rl_Name, off );
			}
//			sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "(%s+%d", parent->rl_Name, brance->rl_Offset - parent->rl_Offset );
		}
		else
		{
			sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "(%s", brance->rl_Name );
		}

		// --

		p = strlen( rt->rt_Container.Hunk.ms_Buf_Argument );

		parent = rel->rl_Parent;

		if ( parent )
		{
			S32 off = rel->rl_Offset - parent->rl_Offset;

			if ( off < 0 )
			{
				sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "-%s%d)", parent->rl_Name, off );
			}
			else
			{
				sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "-%s+%d)", parent->rl_Name, off );
			}
//			sprintf( & rt->rt_Container.Hunk.ms_Buf_Argument[p], "-%s+%d)", parent->rl_Name, rel->rl_Offset - parent->rl_Offset );
		}
		else
		{
			sprintf( & rt->rt_Container.Hunk.ms_Buf_Argument[p], "-%s)", rel->rl_Name );
		}

		// --

//		sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "(%s-%s)", brance->rl_Name, rel->rl_Name );
		sprintf( rt->rt_Container.Hunk.ms_Buf_Comment, "%02x%02x", sec->rfs_MemoryBuf[ pos + 0 ], sec->rfs_MemoryBuf[ pos + 1 ] );

		if (( rl ) && ( rl->rl_Offset == pos ))
		{
			rt->rt_Container.Hunk.ms_Ptr_Label = rl;
			
			rl = RS4GetNext( rl );
		}
		else
		{
			rt->rt_Container.Hunk.ms_Ptr_Label = NULL;
		}

		ERR_CHK( RS4AddSourceLine( & ec, rt ))

		len += 2;
		pos += 2;
	}

	// --

	*l = len;
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
CHR buf[256];
S64 len;
S32 cnt;
MEM mem;

	DDEBUG( { printf( "%s:%04d: RS4Decode_Code\n", __FILE__, __LINE__ ); } )

	ec	= RS4ErrStat_Error;
	fs	= RS4FuncStat_Error;

	#ifdef DEBUG
	if ( ! rt )
	{
		ec = RS4ErrStat_Internal;
		printf( "%s:%04d: NULL Pointer\n", __FILE__, __LINE__ );
		printf( "rt ................. : %p\n", rt );
		goto bailout;
	}
	#endif

	rt->rt_Container.Hunk.ms_Buf_Comment = buf;

	len = 0;
	sec = rt->rt_Section;
	mem	= sec->rfs_MemoryBuf;

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

		if (( rl ) && ( rl->rl_Offset == pos ))
		{
			rt->rt_Container.Hunk.ms_Ptr_Label = rl;
		}
		else
		{
			rt->rt_Container.Hunk.ms_Ptr_Label = NULL;
		}

		rt->rt_Container.Hunk.ms_StartAddr = sec->rfs_MemoryAdr + pos;
		rt->rt_CurMemAdr = sec->rfs_MemoryAdr + pos;
		rt->rt_CurMemBuf = & mem[ pos ];
		rt->rt_CurMemType = & sec->rfs_MemoryType[ pos ];

		ds = rt->rt_Decoder( & ec, rt );

		if ( ds != RS4DecodeStat_Okay )
		{
			fs = RS4FuncStat_Error;
			// ec allready set

			#ifdef DEBUG
			printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
			#endif

			goto bailout;
		}

		// --

		buf[0] = 0;

		for( cnt=0 ; cnt<rt->rt_CPU.M68k.mt_OpcodeSize ; cnt++ )
		{
			S32 p = strlen( buf );

			if (( cnt & 1 ) || ( cnt == 0 ))
			{
				sprintf( & buf[p], "%02x", mem[pos+cnt] );
			}
			else
			{
				sprintf( & buf[p], " %02x", mem[pos+cnt] );
			}
		}

		// --

		ERR_CHK( RS4AddSourceLine( & ec, rt ))

		// --

		pos += rt->rt_CPU.M68k.mt_OpcodeSize;
		len += rt->rt_CPU.M68k.mt_OpcodeSize;

		// --

		if ( rt->rt_CPU.M68k.mt_ClearRegMask )
		{
			M68k_ClearRegs( rt, rt->rt_CPU.M68k.mt_ClearRegMask );
			rt->rt_CPU.M68k.mt_ClearRegMask = 0;
		}

		// --
	
		if ( rt->rt_CPU.M68k.mt_LastOpcode )
		{
			if ((( rt->rt_CPU.M68k.mt_Opcode & 0xffff0000 ) != ( 0x4ef90000 ))	// Last Opcode == Jmp
			|| ( mem[pos+0] != 0x4e ) || ( mem[pos+1] != 0xf9 ))	// Next Opcode == Jmp
			{
				ERR_CHK( RS4AddEmptyLine( & ec, rt ))
			}
			break;
		}
	}

	*len_ptr = len;

	// --

	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

bailout:

	rt->rt_Container.Hunk.ms_Buf_Comment = NULL;

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
	RS4Ref *rr, 
	S64 max,
	S64 *len_ptr )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
//RS4FileSection *sec;
RS4FileHeader *fh;
RS4Label *newrl;
CHR buf[256];
S64 offset;
S32 doblk;
S32 doptr;
S32 loop;
S64 size;
S64 val;
S64 len;
S32 cnt;
S32 mm;
U8 c;

	DDEBUG( { printf( "%s:%04d: RS4Decode_Data\n", __FILE__, __LINE__ ); } )

	ec	= RS4ErrStat_Error;
	fs	= RS4FuncStat_Error;

	#ifdef DEBUG
	if ( ! rt )
	{
		ec = RS4ErrStat_Internal;
		printf( "%s:%04d: NULL Pointer\n", __FILE__, __LINE__ );
		printf( "rt ................. : %p\n", rt );
		goto bailout;
	}
	#endif

	fh	= rt->rt_File;
//	sec	= rt->rt_Section;

	// --

	rt->rt_Container.Hunk.ms_StartAddr = rt->rt_CurMemAdr;
	rt->rt_Container.Hunk.ms_Buf_Comment = NULL;

	doblk = FALSE;
	doptr = FALSE;

	if (( rr ) && ( rr->rr_Address == rt->rt_CurMemAdr ))
	{
		rr->rr_Handled = TRUE;
		doptr = TRUE;
	}
	else 
	{
		if (( max > 15 ) && (( rl ) && ( rl->rl_Address == rt->rt_CurMemAdr )))
		{
			c = rt->rt_CurMemBuf[ 0 ];

			for( cnt=1 ; cnt<max ; cnt++ )
			{
				if ( rt->rt_CurMemBuf[ cnt ] != c )
				{
					break;
				}
			}

			if ( cnt == max )
			{
				doblk = TRUE;
			}
		}

		if ( doblk == FALSE )
		{
			mm = max;

			if ( mm >= 4 )
			{
				max  = mm / 4;
				max *= 4;
			}
//			else if (( m == 1 ) || ( m == 2 ) || ( m == 3 ))
//			{
//				max = mm;
//			}
		}
	}

	if ( doptr )
	{
		rt->rt_Container.Hunk.ms_Str_Opcode = "dc.l";

		val =  (( (U64) rt->rt_CurMemBuf[ 0 ] << 24 )
			|   ( (U64) rt->rt_CurMemBuf[ 1 ] << 16 )
			|   ( (U64) rt->rt_CurMemBuf[ 2 ] <<  8 )
			|   ( (U64) rt->rt_CurMemBuf[ 3 ] <<  0 ));

		rt->rt_Container.Hunk.ms_Buf_Argument[0] = 0;

		ERR_CHK( RS4FindLabel_File( & ec, fh, & newrl, val, __FILE__ ))
//		newrl = RS4AddLabel_File( rt->rt_Container.Hunk.ms_HunkStruct, val, RS4LabelType_Unset );
//		newrl = RS4AddLabel_Sec( rt->rt_Container.Hunk.ms_HunkStruct, sec->rt_Section, val, RS4LabelType_Unset );

		if ( newrl )
		{
			ERR_CHK( RS4BuildLabelString( & ec, newrl, rt->rt_Container.Hunk.ms_Buf_Argument ))
		}

		rt->rt_Container.Hunk.ms_Buf_Comment = buf;

		sprintf( buf, "%02x%02x %02x%02x",
			rt->rt_CurMemBuf[ 0 ],
			rt->rt_CurMemBuf[ 1 ],
			rt->rt_CurMemBuf[ 2 ],
			rt->rt_CurMemBuf[ 3 ]
		);

		size = 4;
	}
	else if ( doblk )
	{
		rt->rt_Container.Hunk.ms_Str_Opcode = "blk.b";
		size = max;

		if ( rt->rt_CurMemBuf[ 0 ] == 0 )
		{
			sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "%" PRId64 ",0", max );
		}
		else
		{
			sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "%" PRId64 ",$%02x", max, rt->rt_CurMemBuf[ 0 ] );
		}
	}
	else if ( max > 3 )
	{
		// 1 Long or more

		rt->rt_Container.Hunk.ms_Str_Opcode = "dc.l";

		loop = ( max >= 16 ) ? 4 : max / 4;

		offset = 0;

		rt->rt_Container.Hunk.ms_Buf_Argument[0] = 0;

		for( cnt=0 ; cnt<loop ; cnt++ )
		{
			val =  (( (U64) rt->rt_CurMemBuf[ offset+0 ] << 24 )
				|   ( (U64) rt->rt_CurMemBuf[ offset+1 ] << 16 )
				|   ( (U64) rt->rt_CurMemBuf[ offset+2 ] <<  8 )
				|   ( (U64) rt->rt_CurMemBuf[ offset+3 ] <<  0 ));

			len = strlen( rt->rt_Container.Hunk.ms_Buf_Argument );

			snprintf( & rt->rt_Container.Hunk.ms_Buf_Argument[len], 64-len, "$%08" PRIx64 ",", val );

			offset += 4;
		}

		size = offset;

		len = strlen( rt->rt_Container.Hunk.ms_Buf_Argument );

		rt->rt_Container.Hunk.ms_Buf_Argument[len-1] = 0;
	}
	else if ( max > 1 )
	{
		// 1 Word

		size = 2;

		val = (( rt->rt_CurMemBuf[ 0 ] << 8 ) | ( rt->rt_CurMemBuf[ 1 ] << 0 ));

		rt->rt_Container.Hunk.ms_Str_Opcode = "dc.w";
		snprintf( rt->rt_Container.Hunk.ms_Buf_Argument, 64, "$%04" PRIx64 "", val );
	}
	else
	{
		// 1 Byte

		size = 1;

		val = ( rt->rt_CurMemBuf[ 0 ] );

		rt->rt_Container.Hunk.ms_Str_Opcode = "dc.b";
		snprintf( rt->rt_Container.Hunk.ms_Buf_Argument, 64, "$%02" PRIx64 "", val );
	}

	ERR_CHK( RS4AddSourceLine( & ec, rt ))

	*len_ptr = size;

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

enum STRINGMODE
{
	SM_ASCII = 11,
	SM_Decimal = 19,
};

#define MAX_LINELEN		38

static enum RS4FuncStat RS4Decode_String( enum RS4ErrorCode *errcode, RS4Trace *rt, RS4Label *rl, S64 max, S64 *len_ptr )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
enum STRINGMODE mode;
RS4FileSection *sec;
CHR comment[128];
S32 linelen;
S32 bytes;
S32 size;
S64 adr;
STR arg;
S32 chr;
S32 len;
S32 pos;
S32 cnt;
S32 p;
MEM mem;

	DDEBUG( { printf( "%s:%04d: RS4Decode_String\n", __FILE__, __LINE__ ); } )

	ec	= RS4ErrStat_Error;
	fs	= RS4FuncStat_Error;

	// --

	#ifdef DEBUG
	if (( ! rt ) || ( ! rl ))
	{
		ec = RS4ErrStat_Internal;
		printf( "%s:%04d: NULL Pointer\n", __FILE__, __LINE__ );
		printf( "rt ................. : %p\n", rt );
		printf( "rl ................. : %p\n", rl );
		goto bailout;
	}
	#endif

	sec = rt->rt_Section;
	arg = rt->rt_Container.Hunk.ms_Buf_Argument;
	mem = sec->rfs_MemoryBuf;
	adr = rl->rl_Address;
	pos = rl->rl_Offset;
	size= rl->rl_Size;
	len = 0;

	rt->rt_Container.Hunk.ms_Buf_Comment = comment;
	rt->rt_Container.Hunk.ms_Str_Opcode = "dc.b";
	rt->rt_Container.Hunk.ms_Ptr_Label = rl;

	if ( size <= 0 )
	{
		ec = RS4ErrStat_Internal;

		#ifdef DEBUG
		printf( "%s:%04d: Zero Length\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	do
	{
		// Main Loop, build one line of max 20 chars

		rt->rt_Container.Hunk.ms_StartAddr = adr;

		mode = SM_Decimal;
		linelen = 0;
		bytes = 0;
		arg[0] = 0;
		p = pos;

		while( size > 0 )
		{
			chr = mem[pos];

			if (( chr >= 0x20 ) && ( chr < 0x7f ) && ( chr != '"' ))
			{
				// Chr is an ASCII type

				if ( mode == SM_ASCII )
				{
					// Make sure there are space for: Char

					if ( linelen > MAX_LINELEN - 1 )
					{
						break;
					}

					// We are in ascii mode, just add
					sprintf( & arg[linelen], "%c", chr );
				}
				else
				{
					// We are not in ascii mode, so lets switch

					if ( linelen == 0 )
					{
						sprintf( & arg[linelen], "\"%c", chr );
					}
					else
					{
						// Make sure there are space for: ," + %c

						if ( linelen > MAX_LINELEN - 3 )
						{
							break;
						}

						sprintf( & arg[linelen], ",\"%c", chr );
					}

					mode = SM_ASCII;
				}
			}
			else
			{
				if ( mode == SM_Decimal )
				{
					// Make sure there are space for: , %d

					if ( linelen == 0 )
					{
						// We are not in ascii mode, just add
						sprintf( & arg[linelen], "%d", chr );
					}
					else
					{
						if ( linelen > MAX_LINELEN - 4 ) // %d may be 3 bytes
						{
							break;
						}

						// We are not in ascii mode, just add
						sprintf( & arg[linelen], ",%d", chr );
					}
				}
				else
				{
					// We are in ascii mode, so lets switch

					// Make sure there are space for: ", %d

					if ( linelen > MAX_LINELEN - 5 ) // %d may be 3 bytes
					{
						break;
					}

					sprintf( & arg[linelen], "\",%d", chr );

					mode = SM_Decimal;
				}
			}

			pos++;
			size--;
			bytes++;
			linelen = strlen( arg );

			if ( chr == 10 )
			{
				break;
			}
		}

		if ( mode == SM_ASCII )
		{
			arg[linelen++] = '"';
		}

		arg[linelen] = 0;

		// --

		comment[0] = 0;

		for( cnt=0 ; cnt<bytes ; cnt++ )
		{
			linelen = strlen( comment );

			if (( cnt == 0 ) || ( cnt & 1 ))
			{
				sprintf( & comment[linelen], "%02x", mem[ p + cnt ] );
			}
			else
			{
				sprintf( & comment[linelen], " %02x", mem[ p + cnt ] );
			}
		}

		// --

		ERR_CHK( RS4AddSourceLine( & ec, rt ))

		rt->rt_Container.Hunk.ms_Ptr_Label = NULL;
		adr += bytes;
		len	+= bytes;
	}
	while( size > 0 );

	if ( rl->rl_Size != len )
	{
		ec = RS4ErrStat_Internal;

		#ifdef DEBUG
		printf( "%s:%04d: Error invalid length ( %" PRId64 " != %d )\n", __FILE__, __LINE__, rl->rl_Size, len );
		#endif

		goto bailout;
	}

	if ( len > max )
	{
		ec = RS4ErrStat_Internal;

		#ifdef DEBUG
		printf( "%s:%04d: Error invalid length ( %" PRId64 " != %d )\n", __FILE__, __LINE__, max, len );
		#endif

		goto bailout;
	}

	rt->rt_Container.Hunk.ms_Buf_Comment = NULL;

	*len_ptr = len;

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

#undef MAX_LINELEN

// --

static enum RS4FuncStat RS4Decode_BSS( enum RS4ErrorCode *errcode, RS4Trace *rt, S64 max, S64 *len_ptr )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
S64 size;

	DDEBUG( { printf( "%s:%04d: RS4Decode_BSS\n", __FILE__, __LINE__ ); } )

	ec = RS4ErrStat_Error;
	fs = RS4FuncStat_Error;

	// --

	#ifdef DEBUG
	if ( ! rt )
	{
		ec = RS4ErrStat_Internal;
		printf( "%s:%04d: NULL Pointer\n", __FILE__, __LINE__ );
		printf( "rt ................. : %p\n", rt );
		goto bailout;
	}
	#endif

	size = 1 * max;

	rt->rt_Container.Hunk.ms_StartAddr = rt->rt_CurMemAdr;
	rt->rt_Container.Hunk.ms_Str_Opcode = "ds.b";
	snprintf( rt->rt_Container.Hunk.ms_Buf_Argument, 64, "%" PRId64 "", size );

	ERR_CHK( RS4AddSourceLine( & ec, rt ))

	*len_ptr = size;

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
// Section Init : Header

static enum RS4FuncStat Handle_Init( enum RS4ErrorCode *errcode, RS4Trace *rt UNUSED )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4FileSection *sec;
RS4Label *rl;
STR sectype;
STR memtype;

	ec	= RS4ErrStat_Error;
	fs	= RS4FuncStat_Error;
	sec	= rt->rt_Section;

	// --

	/**/ if ( sec->rfs_SecType == RS4ST_Code )
	{
		sectype = "CODE";
	}
	else if ( sec->rfs_SecType == RS4ST_Data )
	{
		sectype = "DATA";
	}
	else if ( sec->rfs_SecType == RS4ST_BSS )
	{
		sectype = "BSS";
	}
	else
	{
		sectype = "???";
	}

	/**/ if ( sec->rfs_HunkData.hfs_MemoryType == HUNK_MEMTYPE_Chip )
	{
		memtype = "_C";
	}
	else if ( sec->rfs_HunkData.hfs_MemoryType == HUNK_MEMTYPE_Fast )
	{
		memtype = "_F";
	}
	else // 0 = Public, 3 = ??
	{
		memtype = "";
	}

	ERR_CHK( RS4AddEmptyLine( & ec, rt ))

	if ( Sec_Split )
	{
		STR name;
		S32 stat;

		stat = asprintf( & name, "%s.i", OutputFile );

		if (( stat < 0 ) || ( ! name ))
		{
			ec = RS4ErrStat_OutOfMemory;

			#ifdef DEBUG
			printf( "%s:%04d: Error allocating memory\n", __FILE__, __LINE__ );
			#endif

			goto bailout;
		}

		rt->rt_Container.Hunk.ms_StartAddr = 0;
		rt->rt_Container.Hunk.ms_Str_Opcode = "INCLUDE";
		sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "\"%s\"", name );

		fs = RS4AddSourceLine( & ec, rt );

		free( name );
		name = NULL;

		if ( fs != RS4FuncStat_Okay )
		{
			// ec allready set

			#ifdef DEBUG
			printf( "%s:%04d: Error adding source node\n", __FILE__, __LINE__ );
			#endif

			goto bailout;
		}

		ERR_CHK( RS4AddEmptyLine( & ec, rt ))
	}

	rt->rt_Container.Hunk.ms_StartAddr = 0;
	rt->rt_Container.Hunk.ms_Str_Opcode = "SECTION";
	sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "SEC%d,%s%s", sec->rfs_SecNr, sectype, memtype );

	ERR_CHK( RS4AddSourceLine( & ec, rt ))

	ERR_CHK( RS4AddEmptyLine( & ec, rt ))

	fs	= RS4FuncStat_Okay;
	ec	= RS4ErrStat_Okay;

	// --

	if ( Sec_xDef )
	{
		rl = RS4GetHead( & sec->rfs_SecLabels );

		if ( rl )
		{
			S32 add_nl = FALSE;

			while( rl )
			{
				if (( rl->rl_xDef ) && ( ! rl->rl_Parent ))
				{
					add_nl = TRUE;

					rt->rt_Container.Hunk.ms_StartAddr = 0;
					rt->rt_Container.Hunk.ms_Str_Opcode = "XDEF";
					sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "%s", rl->rl_Name );

					ERR_CHK( RS4AddSourceLine( & ec, rt ))
				}

				rl = RS4GetNext( rl );
			}

			// --

			if ( add_nl )
			{
				ERR_CHK( RS4AddEmptyLine( & ec, rt ))
			}
		}
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
// Section Exit : Bottom

static enum RS4FuncStat Handle_Exit( 	
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

	// --
	// One label can be just after Memory BLock

	rl	= *rlptr;

	while(( rl ) && ( rl->rl_Offset < pos ))
	{
		rl = RS4GetNext( rl );
	}

	if (( rl ) && ( rl->rl_Offset == pos ))
	{
		rt->rt_Container.Hunk.ms_StartAddr	= 0;
 		rt->rt_Container.Hunk.ms_Str_Opcode	= "";
		rt->rt_Container.Hunk.ms_Ptr_Label	= rl;
		rt->rt_Container.Hunk.ms_Buf_Argument[0] = 0;

		ERR_CHK( RS4AddSourceLine( & ec, rt ))
	}
	else
	{
		fs	= RS4FuncStat_Okay;
		ec	= RS4ErrStat_Okay;
	}

	// --

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fs );
}

// --
// len == 0 is Error

static S64 Handle_Unset( 
		enum RS4ErrorCode *errcode, 
		RS4Trace *rt,		// Trace info
		RS4Label **rlptr,	// Current Label
		RS4Ref **rrptr,		// Current Ref
UNUSED	MEM type,		// MemType
UNUSED	S64 size,		// MemSize
		S64 max,		// max bytes ( too .. next label, next ref, sec end, type change )
		S64 pos )		// mem/type pos
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4FileSection *sec;
RS4Label *rl;
RS4Ref *rr;
S64 len;
S64 l;

	l	= 0;
	ec	= RS4ErrStat_Error;
	sec	= rt->rt_Section;
	rl	= *rlptr;
	rr	= *rrptr;

	// --

	if (( sec->rfs_SecType == RS4ST_BSS )
	||	( sec->rfs_DataSize <= pos ))
	{
		ERR_CHK( RS4Decode_BSS( & ec, rt, max, & len ))
	}
	else
	{
		ERR_CHK( RS4Decode_Data( & ec, rt, rl, rr, max, & len ))
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
		RS4Trace *rt,		// Trace info
		RS4Label **rlptr,	// Current Label
UNUSED	RS4Ref **rrptr,		// Current Ref
		MEM type,		// MemType
		S64 size,		// MemSize
UNUSED	S64 max,		// max bytes ( too .. next label, next ref, sec end, type change )
		S64 pos )		// mem/type pos
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Label *rl;
S64 len;
S64 l;

	l	= 0;
	ec	= RS4ErrStat_Error;
	rl	= *rlptr;

	// --

	ERR_CHK( RS4Decode_Code( & ec, rt, rl, size, pos, type, & len ) )

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
//	MEM type,		// MemType
//	S64 size,		// MemSize
	S64 max,		// max bytes ( too .. next label, next ref, sec end, type change )
	S64 pos )		// mem/type pos
{
enum RS4LabelType t;
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Label *rl;
RS4Ref *rr;
S64 len;
S64 l;

	l	= 0;
	ec	= RS4ErrStat_Error;
	rl	= *rlptr;
	rr	= *rrptr;
	len	= 0;

	t	= (( rl ) && ( rl->rl_Offset == pos )) ? rl->rl_Type1 : RS4LabelType_Unknown ;

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
			ERR_CHK( RS4Decode_Struct( & ec, rt, rl, pos, & len ))
			break;
		}

//		case RS4LabelType_Unset:
		case RS4LabelType_Unknown:
		{
			ERR_CHK( RS4Decode_Data( & ec, rt, rl, rr, max, & len ))
			break;
		}

		default:
		{
			ec = RS4ErrStat_Internal;
			fs = RS4FuncStat_Error;

			#ifdef DEBUG
			printf( "%s:%04d: Error : Unknow Label Type %d\n", __FILE__, __LINE__, t );
			goto bailout;
			#endif

			break;
		}
	}

	if ( fs != RS4FuncStat_Okay )
	{
		// ec allready set

		#ifdef DEBUG
		printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
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
		// Insert Empty line, to make source code look better
		//

		while( TRUE )
		{
			if ( ! pos )
			{
				break;
			}

			S32 type1 = type[pos];
			S32 type2 = type[pos-1];

			if ( type1 == type2 )
			{
				break;
			}

			if ( type1 != RS4MT_Code )
			{
				break;
			}

			ERR_CHK( RS4AddEmptyLine( & ec, rt ))
			break;
		}

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
//				len = Handle_Data( & ec, rt, & rl, & rr, type, size, max, pos );
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

enum RS4FuncStat RS4BuildSource_File( enum RS4ErrorCode *errcode, RS4FileHeader *fh )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Trace rt;
CHR argbuf[256];
S32 cnt;

	DDEBUG( { printf( "%s:%04d: RS4BuildSource_File\n", __FILE__, __LINE__ ); } )

	ec = RS4ErrStat_Error;
	fs = RS4FuncStat_Error;

	#ifdef DEBUG
	if ( ! fh )
	{
		ec = RS4ErrStat_Internal;
		printf( "%s:%04d: NULL Pointer\n", __FILE__, __LINE__ );
		printf( "fh ................. : %p\n", fh );
		goto bailout;
	}
	#endif

	// --

	ERR_CHK( RS4InitTrace( & ec, & rt, fh, RS4TracePass_Build ))

	rt.rt_Container.Hunk.ms_Buf_Argument = & argbuf[0];

	// --
	#if 0

	if (( fh->rfh_SecFirst >= 0 ) && ( fh->rfh_SecArraySize > 0 ))
	{
		rt.rt_Section = fh->rfh_SecArray[ fh->rfh_SecFirst ].rsi_Section;

		fs = RS4Parser( & ec, & rt );

		if ( fs != RS4FuncStat_Okay )
		{
			// ec allready set

			#ifdef DEBUG
			printf( "%s:%04d: Error generating source\n", __FILE__, __LINE__ );
			#endif

			goto bailout;
		}
	}

	#endif
	// --

	for( cnt=0 ; cnt < fh->rfh_SecArraySize ; cnt++ )
	{
//		if ( cnt == fh->rfh_SecFirst )
//		{
//			continue;
//		}

		rt.rt_Section = fh->rfh_SecArray[ cnt ].rsi_Section;

		ERR_CHK( RS4Parser( & ec, & rt ))
	}

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
