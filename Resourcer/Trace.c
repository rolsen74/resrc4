
/*
 * Copyright (c) 2014-2024 Rene W. Olsen < renewolsen @ gmail . com >
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

#define DDEBUG(x)

// --

static enum RS4FuncStat RS4Text_ScanSection( enum RS4ErrorCode *errcode, RS4FileSection *sec )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Label *next;
RS4Label *rl;
uint8_t *type;
uint8_t *buf;
int size;
int max;
int len;
int cnt;
int c;
int i;

	DDEBUG( { printf( "%s:%04d: Text_ScanHunk\n", __FILE__, __LINE__ ); } )

	// --

	ec		= RS4ErrStat_Error;
	fs		= RS4FuncStat_Error;
	buf		= sec->rfs_MemoryBuf;
	size	= sec->rfs_MemorySize;
	type	= sec->rfs_MemoryType;

	rl = RS4GetHead( & sec->rfs_SecLabels );

	while( rl )
	{
		if (( rl->rl_Offset >= 0 ) 
		&&	( rl->rl_Offset < size )
		&&	( rl->rl_Type1 == RS4LabelType_Unset )
		&&	( buf[  rl->rl_Offset ] != 0 )
		&&	( type[ rl->rl_Offset ] == RS4MT_Unset ))
		{
			len = rl->rl_Offset;

			max = size - len;

			next = RS4GetNext( rl );

			if ( next )
			{
				max = RS4MIN( max, next->rl_Offset - rl->rl_Offset );
			}

			for( cnt=0 ; cnt<max ; cnt++ )
			{
				c = buf[len];

				if ((( c < 127 ) && ( 31 < c )) 
				||	( c == 9 )
				||	( c == 10 ))
//				||	( c == 13 ))
				{
					len++;
				} 
				else
				{
					break;
				}
			}

			for( i=0 ; i<4 ; i++ )
			{
				// NUL Term and max 3 bytes to align
				if (( buf[len] == 0 ) && ( cnt < max ))
				{
					cnt++;
					len++;
				}
				else
				{
					break;
				}
			}

			len -= rl->rl_Offset;

			if (( len > 3 ) && ( len == max ))
			{
				rl->rl_Type1 = RS4LabelType_String;
				rl->rl_Size = len;

				memset( & type[ rl->rl_Offset ], RS4MT_Data, len );
			}
		}

		rl = RS4GetNext( rl );
	}

	// --

	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

	// --

//bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fs );
}

// --

static enum RS4FuncStat RS4Text_Scan( enum RS4ErrorCode *errcode, RS4FileHeader *fh )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4FileSection *sec;
int cnt;

	DDEBUG( { printf( "%s:%04d: Text_Scan\n", __FILE__, __LINE__ ); } )

	ec = RS4ErrStat_Error;
	fs = RS4FuncStat_Error;

	for( cnt=0 ; cnt<fh->rfh_SecArraySize ; cnt++ )
	{
		sec = fh->rfh_SecArray[cnt].rsi_Section;

		if ( sec->rfs_SecType != RS4ST_BSS )
		{
			fs = RS4Text_ScanSection( & ec, sec );

			if ( fs != RS4FuncStat_Okay )
			{
				// ec allready set

				#ifdef DEBUG
				printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
				#endif

				goto bailout;
			}
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

static enum RS4FuncStat RS4ScanSecJumps( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4FileSection *sec;
uint8_t *type;
uint8_t *mem;
int64_t size;
int64_t pos;
int64_t adr;

	DDEBUG( { printf( "%s:%04d: RS4ScanSecJumps\n", __FILE__, __LINE__ ); } )

	ec		= RS4ErrStat_Error;
	fs		= RS4FuncStat_Error;
	sec		= rt->rt_Section;
	adr		= sec->rfs_MemoryAdr;
	mem		= sec->rfs_MemoryBuf;
	type	= sec->rfs_MemoryType;
	size	= sec->rfs_MemorySize;
	pos		= 0;

	if ( size < 2 )
	{
		// not an error
		goto bailout;
	}

	size -= 2; // Avoid buffer overrun

	rt->rt_Section = sec;

	while( pos < size )
	{
		//	Jmp	L02BB(PC,D0.w)	; $0002047a 4efb 0004
		if (( mem[pos] == 0x4e ) && ( mem[pos+1] == 0xfb ))
		{
			rt->rt_CurMemAdr	= adr + pos;
			rt->rt_CurMemBuf	= & mem[pos];
			rt->rt_CurMemType	= & type[ pos ];

			fs = RS4Trace_JumpTable( & ec, rt );

			if ( fs != RS4FuncStat_Okay )
			{
				// ec allready set

				#ifdef DEBUG
				printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
				#endif

				goto bailout;
			}
		}

		pos++;
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

static enum RS4FuncStat RS4ScanForJumps( enum RS4ErrorCode *errcode, RS4Trace *rt, RS4FileHeader *fh )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4FileSection *sec;
int cnt;

	DDEBUG( { printf( "%s:%04d: RS4ScanForJumps\n", __FILE__, __LINE__ ); } )

	ec = RS4ErrStat_Error;
	fs = RS4FuncStat_Error;

	for( cnt=0 ; cnt<fh->rfh_SecArraySize ; cnt++ )
	{
		sec = fh->rfh_SecArray[cnt].rsi_Section;

		if ( sec->rfs_SecType == RS4ST_Code )
		{
			rt->rt_Section = sec;

			fs = RS4ScanSecJumps( & ec, rt );

			if ( fs != RS4FuncStat_Okay )
			{
				// ec allready set

				#ifdef DEBUG
				printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
				#endif

				goto bailout;
			}
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

static enum RS4FuncStat RC4TraceBrance( enum RS4ErrorCode *errcode, RS4Trace *rt, RS4Brance *rb )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4FileSection *sec;
RS4Label *rl;
uint8_t *type;
int64_t jmpadr;
int64_t size;
int64_t pos;
int64_t adr;

	DDEBUG( { printf( "%s:%04d: RC4TraceBrance\n", __FILE__, __LINE__ ); } )

	ec = RS4ErrStat_Error;
	fs = RS4FuncStat_Error;

	#ifdef DEBUG

	if (( ! rt )
	||	( ! rt->rt_File )
	||	( ! rt->rt_Section )
	||	( ! rb ))
	{
		ec = RS4ErrStat_Internal;

		#ifdef DEBUG
		printf( "%s:%04d: Error NULL Pointer\n", __FILE__, __LINE__ );
		printf( "rt ................. : %p\n", rt );
		printf( "rt->rt_File ........ : %p\n", rt->rt_File );
		printf( "rt->rt_Section ..... : %p\n", rt->rt_Section );
		printf( "rb ................. : %p\n", rb );
		#endif
	
		goto bailout;
	}

	#endif

	sec		= rb->rb_Section;
	pos		= rb->rb_Offset;
	size	= sec->rfs_MemorySize;
	type	= sec->rfs_MemoryType;
	adr		= sec->rfs_MemoryAdr;

	rt->rt_Section = sec;

	// --

	memcpy( & rt->rt_CPU.M68k.mt_Registers, & rb->rb_Registers, 16 * sizeof( struct M68kRegister ));

	if ( type[ pos ] == RS4MT_Unset )
	{
		rl = RS4FindLabel_File( rt->rt_File, adr + pos );

		if ( rl )
		{
			memcpy( & rl->rl_Registers, & rb->rb_Registers, 16 * sizeof( struct M68kRegister ));
		}
	}

	// --

	while( TRUE )
	{
		if ( pos >= size )
		{
			break;
		}

		if ( type[ pos ] != RS4MT_Unset )
		{
			break;
		}

		rt->rt_CurMemAdr	=   sec->rfs_MemoryAdr + pos  ;
		rt->rt_CurMemBuf	= & sec->rfs_MemoryBuf [ pos ];
		rt->rt_CurMemType	= & sec->rfs_MemoryType[ pos ];

		ds = rt->rt_Decoder( & ec, rt );

		/**/ if ( ds == RS4DecodeStat_Error )
		{
			// ec allready set

			#ifdef DEBUG
			uint16_t vv1 = (( rt->rt_CurMemBuf[0] << 8 ) | ( rt->rt_CurMemBuf[1] << 0 ));
			uint16_t vv2 = (( rt->rt_CurMemBuf[2] << 8 ) | ( rt->rt_CurMemBuf[3] << 0 ));
			uint16_t vv3 = (( rt->rt_CurMemBuf[4] << 8 ) | ( rt->rt_CurMemBuf[5] << 0 ));

			printf( "\n" );
			printf( "Opcode ....... : $%04x:%04x:%04x\n", vv1, vv2, vv3 );
			printf( "Address ...... : $%08lx\n", rt->rt_CurMemAdr );
			printf( "Sec Nr ....... : %d\n",	sec->rfs_SecNr );
			printf( "Sec Type ..... : $%04x\n", sec->rfs_SecType );
			printf( "Sec Offset ... : $%08lx (%ld)\n", pos, pos );
			printf( "File Offset .. : $%08lx (%ld)\n", sec->rfs_FileOffset+pos, sec->rfs_FileOffset+pos );
			printf( "\n" );
			printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
			#endif

			goto bailout;
		}
		else if ( ds == RS4DecodeStat_UnknownCmd )
		{
			printf( "\n" );
			printf( "Unknown Opcode : $%08x\n",	rt->rt_CPU.M68k.mt_Opcode );
			printf( "Address ...... : $%08lx\n", rt->rt_CurMemAdr );
			printf( "Sec Nr ....... : %d\n",	sec->rfs_SecNr );
			printf( "Sec Type ..... : $%04x\n", sec->rfs_SecType );
			printf( "Sec Offset ... : $%08lx (%ld)\n", pos, pos );
			printf( "File Offset .. : $%08lx (%ld)\n", sec->rfs_FileOffset+pos, sec->rfs_FileOffset+pos );
			printf( "\n" );
//			Log_Dump();
			break;
		}
		#ifdef DEBUG
		else if ( ds != RS4DecodeStat_Okay )
		{
			ec = RS4ErrStat_Internal;

			#ifdef DEBUG
			printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
			#endif

			goto bailout;
		}
		#endif

		memset( & type[ pos ], RS4MT_Code, rt->rt_CPU.M68k.mt_OpcodeSize );

		if ( rt->rt_CPU.M68k.mt_JmpRegister.mr_Type1 != RRT_Unknown )
		{
			switch( rt->rt_CPU.M68k.mt_JmpRegister.mr_Type1 )
			{
				case RRT_Label:
				{
					if ( ! rt->rt_CPU.M68k.mt_JmpRegister.mr_Label )
					{
						ec = RS4ErrStat_Error;
						fs = RS4FuncStat_Error;

						#ifdef DEBUG
						printf( "%s:%04d: Error NULL Pointer\n", __FILE__, __LINE__ );
						#endif

						goto bailout;
					}

					jmpadr = rt->rt_CPU.M68k.mt_JmpRegister.mr_Label->rl_Address;

					rb = RS4AddBrance_File( & ec, rt->rt_File, jmpadr );

					if ( ! rb )
					{
						// ec allready set
						fs = RS4FuncStat_Error;

						#ifdef DEBUG
						printf( "%s:%04d: Error adding brance adr $%08lx\n", __FILE__, __LINE__, jmpadr );
						#endif

						goto bailout;
					}

					rl = RS4FindLabel_File( rt->rt_File, jmpadr );

					if ( ! rl )
					{
						ec = RS4ErrStat_Error;
						fs = RS4FuncStat_Error;

						#ifdef DEBUG
						printf( "%s:%04d: Error adding brance adr $%08lx\n", __FILE__, __LINE__, jmpadr );
						#endif

						goto bailout;
					}

					memcpy( & rb->rb_Registers, & rt->rt_CPU.M68k.mt_Registers, 16 * sizeof( struct M68kRegister ));
					memcpy( & rl->rl_Registers, & rt->rt_CPU.M68k.mt_Registers, 16 * sizeof( struct M68kRegister ));
					break;
				}

				case RRT_Address:
				{
					jmpadr = rt->rt_CPU.M68k.mt_JmpRegister.mr_Address;

					rb = RS4AddBrance_File( & ec, rt->rt_File, jmpadr );

					if ( ! rb )
					{
						// ec allready set
						fs = RS4FuncStat_Error;

						#ifdef DEBUG
						printf( "%s:%04d: Error adding brance adr $%08lx\n", __FILE__, __LINE__, jmpadr );
						#endif

						goto bailout;
					}

					rl = RS4FindLabel_File( rt->rt_File, jmpadr );

					if ( ! rl )
					{
						ec = RS4ErrStat_Error;
						fs = RS4FuncStat_Error;

						#ifdef DEBUG
						printf( "%s:%04d: Error adding brance adr $%08lx\n", __FILE__, __LINE__, jmpadr );
						#endif

						goto bailout;
					}

					memcpy( & rb->rb_Registers, & rt->rt_CPU.M68k.mt_Registers, 16 * sizeof( struct M68kRegister ));
					memcpy( & rl->rl_Registers, & rt->rt_CPU.M68k.mt_Registers, 16 * sizeof( struct M68kRegister ));
					break;
				}

				default:
				{
					ec = RS4ErrStat_Internal;
					fs = RS4FuncStat_Error;

					#ifdef DEBUG
					printf( "%s:%04d: Error : mr_Type (%d)\n", __FILE__, __LINE__, rt->rt_CPU.M68k.mt_JmpRegister.mr_Type1 );
					#endif

					goto bailout;
				}
			}
		}

		// --

		if ( rt->rt_CPU.M68k.mt_ClearRegMask )
		{
			M68k_ClearRegs( rt, rt->rt_CPU.M68k.mt_ClearRegMask );
			rt->rt_CPU.M68k.mt_ClearRegMask = 0;
		}

		// --

		if ( rt->rt_CPU.M68k.mt_LastOpcode )
		{
			break;
		}

		// --

		pos += rt->rt_CPU.M68k.mt_OpcodeSize;
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

enum RS4FuncStat RS4Trace_File( enum RS4ErrorCode *errcode, RS4FileHeader *fh )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Brance *rb;
RS4Trace rt;
char argbuf[256];

	DDEBUG( { printf( "%s:%04d: RS4Trace_File\n", __FILE__, __LINE__ ); } )

	ec = RS4ErrStat_Error;
	fs = RS4FuncStat_Error;

	// --

	#ifdef DEBUG
	if ( ! fh ) 
	{
		ec = RS4ErrStat_Internal;
		printf( "%s:%04d: NULL Pointer\n", __FILE__, __LINE__ );
		goto bailout;
	}
	#endif

	// --

	fs = RS4InitTrace( & ec, & rt, fh, RS4TracePass_Trace );

	if ( fs != RS4FuncStat_Okay )
	{
		// ec allready set

		#ifdef DEBUG
		printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	rt.rt_Container.Hunk.ms_Buf_Argument = argbuf;

	// --
	// Set our start Trace Addresse

	rb = RS4AddBrance_File( & ec, fh, fh->rfh_StartAdr );

	if ( ! rb )
	{
		// ec allready set

		#ifdef DEBUG
		printf( "%s:%04d: Error adding brance adr $%08lx\n", __FILE__, __LINE__, fh->rfh_StartAdr );
		#endif

		goto bailout;
	}

	// --

	fs = RS4ScanForJumps( & ec, & rt, fh );

	if ( fs != RS4FuncStat_Okay )
	{
		// ec allready set

		#ifdef DEBUG
		printf( "%s:%04d: Error \n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	// --

	while( TRUE )
	{
		rb = RS4RemHead( & fh->rfh_BranceList );

		if ( ! rb )
		{
			break;
		}

		fs = RC4TraceBrance( & ec, & rt, rb );

		RS4FreeBrance( NULL, rb );

		if ( fs != RS4FuncStat_Okay )
		{
			// ec allready set

			#ifdef DEBUG
			printf( "%s:%04d: Error tracing brance\n", __FILE__, __LINE__ );
			#endif

			goto bailout;
		}
	}

	// --

	fs = RS4Text_Scan( & ec, fh );

	if ( fs != RS4FuncStat_Okay )
	{
		// ec allready set

		#ifdef DEBUG
		printf( "%s:%04d: Error scanning for text\n", __FILE__, __LINE__ );
		#endif

		goto bailout;
	}

	// --

	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

	// --

bailout:

	// --

	while( TRUE )
	{
		rb = RS4RemHead( & fh->rfh_BranceList  );

		if ( ! rb )
		{
			break;
		}

		RS4FreeBrance( NULL, rb );
	}

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fs );
}

// --
