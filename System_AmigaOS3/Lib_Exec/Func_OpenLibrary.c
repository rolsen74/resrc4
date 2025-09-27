
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

struct myLibType
{
	enum AOS3_LibType	Type;
	STR 				Name;
};

// --

static struct myLibType myLibs[] =
{
{ AOS3_LibType_AmigaguideBase,			"amigaguide.library" },
{ AOS3_LibType_AslBase,					"asl.library" },
{ AOS3_LibType_BulletBase,				"bullet.library" },
{ AOS3_LibType_ColorwheelBase,			"colorwheel.library" },
{ AOS3_LibType_CommoditiesBase,			"commodities.library" },
{ AOS3_LibType_DatatypesBase,			"datatypes.library" },
{ AOS3_LibType_DiskfontBase,			"diskfont.library" },
{ AOS3_LibType_DosBase,					"dos.library" },
{ AOS3_LibType_DTClassBase,				"dtclass.library" },
{ AOS3_LibType_ExecBase,				"exec.library" },
{ AOS3_LibType_ExpansionBase,			"expansion.library" },
{ AOS3_LibType_GadToolsBase,			"gadtools.library" },
{ AOS3_LibType_GraphicsBase,			"graphics.library" },
{ AOS3_LibType_IconBase,				"icon.library" },
{ AOS3_LibType_IFFParseBase,			"iffparse.library" },
{ AOS3_LibType_IntuitionBase,			"intuition.library" },
{ AOS3_LibType_KeymapBase,				"keymap.library" },
{ AOS3_LibType_LayersBase,				"layers.library" },
{ AOS3_LibType_LocaleBase,				"locale.library" },
{ AOS3_LibType_LowlevelBase,			"lowlevel.library" },
{ AOS3_LibType_MathffpBase,				"mathffp.library" },
{ AOS3_LibType_MathieeedoubbasBase,		"mathieeedoubbas.library" },
{ AOS3_LibType_MathieeedoubtransBase,	"mathieeedoubtrans.library" },
{ AOS3_LibType_MathieeesingbasBase,		"mathieeesingbas.library" },
{ AOS3_LibType_MathieeesingtransBase,	"mathieeesingtrans.library" },
{ AOS3_LibType_MathtransBase,			"mathtrans.library" },
{ AOS3_LibType_NonvolatileBase,			"nonvolatile.library" },
{ AOS3_LibType_RexxsyslibBase,			"rexxsyslib.library" },
{ AOS3_LibType_TranslatorBase,			"translator.library" },
{ AOS3_LibType_UtilityBase,				"utility.library" },
{ AOS3_LibType_WorkbenchBase,			"wb.library" },
{ 0, NULL }
};

// --
// Used by OpenLibrary and OldOpenLibrary


enum RS4DecodeStat AOS3_Exec_OpenLibrary_Func( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
RS4Label *rl;
STR buf;
S32 pos;

	ec = RS4ErrStat_Okay;
	ds = RS4DecodeStat_Okay;

	// A1 = Library name
	if ( rt->rt_CPU.M68k.mt_Registers[ M68KREGT_A1 ].mr_Type1 != RRT_Label )
	{
		goto bailout;
	}

	rl = rt->rt_CPU.M68k.mt_Registers[ M68KREGT_A1 ].mr_Label;

	if ( ! rl )
	{
		goto bailout;
	}

	#ifdef DEBUG
	if ( rl->rl_ID != RS4ID_Label )
	{
		ec = RS4ErrStat_Internal;
		ds = RS4DecodeStat_Error;
		printf( "%s:%04d: Error Invalid Label ID (rl %p)\n", __FILE__, __LINE__, rl );
		goto bailout;
	}
	#endif

	buf = (PTR ) rl->rl_Memory;

	if ( ! buf )
	{
		goto bailout;
	}

	// todo?? validate that buf is a ASCII string
	// make Mark_NulString do it

	Mark_NulString( rl );

	pos = 0;

	while( myLibs[pos].Name )
	{
		if ( ! strcmp( buf, myLibs[pos].Name ))
		{
			break;
		}
		else
		{
			pos++;
		}
	}

	if ( myLibs[pos].Name )
	{
		// Don't clear Reg D0
		rt->rt_CPU.M68k.mt_ClearRegMask &= ~( 1 << M68KREGT_D0 ); 
		rt->rt_CPU.M68k.mt_Registers[ M68KREGT_D0 ].mr_Type1 = RRT_Library,
		rt->rt_CPU.M68k.mt_Registers[ M68KREGT_D0 ].mr_Type2 = myLibs[pos].Type;

// printf( "OpenLib: '%s'\n", myLibs[pos].Name );

//	mr_Type1	= RRT_Library,
//	mr_Type2	= AOS3_LibType_
//	mr_Label	= 
//	mr_Address	= 

	}
	else
	{
		// DO clear Reg D0
		rt->rt_CPU.M68k.mt_ClearRegMask |= ( 1 << M68KREGT_D0 ); 

		if ( rt->rt_Pass == RS4TracePass_Trace )
		{
			printf( "Unsupported %s library found at $%08" PRIx64 "\n", buf, rt->rt_CurMemAdr );
		}
	}

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( ds );
}

// --
