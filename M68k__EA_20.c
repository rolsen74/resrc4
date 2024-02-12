
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
// -- Mode 20 - Clr (Ax)

inline static void MODE_20( struct M68kStruct *ms, char *outstr )
{
	sprintf( outstr, "(%s)", Ax_RegNames[ ms->ms_ArgEReg ] );

//	if ( ms->ms_Registers[ REG_Ax + ms->ms_ArgEReg ].mr_Type == RT_Label )
//	{
//		ms->ms_Registers[ REG_Ax + ms->ms_ArgEReg ].mr_Data.mr_LabelNode->hl_Label_Type = LT_Unset;
//	}
}

// --
