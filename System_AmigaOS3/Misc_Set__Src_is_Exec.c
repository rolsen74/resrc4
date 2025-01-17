
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

#include "Resourcer/ReSrc4.h"

// --

#define DDEBUG(x)

// --

enum RS4ErrorCode Misc_Set__Src_is_Exec( 
	struct M68kRegister *dst_reg, 
	RS4Trace *rt,
	uint8_t *mem )
{
enum RS4ErrorCode ec;
RS4Label *dst_rl;
uint32_t val;
	
	ec = RS4ErrStat_Error;

	// Dst is Addr : $00000004.l
	/**/ if (( rt->rt_CPU.M68k.mt_ArgEMode == 7 ) && ( rt->rt_CPU.M68k.mt_ArgEReg == 1 ))	
	{
		val = (( mem[0] << 24 ) | ( mem[1] << 16 ) | ( mem[2] << 8 ) | ( mem[3] << 0 ));

		DDEBUG( printf( "write : 11 :\n" ); )

		dst_rl = RS4FindLabel_File( rt->rt_File, val );

		if (( dst_rl ) && ( ! dst_rl->rl_UserLocked ))
		{
			#ifdef DEBUG
			printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
			#endif
			goto bailout;
		}
	}

	// Dst is Reg : Dx / Ax
	else if (( rt->rt_CPU.M68k.mt_ArgEMode == 0 ) || ( rt->rt_CPU.M68k.mt_ArgEMode == 1 ))
	{
		DDEBUG( printf( "write : 12 : Exec -> Reg %x\n", dst_reg->mr_Number ); )

		// Exec -> Reg
		dst_reg->mr_Type1	= RRT_Library;
		dst_reg->mr_Type2	= AOS3_LibType_ExecBase;
		dst_reg->mr_Label	= NULL;
		dst_reg->mr_Address	= 0;
	}

	// Dst is Unknown : Unsupported
	else
	{
		DDEBUG( printf( "write: 8\n" ); )

		// Do Nothing
	}

	ec = RS4ErrStat_Okay;

bailout:

	return( ec );
}

// --
