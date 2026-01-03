
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

enum RS4ErrorCode Misc_Set__Src_is_Unknown( 
	struct AmigaOS3_Misc_Move_GetSetStruct *gss UNUSED,
	struct M68kRegister *dst_reg, 
	RS4Trace *rt,
	MEM mem )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Label *dst_rl;

ec = RS4ErrStat_Error;

	// Dst is Addr : $00000004.l
	/**/ if (( rt->rt_CPU.M68k.mt_ArgEMode == 7 ) && ( rt->rt_CPU.M68k.mt_ArgEReg == 1 ))	
	{
		DDEBUG( printf( "write : 41 :\n" ); )

		U32 val = (( mem[0] << 24 ) | ( mem[1] << 16 ) | ( mem[2] << 8 ) | ( mem[3] << 0 ));

		ERR_CHK( RS4FindLabel_File( & ec, rt->rt_File, & dst_rl, val, __FILE__ ))

		if (( dst_rl ) && ( ! dst_rl->rl_UserLocked ))
		{
			dst_rl->rl_Type1 = RS4LabelType_Unknown;
			dst_rl->rl_Type2 = 0;
			dst_rl->rl_Type3 = 0;
		}
	}

	// Dst is Reg : Dx
	else if ( rt->rt_CPU.M68k.mt_ArgEMode == 0 )
	{
		// Reg -> Reg
		dst_reg->mr_Type1 = RRT_Unknown;

		DDEBUG( printf( "write : 42 : destroy Reg\n" ); )

	}

	// Dst is Reg : Ax
	else if ( rt->rt_CPU.M68k.mt_ArgEMode == 1 )
	{
		// Reg -> Reg
		dst_reg->mr_Type1 = RRT_Unknown;

		DDEBUG( printf( "write : 43 : destroy Reg\n" ); )

	}

	// Dst is Unknown : Unsupported
	else
	{
		DDEBUG( printf( "write : 44 : \n" ); )

		// Do Nothing
	}

	ec = RS4ErrStat_Okay;

bailout:

	return( ec );
}

// --
