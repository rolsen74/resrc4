
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

enum RS4ErrorCode Misc_Set__Src_is_Unknown( 
	struct AmigaOS3_Misc_Move_GetSetStruct *gss UNUSED,
	struct M68kRegister *dst_reg, 
	RS4Trace *rt,
	uint8_t *mem UNUSED )
{
enum RS4ErrorCode ec;
RS4Label *dst_rl;
uint32_t val;

	ec = RS4ErrStat_Error;

	// Dst is Addr : $00000004.l
	/**/ if (( rt->rt_CPU.M68k.mt_ArgEMode == 7 ) && ( rt->rt_CPU.M68k.mt_ArgEReg == 1 ))	
	{
		DDEBUG( printf( "write : 41 :\n" ); )

		val = (( mem[0] << 24 ) | ( mem[1] << 16 ) | ( mem[2] << 8 ) | ( mem[3] << 0 ));

		dst_rl = RS4FindLabel_File( rt->rt_File, val );

		if (( dst_rl ) && ( ! dst_rl->rl_UserLocked ))
		{
			dst_rl->rl_Type1	= RS4LabelType_Unknown;
		}
	}

	// Dst is Reg : Dx
	else if ( rt->rt_CPU.M68k.mt_ArgEMode == 0 )
	{
		// Reg -> Reg
		dst_reg->mr_Type1 = RRT_Unknown;

		DDEBUG( printf( "write : 42 : destroy Reg %x\n", dst_reg->mr_Number ); )

	}

	// Dst is Reg : Ax
	else if ( rt->rt_CPU.M68k.mt_ArgEMode == 1 )
	{
		// Reg -> Reg
		dst_reg->mr_Type1 = RRT_Unknown;

		DDEBUG( printf( "write : 43 : destroy Reg %x\n", dst_reg->mr_Number ); )

	}

	// Dst is Unknown : Unsupported
	else
	{
		DDEBUG( printf( "write : 44 : \n" ); )

		// Do Nothing
	}

	ec = RS4ErrStat_Okay;

//bailout:

	return( ec );
}

// --
