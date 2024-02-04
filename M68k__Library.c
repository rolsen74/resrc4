
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

struct myLibType
{
	enum LibType	Type;
	char *			(*Func)( struct M68kStruct *ms, int16_t val );
};

static struct myLibType myLibs[] =
{
{ LIBT_AmigaguideBase,		M68k_Lib_Amigaguide },
{ LIBT_AslBase,				M68k_Lib_Asl },
{ LIBT_BattclockBase,		M68k_Lib_Battclock },
{ LIBT_BattmemBase,			M68k_Lib_Battmem },
{ LIBT_BulletBase,			M68k_Lib_Bullet },
{ LIBT_CardresBase,			M68k_Lib_Cardres },
{ LIBT_CiaBase,				M68k_Lib_Cia },
{ LIBT_ColorwheelBase,		M68k_Lib_Colorwheel },
{ LIBT_CommoditiesBase,		M68k_Lib_Commodities },
//{ LIBT_ConsoleBase,			M68k_Lib_Console },
{ LIBT_DatatypesBase,		M68k_Lib_Datatypes },
{ LIBT_DiskBase,			M68k_Lib_Disk },
{ LIBT_DiskfontBase,		M68k_Lib_Diskfont },
{ LIBT_DosBase,				M68k_Lib_Dos },
{ LIBT_DTClassBase,			M68k_Lib_DTClass },
{ LIBT_ExecBase,			M68k_Lib_Exec },
{ LIBT_ExpansionBase,		M68k_Lib_Expansion },
{ LIBT_GadToolsBase,		M68k_Lib_Gadtools },
{ LIBT_GraphicsBase,		M68k_Lib_Graphics },
{ LIBT_IconBase,			M68k_Lib_Icon },
{ LIBT_IFFParseBase,		M68k_Lib_IFFParse },
//{ LIBT_InputBase,			M68k_Lib_Input },
{ LIBT_IntuitionBase,		M68k_Lib_Intuition },
{ LIBT_KeymapBase,			M68k_Lib_Keymap },
{ LIBT_LayersBase,			M68k_Lib_Layers },
{ LIBT_LocaleBase,			M68k_Lib_Locale },
{ LIBT_MathffpBase,			M68k_Lib_Mathffp },
{ LIBT_MathieeedoubbasBase,		M68k_Lib_Mathieeedoubbas },
{ LIBT_MathieeedoubtransBase,	M68k_Lib_Mathieeedoubtrans },
{ LIBT_MathieeesingbasBase,		M68k_Lib_Mathieeesingbas },
{ LIBT_MathieeesingtransBase,	M68k_Lib_Mathieeesingtrans },
{ LIBT_MathtransBase,		M68k_Lib_Mathtrans },
{ LIBT_MiscBase,			M68k_Lib_Misc },
{ LIBT_PotgoBase,			M68k_Lib_Potgo },
//{ LIBT_RamdriveBase,		M68k_Lib_Ramdrive },
{ LIBT_RexxsyslibBase,		M68k_Lib_Rexxsyslib },
//{ LIBT_TimerBase,			M68k_Lib_Timer },
{ LIBT_UtilityBase,			M68k_Lib_Utility },
{ LIBT_WorkbenchBase,		M68k_Lib_Workbench },
{ 0, NULL }
};

// --

char *M68k_FindLibFunc( struct M68kStruct *ms, int16_t val )
{
uint32_t type;
char *lvo;
int pos;

	pos = 0;

	type = ( ms->ms_Registers[ REG_Ax + ms->ms_ArgEReg ].mr_Type >> 16 );

	// Yeah an Array is better
	while( myLibs[pos].Func )
	{
		if ( myLibs[pos].Type == type )
		{
			break;
		}
		else
		{
			pos++;
		}
	}

	if ( myLibs[pos].Func )
	{
		lvo = myLibs[pos].Func( ms, val );
	}
	else
	{
		printf( "Unknown libcall at %08x\n", ms->ms_MemoryAdr );
		lvo = NULL;
	}

	return( lvo );
}

// --
