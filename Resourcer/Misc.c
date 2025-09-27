
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

STR RS4Strdup( STR string ) 
{
STR str;
S32 len;

	str = NULL;

	if ( string == NULL ) 
	{
		goto bailout;
	}

	len = strlen( string );

	str = malloc( len + 1 );

	if ( str == NULL ) 
	{
		goto bailout;
	}

	strcpy( str, string );

bailout:

	return( str );
}

// -- Init Trace

enum RS4FuncStat RS4InitTrace( enum RS4ErrorCode *errcode, RS4Trace *rt, RS4FileHeader *fh, enum RS4TracePass type )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;

	// --

	ec = RS4ErrStat_Error;
	fs = RS4FuncStat_Error;

	// --

	if (( ! rt )
	||	( ! fh ))
	{
		ec = RS4ErrStat_Internal;

		#ifdef DEBUG
		printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	// --

	memset( rt, 0, sizeof( RS4Trace ));

	rt->rt_ID		= RS4ID_Trace;
	rt->rt_File		= fh;
	rt->rt_Pass		= type;

	#ifdef DEBUG
	#ifdef SUPPORT_M68K

	for( S32 cnt=0 ; cnt<8 ; cnt++ )
	{
		rt->rt_CPU.M68k.mt_Registers[cnt+0].mr_Number = 0xd0 + cnt;
		rt->rt_CPU.M68k.mt_Registers[cnt+8].mr_Number = 0xa0 + cnt;
	}

	#endif
	#endif



	// --
	// -- Figure out what Decoder to use

	switch( fh->rfh_FileType )
	{
		#ifdef SUPPORT_HUNK
		case RS4FileType_Hunk:
		{
			rt->rt_Decoder = M68k_Decoder;
			break;
		}
		#endif

		default:
		{
			ec = RS4ErrStat_UnsupportedFileType;

			#ifdef DEBUG
			printf( "%s:%04d: Error unsupported File Type (%d)\n", __FILE__, __LINE__, fh->rfh_FileType );
			#endif

			goto bailout;
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

STR FindFileName( STR name )
{
STR str;
S32 pos;

	pos = 0;
	str = name;

	while( name[pos] )
	{
		if ( name[pos+1] == 0 )
		{
			break;
		}

		if (( name[pos] == '/' )
		||	( name[pos] == ':' )
		||	( name[pos] == '\\' ))
		{
			str = & name[pos+1];
		}

		pos++;
	}

	return( str );
}

// --

void Mark_NulString( RS4Label *rl )
{
RS4FileSection *sec;
MEM type;
MEM mem;
S64 off;
S32 chr;
S32 len;

	if ( rl->rl_UserLocked )
	{
		// No Error
		return;
	}

	// Check we have not been set before
	if ( rl->rl_Type1 != RS4LabelType_Unset )
	{
		if (( rl->rl_Type1 != RS4LabelType_String )	
		&&	( rl->rl_Type1 != RS4LabelType_Unknown ))
		{
			// Change rl type, and continue?
			printf( "String Allready set\n" );
			printf( "%s:%04d: Error Adr: $%08" PRIx64 " (%d)\n", __FILE__, __LINE__, rl->rl_Address, rl->rl_Type1 );
			return;
		}
		else
		{
			// Same Type, no error
			return;
		}
	}

	// todo?? validate that Label is a ASCII string

	// -- Check Section Type
	sec = rl->rl_Section;

	if ( sec->rfs_SecType == RS4ST_BSS )
	{
		return;
	}

	// --
	off	= rl->rl_Offset;
	mem = sec->rfs_MemoryBuf;
	type= sec->rfs_MemoryType;

// printf( "Offset: %d\n", off );

	while( TRUE )
	{
		chr = mem[off];

		type[off++] = RS4MT_Data;

		if ( chr == 0 )
		{
			break;
		}
	}

	len = off - rl->rl_Offset;

//printf( "String Length: %d\n", len );

//	rl->rl_UserLocked = TRUE;
	rl->rl_Type1 = RS4LabelType_String;
	rl->rl_Size = len;
}

// --

enum RS4FuncStat Mark_Struct( enum RS4ErrorCode *errcode, RS4Label *rl, enum RS4StructID id )
{
struct DataStructHeader *dsh;
struct DataStructNode *dsn;
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4FileSection *sec;
RS4FileHeader *fh;
RS4Label *rl2;
MEM type;
MEM mem;
S64 off;
//S64 adr;
S64 val;
//S64 cnt;
S32 size;
S32 cnt;

	ec = RS4ErrStat_Error;
	fs = RS4FuncStat_Error;

	if ( rl->rl_UserLocked )
	{
		// No Error
		ec = RS4ErrStat_Okay;
		fs = RS4FuncStat_Okay;
//		printf( "Mark_Struct : Locked : %p\n", rl );
		goto bailout;
	}

	#if 0
	// we can define ds.b structs
	if ( sec->rfs_SecType == RS4ST_BSS )
	{
		#ifdef DEBUG
		printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
		#endif
		goto bailout;
	}
	#endif

	// Validate ID
	if (( id <= RS4StructID_Unknown ) || ( id >= RS4StructID_Last ))
	{
		#ifdef DEBUG
		printf( "%s:%04d: Error : ID %d\n", __FILE__, __LINE__, id );
		#endif
		goto bailout;
	}

	// Check we have not been set before
	if ( rl->rl_Type1 != RS4LabelType_Unset )
	{
		if (( rl->rl_Type1 != RS4LabelType_Struct )		
		||	( rl->rl_Type2 != (S32) id ))
		{
			ec = RS4ErrStat_Error;
			fs = RS4FuncStat_Error;
			// Change rl type, and continue?
			printf( "Struct Allready set\n" );
			printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
			goto bailout;
		}
		else
		{
			// Same Type, no error
			ec = RS4ErrStat_Okay;
			fs = RS4FuncStat_Okay;
			goto bailout;
		}
	}

	// --
	sec 	= rl->rl_Section;
	fh		= sec->rfs_File;
	dsh		= DataStructTable[id];
	off		= rl->rl_Offset;
//	adr		= sec->rfs_MemoryAdr;
	mem		= sec->rfs_MemoryBuf;
	type	= sec->rfs_MemoryType;
	size	= dsh->dsh_Size;

	#ifdef DEBUG
	if ( size <= 0 )
	{
		printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
		goto bailout;
	}
	#endif

	// --

	if ( sec->rfs_SecType != RS4ST_BSS )
	{
		for( cnt=0 ; cnt < dsh->dsh_Entries ; cnt++ )
		{
			dsn = & dsh->dsh_Data[cnt];

			switch( dsn->dsn_Type )
			{
				case DST_Pointer:
				{
					val	= ( (U64) mem[ off + 0 ] << 24 | 
							(U64) mem[ off + 1 ] << 16 | 
							(U64) mem[ off + 2 ] <<  8 | 
							(U64) mem[ off + 3 ] <<  0 );

					if ( val )
					{
						rl2 = RS4AddLabel_File( NULL, fh, val, RS4LabelType_Unset );

						if ( ! rl2 )
						{
							printf( "Error adding Label at $%08" PRIx64 "\n", rl->rl_Address );
						}
					}
					break;
				}

				case DST_String:
				{
					val	= ( (U64) mem[ off + 0 ] << 24 | 
							(U64) mem[ off + 1 ] << 16 | 
							(U64) mem[ off + 2 ] <<  8 | 
							(U64) mem[ off + 3 ] <<  0 );

					if ( val )
					{
						rl2 = RS4AddLabel_File( NULL, fh, val, RS4LabelType_Unset );

						if ( ! rl2 )
						{
							printf( "Error adding Label at $%08" PRIx64 "\n", rl->rl_Address );
						}
						else
						{
							Mark_NulString( rl2 );
						}
					}
					break;
				}

				case DST_Struct:
				{
					val	= ( (U64) mem[ off + 0 ] << 24 | 
							(U64) mem[ off + 1 ] << 16 | 
							(U64) mem[ off + 2 ] <<  8 | 
							(U64) mem[ off + 3 ] <<  0 );

					if ( val )
					{
						rl2 = RS4AddLabel_File( NULL, fh, val, RS4LabelType_Unset );

						if ( ! rl2 )
						{
							printf( "Error adding Label at $%08" PRIx64 "\n", rl->rl_Address );
						}
						else
						{
							fs = Mark_Struct( & ec, rl2, dsn->dsn_ID );

							if ( fs != RS4FuncStat_Okay )
							{
								#ifdef DEBUG
								printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
								#endif
								goto bailout;
							}
						}
					}
					break;
				}

				default:
				{
					break;
				}
			}

			off += dsn->dsn_Size;
		}
	}

	// --

	off		= rl->rl_Offset;
	memset( & type[off], RS4MT_Data, size );

//	rl->rl_UserLocked = TRUE;
	rl->rl_Type1 = RS4LabelType_Struct;
	rl->rl_Type2 = id;
	rl->rl_Size = size;

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

#ifdef SUPPORT_M68K
void M68k_ClearRegs( RS4Trace *rt, S32 mask )
{
S32 cnt;
S32 reg;

	reg = 1;

	#if 0

	printf( "Clear Regs [$%08" PRIx64 "] : ", rt->rt_CurMemAdr );

	for( cnt=0 ; cnt<16 ; cnt++ )
	{
		if ( mask & reg )
		{
			if ( cnt < 8 )	printf( "D%d ", cnt );
			else			printf( "A%d ", cnt-8 );

			rt->rt_CPU.M68k.mt_Registers[cnt].mr_Type1 = RRT_Unknown;
		}

		reg = reg << 1;
	}

	printf( "\n" );

	#else

	for( cnt=0 ; cnt<16 ; cnt++ )
	{
		if ( mask & reg )
		{
			rt->rt_CPU.M68k.mt_Registers[cnt].mr_Type1 = RRT_Unknown;
		}

		reg = reg << 1;
	}

	#endif
}
#endif

// --
