
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

#include "Resourcer/ReSrc4.h"

// --

#define DDEBUG(x)

// --

enum RS4FuncStat AmigaOS3_Misc_Move_Get( 
	enum RS4ErrorCode *errcode, 
	RS4Trace *rt, 
	struct M68kRegister *cur, 
	MEM cur_mem,
	struct AmigaOS3_Misc_Move_GetSetStruct *gss )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Label *rl;

	ec = RS4ErrStat_Error;
	fs = RS4FuncStat_Error;

	DDEBUG( printf( "\nAmigaOS3_Misc_Move_Get : Addr $%08lx\n", rt->rt_CurMemAdr ); )

	#ifdef DEBUG
	memset( gss, 0x55, sizeof( struct AmigaOS3_Misc_Move_GetSetStruct ));
	#endif

	gss->SrcType = GSSType_Unset;

	/*
	**
	**
	**
	*/

	// Src : $0004.w
	/**/ if (( rt->rt_CPU.M68k.mt_ArgEMode == 7 ) && ( rt->rt_CPU.M68k.mt_ArgEReg == 0 ))
	{
		U16 val = (( cur_mem[0] << 8 ) | ( cur_mem[1] << 0 ));

		if ( val == 4 )
		{
			DDEBUG( printf( "read  : 90.1.1 : $0004.w : Exec : 4.w\n" ); )
			gss->SrcType = GSSType_Exec;
		}
		else
		{
			DDEBUG( printf( "read  : 90.1.2 : $0004.w : ???? : ?.w\n" ); )
			gss->SrcType = GSSType_Unknown;
		}
	}

	/*
	**
	**
	**
	*/

	// Src : $0004.l
	else if (( rt->rt_CPU.M68k.mt_ArgEMode == 7 ) && ( rt->rt_CPU.M68k.mt_ArgEReg == 1 ))
	{
		U32 val = (( cur_mem[0] << 24 ) | ( cur_mem[1] << 16 ) | ( cur_mem[2] << 8 ) | ( cur_mem[3] << 0 ));

		if ( val == 4 )
		{
			DDEBUG( printf( "read  : 90.2.1 : $0004.l : exec : 4.l\n" ); )
			gss->SrcType = GSSType_Exec;
		}
		else
		{
			rl = RS4FindLabel_File( rt->rt_File, val, __FILE__ );

			if ( rl )
			{
				DDEBUG( printf( "read  : 90.2.2 : $0004.l : Label : $%08x.l\n", val ); )

				if (( rl->rl_Type1 == RS4LabelType_Pointer ) && ( rl->rl_Type2 == RS4LabelPtrType_Library ))
				{
					gss->SrcType = GSSType_Label;
					gss->Label = rl;
				}
				else if ( rl->rl_Offset < rt->rt_Section->rfs_MemorySize )
				{
					DDEBUG( printf( "read  : 90.2.2 : $0004.l : Label : $%08x.l\n", val2 ); )

					MEM mem2 = & rl->rl_Memory[ rl->rl_Offset ];
					U32 val2 = (( mem2[0] << 24 ) | ( mem2[1] << 16 ) | ( mem2[2] << 8 ) | ( mem2[3] << 0 ));

					RS4Label *rl2 = RS4FindLabel_File( rt->rt_File, val2, __FILE__ );

					if ( rl2 )
					{
						gss->SrcType = GSSType_Label;
						gss->Label = rl2;
					}
					else
					{
						gss->SrcType = GSSType_Unknown;
					}
				}
				else
				{
					gss->SrcType = GSSType_Unknown;
				}
			}
			else
			{
				DDEBUG( printf( "read  : 90.2.3 : $0004.l : Addr : ??\n" ); )
				gss->SrcType = GSSType_Unknown;
			}
		}
	}

	/*
	**
	**
	**
	*/

	// Dn Reg
	else if ( rt->rt_CPU.M68k.mt_ArgEMode == 0 )
	{
		DDEBUG( printf( "read  : 90.3.1 : Dx Reg : \n" ); )

		gss->SrcType = GSSType_Reg;
		gss->Reg = *cur;
	}

	/*
	**
	**
	**
	*/

	// An Reg
	else if ( rt->rt_CPU.M68k.mt_ArgEMode == 1 )
	{
		DDEBUG( printf( "read  : 90.4.1 : Ax Reg : \n" ); )

		gss->SrcType = GSSType_Reg;
		gss->Reg = *cur;
	}

	/*
	**
	**
	**
	*/

	// (xxx,Ax)
	else if ( rt->rt_CPU.M68k.mt_ArgEMode == 5 )
	{
		DDEBUG( printf( "read  : 90.5.1 : (xxx,Ax) : \n" ); )

		struct M68kRegister *mr;

		mr = & rt->rt_CPU.M68k.mt_Registers[ M68KREGT_Ax + rt->rt_CPU.M68k.mt_ArgEReg ];

		if (( mr->mr_Type1 == RRT_Label ) && ( mr->mr_Label ))
		{
			S16 off = (( cur_mem[0] << 8 ) | ( cur_mem[1] << 0 ));
			S64 adr = mr->mr_Label->rl_Address + off;

			rl = RS4FindLabel_File( rt->rt_File, adr, __FILE__ );

			if ( rl )
			{
				DDEBUG( printf( "read  : 90.5.2 : (xxx,Ax) : Label :\n" ); )
				gss->SrcType = GSSType_Label;
				gss->Label = rl;
			}
			else
			{
				DDEBUG( printf( "read  : 90.5.3 : (xxx,Ax) : Addr : ??\n" ); )
				gss->SrcType = GSSType_Unknown;
			}
		}
		else
		{
			DDEBUG( printf( "read  : 90.5.4 : (xxx,Ax) : Unknown\n" ); )
			gss->SrcType = GSSType_Unknown;
		}
	}

	/*
	**
	**
	**
	*/

	// Src : # $0004.l
	else if (( rt->rt_CPU.M68k.mt_ArgEMode == 7 ) && ( rt->rt_CPU.M68k.mt_ArgEReg == 4 ))
	{
		DDEBUG( printf( "read  : 90.6.1 : #$xxxxxxxx.l \n" ); )

		U32 val = (( cur_mem[0] << 24 ) | ( cur_mem[1] << 16 ) | ( cur_mem[2] << 8 ) | ( cur_mem[3] << 0 ));

		rl = RS4FindLabel_File( rt->rt_File, val, __FILE__ );

		if ( rl )
		{
			DDEBUG( printf( "read  : 90.6.3 : $0004.l : Label : #$xxxxxxxx.l : Addr $%08x\n", val ); )
			gss->SrcType = GSSType_Label;
			gss->Label = rl;
		}
		else
		{
			DDEBUG( printf( "read  : 90.6.7 : #$xxxxxxxx.l : Addr  $%08x: ??\n", val ); )
			gss->SrcType = GSSType_Unknown;
		}
	}

	/*
	**
	**
	**
	*/

	// Src : (xxxx.w,pc)
	else if (( rt->rt_CPU.M68k.mt_ArgEMode == 7 ) && ( rt->rt_CPU.M68k.mt_ArgEReg == 2 ))
	{
		DDEBUG( printf( "read  : 90.7.1 : (xxxx,PC) : implement me\n" ); )

		S16 val = (( cur_mem[0] << 8 ) | ( cur_mem[1] << 0 ));
		S64 adr = rt->rt_CurMemAdr + 2 + val;

		rl = RS4FindLabel_File( rt->rt_File, adr, __FILE__ );

		if ( rl )
		{
			DDEBUG( printf( "read  : 90.7.3 : $0004.l : Label : #$xxxxxxxx.l : Addr $%08x\n", val ); )
			gss->SrcType = GSSType_Label;
			gss->Label = rl;
		}
		else
		{
			DDEBUG( printf( "read  : 90.7.7 : #$xxxxxxxx.l : Addr  $%08x: ??\n", val ); )
			gss->SrcType = GSSType_Unknown;
		}
	}

	/*
	**
	**
	**
	*/

	// Unknown
	else
	{
		DDEBUG( printf( "read  : 90.8.1 : Unknown\n" ); )
		gss->SrcType = GSSType_Unknown;
	}

	/*
	**
	**
	**
	*/

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
