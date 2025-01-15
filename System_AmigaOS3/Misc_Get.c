
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

#include "Resourcer/ReSrc4.h"

// --

#define DDEBUG(x)

// --

enum RS4FuncStat AmigaOS3_Misc_Move_Get( 
	enum RS4ErrorCode *errcode, 
	RS4Trace *rt, struct 
	M68kRegister *cur, 
	uint8_t *mem, 
	struct AmigaOS3_Misc_Move_GetSetStruct *gss )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Label *rl;
uint32_t val;

	ec = RS4ErrStat_Error;
	fs = RS4FuncStat_Error;

	DDEBUG( printf( "\nAmigaOS3_Misc_Move_Get\n" ); )

	#ifdef DEBUG
	memset( gss, 0x55, sizeof( struct AmigaOS3_Misc_Move_GetSetStruct ));
	#endif

	gss->SrcType = _Unset;

	// --

	/**/ if (( rt->rt_CPU.M68k.mt_ArgEMode == 7 ) && ( rt->rt_CPU.M68k.mt_ArgEReg == 0 ))	// Move.l $0004.w
	{
		val = (( mem[0] << 8 ) | ( mem[1] << 0 ));

		if ( val == 4 )
		{
			DDEBUG( printf( "read  : 90 : exec : 4.w\n" ); )
			gss->SrcType = _Exec;
		}
		else
		{
			DDEBUG( printf( "read  : 91 : ???? : ?.w\n" ); )
			gss->SrcType = _Unknown;
		}
	}
	else if (( rt->rt_CPU.M68k.mt_ArgEMode == 7 ) && ( rt->rt_CPU.M68k.mt_ArgEReg == 1 ))	// Move.l $00000004
	{
		val = (( mem[0] << 24 ) | ( mem[1] << 16 ) | ( mem[2] << 8 ) | ( mem[3] << 0 ));

		if ( val == 4 )
		{
			DDEBUG( printf( "read  : 92 : exec : 4.l\n" ); )
			gss->SrcType = _Exec;
		}
		else
		{
			rl = RS4FindLabel_File( rt->rt_File, val );

			if ( rl )
			{
				DDEBUG( printf( "read  : 93 : label : x.l\n" ); )
				gss->SrcType = _Label;
				gss->Label = rl;
			}
			else
			{
				DDEBUG( printf( "read  : 94 : label : ??\n" ); )
				gss->SrcType = _Unknown;
			}
		}
	}
	else if ( rt->rt_CPU.M68k.mt_ArgEMode == 0 )	// Dn Reg
	{
		DDEBUG( printf( "read  : 95 : Dx Reg : Reg %x\n", cur->mr_Number ); )
		gss->SrcType = _Reg;
		gss->Reg = *cur;

		#ifdef DEBUG
		if (( cur->mr_Type1 == RRT_Label ) && ( ! cur->mr_Label ))
		{
			printf( "Reg Label is NULL $%08lx\n", rt->rt_CurMemAdr );
			printf( "Opcode: %s\n", rt->rt_Container.Hunk.ms_Str_Opcode );
		}
		#endif
	}
	else if ( rt->rt_CPU.M68k.mt_ArgEMode == 1 )	// An Reg
	{
		DDEBUG( printf( "read  : 96 : Ax Reg : Reg %x\n", cur->mr_Number ); )
		gss->SrcType = _Reg;
		gss->Reg = *cur;

		#ifdef DEBUG
		if (( cur->mr_Type1 == RRT_Label ) && ( ! cur->mr_Label ))
		{
			printf( "Reg Label is NULL $%08lx\n", rt->rt_CurMemAdr );
			printf( "Opcode: %s\n", rt->rt_Container.Hunk.ms_Str_Opcode );
		}
		#endif
	}
	else
	{
		DDEBUG( printf( "read  : 97 : Unknown\n" ); )
		gss->SrcType = _Unknown;
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
