
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

enum
{
	CReg_Unknown,
	CReg_SFC,
	CReg_DFC,
	CReg_USP,
	CReg_VBR,

	CReg_CACR,
	CReg_CAAR,
	CReg_MSP,
	CReg_ISP,

	CReg_TC,
	CReg_ITT0,
	CReg_ITT1,
	CReg_DTT0,
	CReg_DTT1,
	CReg_MMUSR,
	CReg_URP,
	CReg_SRP,

	CReg_Last
};

static CSTR CRegNames[CReg_Last] =
{
	[CReg_Unknown] 	= "<?>",

	[CReg_SFC]		= "SFC",	// 000
	[CReg_DFC]		= "DFC",	// 001
	[CReg_USP]		= "USP",	// 800
	[CReg_VBR]		= "VBR",	// 801

	[CReg_CACR]		= "SFC",	// 002
	[CReg_CAAR]		= "DFC",	// 802
	[CReg_MSP]		= "USP",	// 803
	[CReg_ISP]		= "VBR",	// 804

	[CReg_TC]		= "SRP",	// 003
	[CReg_ITT0]		= "ITT0",	// 004
	[CReg_ITT1]		= "ITT1",	// 005
	[CReg_DTT0]		= "DTT0",	// 006
	[CReg_DTT1]		= "DTT1",	// 007
	[CReg_MMUSR]	= "MMUSR",	// 805
	[CReg_URP]		= "URP",	// 806
	[CReg_SRP]		= "SRP",	// 807

};

enum RS4DecodeStat M68kCmd_MOVEC( enum RS4ErrorCode *errcode, RS4Trace *rt )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
CSTR name;
S32 ireg;	// Integer Reg
S32 creg;	// Control Reg 
U32 reg;
S32 dr;		// Direction
S32 ad;		// Type: Ax or Dx

	// --

	dr		= ( rt->rt_CPU.M68k.mt_Opcode & 0x00010000 ) >> 16;
	ad		= ( rt->rt_CPU.M68k.mt_Opcode & 0x00008000 ) >> 15;
	ireg	= ( rt->rt_CPU.M68k.mt_Opcode & 0x00007000 ) >> 12;
	creg	= ( rt->rt_CPU.M68k.mt_Opcode & 0x00000fff ) >> 0;
	name	= ( ad ) ? Ax_RegNames[ ireg ] : Dx_RegNames[ ireg ];

	// --

	rt->rt_Container.Hunk.ms_Str_Opcode = "Movec";
	rt->rt_CPU.M68k.mt_ArgType = M68KSIZE_Long;
	rt->rt_CPU.M68k.mt_OpcodeSize = 4;

	switch( creg )
	{
		case 0x000:		reg = CReg_SFC;		break;
		case 0x001:		reg = CReg_DFC;		break;
		case 0x002:		reg = CReg_CAAR;	break;

		case 0x800:		reg = CReg_USP;		break;
		case 0x801:		reg = CReg_VBR;		break;
		case 0x802:		reg = CReg_CAAR;	break;
		case 0x803:		reg = CReg_MSP;		break;
		case 0x804:		reg = CReg_ISP;		break;

		case 0x003:		reg = CReg_SRP;		break;
		case 0x004:		reg = CReg_ITT0;	break;
		case 0x005:		reg = CReg_ITT1;	break;
		case 0x006:		reg = CReg_DTT0;	break;
		case 0x007:		reg = CReg_DTT1;	break;
		case 0x805:		reg = CReg_MMUSR;	break;
		case 0x806:		reg = CReg_URP;		break;
		case 0x807:		reg = CReg_SRP;		break;

		default:
		{
			reg = CReg_Unknown;
			break;
		}
	}

	// --

	if ( dr )
	{

		sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "%s,%s", name, CRegNames[reg] );

	}
	else
	{

		sprintf( rt->rt_Container.Hunk.ms_Buf_Argument, "%s,%s", CRegNames[reg], name );

		if ( ad )
		{
			rt->rt_CPU.M68k.mt_ClearRegMask |= 1U << ( M68KREGT_A0 + ireg ); 
		}
		else
		{
			rt->rt_CPU.M68k.mt_ClearRegMask |= 1U << ( M68KREGT_D0 + ireg ); 
		}
	}

	// --

	ds = RS4DecodeStat_Okay;
	ec = RS4ErrStat_Okay;

	// --

//bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( ds );
}
