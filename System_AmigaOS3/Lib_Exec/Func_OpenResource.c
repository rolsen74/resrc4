
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
	char *				Name;
};

static struct myLibType myRes[] =
{
{ AOS3_LibType_BattclockBase,	"battclock.resource" },
{ AOS3_LibType_BattmemBase,		"battmem.resource" },
{ AOS3_LibType_CardresBase,		"cardres.resource " },
{ AOS3_LibType_CiaBase,			"ciaa.resource" },
{ AOS3_LibType_CiaBase,			"ciab.resource" },
{ AOS3_LibType_DiskBase,		"disk.resource " },
{ AOS3_LibType_MiscBase,		"misc.resource" },
{ AOS3_LibType_PotgoBase,		"potgo.resource" },
{ 0, NULL }
};

// --

enum RS4DecodeStat AOS3_Exec_Func_OpenResource( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
RS4Label *rl;
char *buf;
int pos;

	ec = RS4ErrStat_Okay;
	ds = RS4DecodeStat_Okay;

	// A1 = Resource name
	if ( rt->rt_CPU.M68k.mt_Registers[ M68KREGT_A1 ].mr_Type1 != RRT_Label )
	{
		goto bailout;
	}

	rl = rt->rt_CPU.M68k.mt_Registers[ M68KREGT_A1 ].mr_Label;

	if ( ! rl )
	{
		goto bailout;
	}

	buf = (void *) rl->rl_Memory;

	if ( ! buf )
	{
		goto bailout;
	}

	Mark_NulString( rl );

	pos = 0;

	while( myRes[pos].Name )
	{
		if ( ! strcmp( buf, myRes[pos].Name ))
		{
			break;
		}
		else
		{
			pos++;
		}
	}

	if ( myRes[pos].Name )
	{
		// Don't clear Reg D0
		rt->rt_CPU.M68k.mt_ClearRegMask &= ~( 1 << M68KREGT_D0 ); 
		rt->rt_CPU.M68k.mt_Registers[M68KREGT_D0].mr_Type1 = RRT_Library;
		rt->rt_CPU.M68k.mt_Registers[M68KREGT_D0].mr_Type2 = myRes[pos].Type;
	}
	else
	{
		// DO clear Reg D0
		rt->rt_CPU.M68k.mt_ClearRegMask |= ( 1 << M68KREGT_D0 ); 

		if ( rt->rt_Pass == RS4TracePass_Trace )
		{
			printf( "Unsupported %s Resource found at $%08" PRIx64 "\n", buf, rt->rt_CurMemAdr );
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
