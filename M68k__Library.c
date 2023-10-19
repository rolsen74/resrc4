
/*
 * Copyright (c) 2014-2023 Rene W. Olsen <renewolsen@gmail.com>
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

char *M68k_FindLibFunc( struct M68kStruct *ms, int16_t val )
{
uint32_t type;
char *lvo;

	type = ( ms->ms_Registers[ REG_Ax + ms->ms_ArgEReg ].mr_Type >> 16 );

	switch( type )
	{
		case LIBT_DiskFontBase:
		{
			lvo = M68k_Lib_DiskFont( ms, val );
			break;
		}

		case LIBT_DosBase:
		{
			lvo = M68k_Lib_Dos( ms, val );
			break;
		}

		case LIBT_ExecBase:
		{
			lvo = M68k_Lib_Exec( ms, val );
			break;
		}

		case LIBT_GraphicsBase:
		{
			lvo = M68k_Lib_Graphics( ms, val );
			break;
		}

		case LIBT_IntuitionBase:
		{
			lvo = M68k_Lib_Intuition( ms, val );
			break;
		}

		default:
		{
printf( "libcall unknown %08x\n", ms->ms_MemoryAdr );
			lvo = NULL;
			break;
		}
	}

	return( lvo );
}

// --