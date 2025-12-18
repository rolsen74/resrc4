
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
// -- Mode 60 - Full Extension Word Format

enum RS4DecodeStat MODE_60_Full_0( enum RS4ErrorCode *errcode, RS4Trace *rt, STR outstr )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
MEM mem;
S32 SCALE;
S32 mode;
S32 REG;
S32 pos;
S32 IIS;
S32 AD;
S32 WL;
S32 BS;
S32 IS;
S32 BD;

	// --

	ec = RS4ErrStat_Error;
	ds = RS4DecodeStat_Error;

	// --

	// Full Extension Word Format

	// [A/D] [REG] [W/L] [Scale] [1]  [BS] [IS] [BD SIZE]  [0] [I/IS]
	//   1     3     1      2     1    1    1       2       1    3
	// [  4 bit  ] [     4 bit     ]  [      4 bit      ]  [  4bit  ]

	mem = rt->rt_CurMemBuf;
	pos = rt->rt_CPU.M68k.mt_ArgSize;

	AD		= ( mem[ pos + 0 ] & 0x80 );
	REG		= ( mem[ pos + 0 ] & 0x70 ) >> 4;
	WL		= ( mem[ pos + 0 ] & 0x08 );
	SCALE	= ( mem[ pos + 0 ] & 0x06 ) >> 1;
	BS		= ( mem[ pos + 1 ] & 0x80 );
	IS		= ( mem[ pos + 1 ] & 0x40 );
	BD		= ( mem[ pos + 1 ] & 0x30 ) >> 4;
	IIS		= ( mem[ pos + 1 ] & 0x07 );

	mode	 = BD;
	mode	|= ( IIS ) << 4;
	mode	|= ( IS ) ? 0x0100 : 0x0000 ;
	mode	|= ( BS ) ? 0x1000 : 0x0000 ;

	switch( mode )
	{
		#if 0

		// Todo: the .l can be Pointers, check IsRef()
	
		// From EA_60
		case 0x0002: // BS 0, IS 0, IIS 0, BD 2 - ($0086.w,a5,d0.w*4), AsmPro wants (a5,d0.w*4,$0086.w)
		case 0x0003: // BS 0, IS 0, IIS 0, BD 3 - ($0086.l,a5,d0.w*4), AsmPro wants (a5,d0.w*4,$0086.l)

		case 0x0011: // BS 0, IS 0, IIS 1, BD 2 - ([a5,d0.w*4])
		case 0x0012: // BS 0, IS 0, IIS 1, BD 2 - ([$0086.w,a5,d0.w*4])
		case 0x0013: // BS 0, IS 0, IIS 1, BD 3 - ([$0086.l,a5,d0.w*4])

		case 0x0021: // BS 0, IS 0, IIS 3, BD 1 - ([a5,d0.w*4],$11.w)
		case 0x0022: // BS 0, IS 0, IIS 2, BD 2 - ([$0086.w,a5,d0.w*4],$11.w)
		case 0x0023: // BS 0, IS 0, IIS 2, BD 3 - ([$0086.l,a5,d0.w*4],$11.w)

		case 0x0031: // BS 0, IS 0, IIS 3, BD 1 - ([a5,d0.w*4],$11.l)
		case 0x0032: // BS 0, IS 0, IIS 3, BD 2 - ([$0086.w,a5,d0.w*4],$11.l)
		case 0x0033: // BS 0, IS 0, IIS 3, BD 3 - ([$0086.l,a5,d0.w*4],$11.l)

		case 0x0051: // BS 0, IS 0, IIS 7, BD 1 - ([a5],d0.w*4)
		case 0x0052: // BS 0, IS 0, IIS 7, BD 2 - ([$0086.w,a5],d0.w*4)
		case 0x0053: // BS 0, IS 0, IIS 7, BD 3 - ([$0086.l,a5],d0.w*4)

		case 0x0061: // BS 0, IS 0, IIS 6, BD 1 - ([a5],d0.w*4,$11.w)
		case 0x0062: // BS 0, IS 0, IIS 6, BD 2 - ([$0086.w,a5],d0.w*4,$11.w)
		case 0x0063: // BS 0, IS 0, IIS 6, BD 3 - ([$0086.l,a5],d0.w*4,$11.w)

		case 0x0071: // BS 0, IS 0, IIS 7, BD 1 - ([a5],d0.w*4,$11.l)
		case 0x0072: // BS 0, IS 0, IIS 7, BD 2 - ([$0086.w,a5],d0.w*4,$11.l)
		case 0x0073: // BS 0, IS 0, IIS 7, BD 3 - ([$0086.l,a5],d0.w*4,$11.l)

		case 0x0111: // BS 0, IS 1, IIS 1, BD 1 - ([A7])
		case 0x0112: // BS 0, IS 1, IIS 1, BD 2 - ([$0004.w,A7])
		case 0x0113: // BS 0, IS 1, IIS 1, BD 3 - ([$0004.l,A7])

		case 0x0121: // BS 0, IS 1, IIS 2, BD 1 - ([A7],$0044.w)
		case 0x0122: // BS 0, IS 1, IIS 2, BD 2 - ([$0004.w,A7],$0044.w)
		case 0x0123: // BS 0, IS 1, IIS 2, BD 3 - ([$0004.l,A7],$0044.w)

		case 0x0131: // BS 0, IS 1, IIS 3, BD 1 - ([A7],$0044.l)
		case 0x0132: // BS 0, IS 1, IIS 3, BD 2 - ([$0004.w,A7],$0044.l)
		case 0x0133: // BS 0, IS 1, IIS 3, BD 3 - ([$0004.l,A7],$0044.l)

		// From EA_73

		case 0x0002: // BS 0, IS 0, IIS 0, BD 2 - (label.w,PC,d0.w*4)
		case 0x0003: // BS 0, IS 0, IIS 0, BD 3 - (label.l,PC,d0.w*4)

		case 0x0011: // BS 0, IS 0, IIS 1, BD 1 - ([PC,d0.w*4])
		case 0x0012: // BS 0, IS 0, IIS 1, BD 2 - ([label.w,PC,d0.w*4])
		case 0x0013: // BS 0, IS 0, IIS 1, BD 3 - ([label.l,PC,d0.w*4])

		case 0x0021: // BS 0, IS 0, IIS 2, BD 1 - ([PC,d0.w*4],$11.w)
		case 0x0022: // BS 0, IS 0, IIS 2, BD 2 - ([label.w,PC,d0.w*4],$11.w)
		case 0x0023: // BS 0, IS 0, IIS 2, BD 3 - ([label.l,PC,d0.w*4],$11.w)

		case 0x0031: // BS 0, IS 0, IIS 3, BD 1 - ([PC,d0.w*4],$11.l)
		case 0x0032: // BS 0, IS 0, IIS 3, BD 2 - ([label.w,PC,d0.w*4],$11.l)
		case 0x0033: // BS 0, IS 0, IIS 3, BD 3 - ([label.l,PC,d0.w*4],$11.l)

		case 0x0051: // BS 0, IS 0, IIS 5, BD 1 - ([PC],d0.w*4)
		case 0x0052: // BS 0, IS 0, IIS 5, BD 2 - ([label.w,PC],d0.w*4)
		case 0x0053: // BS 0, IS 0, IIS 5, BD 3 - ([label.l,PC],d0.w*4)

		case 0x0061: // BS 0, IS 0, IIS 6, BD 1 - ([PC],d0.w*4,$11.w)
		case 0x0062: // BS 0, IS 0, IIS 6, BD 2 - ([label.w,PC],d0.w*4,$11.w)
		case 0x0063: // BS 0, IS 0, IIS 6, BD 3 - ([label.l,PC],d0.w*4,$11.w)

		case 0x0071: // BS 0, IS 0, IIS 7, BD 1 - ([PC],d0.w*4,$11.l)
		case 0x0072: // BS 0, IS 0, IIS 7, BD 2 - ([label.w,PC],d0.w*4,$11.l)
		case 0x0073: // BS 0, IS 0, IIS 7, BD 3 - ([label.l,PC],d0.w*4,$11.l)

		case 0x0103: // BS 0, IS 1, IIS 0, BD 3 - (label.l,PC)

		case 0x0111: // BS 0, IS 1, IIS 1, BD 1 - ([PC])
		case 0x0112: // BS 0, IS 1, IIS 1, BD 2 - ([label.w,PC])
		case 0x0113: // BS 0, IS 1, IIS 1, BD 3 - ([label.l,PC])

		case 0x0121: // BS 0, IS 1, IIS 2, BD 1 - ([PC],$22.w)
		case 0x0122: // BS 0, IS 1, IIS 2, BD 2 - ([label.w,PC],$22.w)
		case 0x0123: // BS 0, IS 1, IIS 2, BD 3 - ([label.l,PC],$22.w)

		case 0x0131: // BS 0, IS 1, IIS 3, BD 1 - ([PC],$44.l)
		case 0x0132: // BS 0, IS 1, IIS 3, BD 2 - ([label.w,PC],$44.l)
		case 0x0133: // BS 0, IS 1, IIS 3, BD 3 - ([label.l,PC],$44.l)

		// -- fast vasm test of mode 73
		// 1 ( d8 , PC , Xn.SIZE * SCALE )
		// 2 ( bd , PC , Xn.SIZE * SCALE )
		// 3 ( [ bd , PC ] , Xn.SIZE * SCALE , od )
		// 4 ( [ bd , PC , Xn.SIZE * SCALE ] , od )
		#endif

		case 0x0002: // BS 0, IS 0, IIS 0, BD 2 - ($0086.w,a5,d0.w*4), AsmPro wants (a5,d0.w*4,$0086.w)
		{
			S16 v1 = ( mem[ pos + 2 ] << 8 | mem[ pos + 3 ] );

			sprintf( outstr, "(%d.w,%s,%s%s%s)", 
				v1,
				Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ],
				( AD ) ? Ax_RegNames[ REG ] : Dx_RegNames[ REG ],
				( WL ) ? ".l" : ".w",
				scale_Names[ SCALE ]
			);

			rt->rt_CPU.M68k.mt_ArgSize += 4;
			break;
		}

		case 0x0003: // BS 0, IS 0, IIS 0, BD 3 - ($0086.l,a5,d0.w*4), AsmPro wants (a5,d0.w*4,$0086.l)
		{
			S32 v1 = ( mem[ pos + 2 ] << 24 | mem[ pos + 3 ] << 16 | mem[ pos + 4 ] << 8 | mem[ pos + 5 ] );

			sprintf( outstr, "(%d.l,%s,%s%s%s)", 
				v1,
				Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ],
				( AD ) ? Ax_RegNames[ REG ] : Dx_RegNames[ REG ],
				( WL ) ? ".l" : ".w",
				scale_Names[ SCALE ]
			);

			rt->rt_CPU.M68k.mt_ArgSize += 6;
			break;
		}

		case 0x0011: // BS 0, IS 0, IIS 1, BD 2 - ([a5,d0.w*4])
		{
			sprintf( outstr, "([%s,%s%s%s])",
				Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ],
				( AD ) ? Ax_RegNames[ REG ] : Dx_RegNames[ REG ],
				( WL ) ? ".l" : ".w",
				scale_Names[ SCALE ]
			);

			rt->rt_CPU.M68k.mt_ArgSize += 2;
			break;
		}

		case 0x0012: // BS 0, IS 0, IIS 1, BD 2 - ([$0086.w,a5,d0.w*4])
		{
			S16 v1 = ( mem[ pos + 2 ] << 8 | mem[ pos + 3 ] );

			sprintf( outstr, "([%d.w,%s,%s%s%s])", 
				v1,
				Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ],
				( AD ) ? Ax_RegNames[ REG ] : Dx_RegNames[ REG ],
				( WL ) ? ".l" : ".w",
				scale_Names[ SCALE ]
			);

			rt->rt_CPU.M68k.mt_ArgSize += 4;
			break;
		}

		case 0x0013: // BS 0, IS 0, IIS 1, BD 3 - ([$0086.l,a5,d0.w*4])
		{
			S32 v1 = ( mem[ pos + 2 ] << 24 | mem[ pos + 3 ] << 16 | mem[ pos + 4 ] << 8 | mem[ pos + 5 ] );

			sprintf( outstr, "([%d.l,%s,%s%s%s])", 
				v1,
				Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ],
				( AD ) ? Ax_RegNames[ REG ] : Dx_RegNames[ REG ],
				( WL ) ? ".l" : ".w",
				scale_Names[ SCALE ]
			);

			rt->rt_CPU.M68k.mt_ArgSize += 6;
			break;
		}

		case 0x0021: // BS 0, IS 0, IIS 2, BD 1 - ([a5,d0.w*4],$11.w)
		{
			S16 v1 = ( mem[ pos + 2 ] << 8 | mem[ pos + 3 ] );

			sprintf( outstr, "([%s,%s%s%s],%d.w)",
				Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ],
				( AD ) ? Ax_RegNames[ REG ] : Dx_RegNames[ REG ],
				( WL ) ? ".l" : ".w",
				scale_Names[ SCALE ],
				v1
			);

			rt->rt_CPU.M68k.mt_ArgSize += 4;
			break;
		}

		case 0x0022: // BS 0, IS 0, IIS 2, BD 2 - ([$0086.w,a5,d0.w*4],$11.w)
		{
			S16 v1 = ( mem[ pos + 2 ] << 8 | mem[ pos + 3 ] );
			S16 v2 = ( mem[ pos + 4 ] << 8 | mem[ pos + 5 ] );

			sprintf( outstr, "([%d.w,%s,%s%s%s],%d.w)",
				v1,
				Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ],
				( AD ) ? Ax_RegNames[ REG ] : Dx_RegNames[ REG ],
				( WL ) ? ".l" : ".w",
				scale_Names[ SCALE ],
				v2
			);

			rt->rt_CPU.M68k.mt_ArgSize += 6;
			break;
		}

		case 0x0023: // BS 0, IS 0, IIS 2, BD 3 - ([$0086.l,a5,d0.w*4],$11.w)
		{
			S32 v1 = ( mem[ pos + 2 ] << 24 | mem[ pos + 3 ] << 16 | mem[ pos + 4 ] << 8 | mem[ pos + 5 ] );
			S16 v2 = ( mem[ pos + 6 ] <<  8 | mem[ pos + 7 ] );

			sprintf( outstr, "([%d.l,%s,%s%s%s],%d.w)",
				v1,
				Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ],
				( AD ) ? Ax_RegNames[ REG ] : Dx_RegNames[ REG ],
				( WL ) ? ".l" : ".w",
				scale_Names[ SCALE ],
				v2
			);

			rt->rt_CPU.M68k.mt_ArgSize += 8;
			break;
		}

		case 0x0031: // BS 0, IS 0, IIS 3, BD 1 - ([a5,d0.w*4],$11.l)
		{
			S32 v1 = ( mem[ pos + 2 ] << 24 | mem[ pos + 3 ] << 16 | mem[ pos + 4 ] << 8 | mem[ pos + 5 ] );

			sprintf( outstr, "([%s,%s%s%s],%d.l)",
				Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ],
				( AD ) ? Ax_RegNames[ REG ] : Dx_RegNames[ REG ],
				( WL ) ? ".l" : ".w",
				scale_Names[ SCALE ],
				v1
			);

			rt->rt_CPU.M68k.mt_ArgSize += 6;
			break;
		}

		case 0x0032: // BS 0, IS 0, IIS 3, BD 2 - ([$0086.w,a5,d0.w*4],$11.l)
		{
			S16 v1 = ( mem[ pos + 2 ] <<  8 | mem[ pos + 3 ] );
			S32 v2 = ( mem[ pos + 4 ] << 24 | mem[ pos + 5 ] << 16 | mem[ pos + 6 ] << 8 | mem[ pos + 7 ] );

			sprintf( outstr, "([%d.w,%s,%s%s%s],%d.l)",
				v1,
				Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ],
				( AD ) ? Ax_RegNames[ REG ] : Dx_RegNames[ REG ],
				( WL ) ? ".l" : ".w",
				scale_Names[ SCALE ],
				v2
			);

			rt->rt_CPU.M68k.mt_ArgSize += 8;
			break;
		}

		case 0x0033: // BS 0, IS 0, IIS 3, BD 3 - ([$0086.l,a5,d0.w*4],$11.l)
		{
			S32 v1 = ( mem[ pos + 2 ] << 24 | mem[ pos + 3 ] << 16 | mem[ pos + 4 ] << 8 | mem[ pos + 5 ] );
			S32 v2 = ( mem[ pos + 6 ] << 24 | mem[ pos + 7 ] << 16 | mem[ pos + 8 ] << 8 | mem[ pos + 9 ] );

			sprintf( outstr, "([%d.l,%s,%s%s%s],%d.l)",
				v1,
				Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ],
				( AD ) ? Ax_RegNames[ REG ] : Dx_RegNames[ REG ],
				( WL ) ? ".l" : ".w",
				scale_Names[ SCALE ],
				v2
			);

			rt->rt_CPU.M68k.mt_ArgSize += 10;
			break;
		}

		case 0x0051: // BS 0, IS 0, IIS 5, BD 1 - ([a5],d0.w*4)
		{
			sprintf( outstr, "([%s],%s%s%s)",
				Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ],
				( AD ) ? Ax_RegNames[ REG ] : Dx_RegNames[ REG ],
				( WL ) ? ".l" : ".w",
				scale_Names[ SCALE ]
			);

			rt->rt_CPU.M68k.mt_ArgSize += 2;
			break;
		}

		case 0x0052: // BS 0, IS 0, IIS 5, BD 2 - ([$0086.w,a5],d0.w*4)
		{
			S16 v1 = ( mem[ pos + 2 ] <<  8 | mem[ pos + 3 ] );

			sprintf( outstr, "([%d.w,%s],%s%s%s)",
				v1,
				Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ],
				( AD ) ? Ax_RegNames[ REG ] : Dx_RegNames[ REG ],
				( WL ) ? ".l" : ".w",
				scale_Names[ SCALE ]
			);

			rt->rt_CPU.M68k.mt_ArgSize += 4;
			break;
		}

		case 0x0053: // BS 0, IS 0, IIS 5, BD 3 - ([$0086.l,a5],d0.w*4)
		{
			S32 v1 = ( mem[ pos + 2 ] << 24 | mem[ pos + 3 ] << 16 | mem[ pos + 4 ] << 8 | mem[ pos + 5 ] );

			sprintf( outstr, "([%d.l,%s],%s%s%s)",
				v1,
				Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ],
				( AD ) ? Ax_RegNames[ REG ] : Dx_RegNames[ REG ],
				( WL ) ? ".l" : ".w",
				scale_Names[ SCALE ]
			);

			rt->rt_CPU.M68k.mt_ArgSize += 6;
			break;
		}

		case 0x0061: // BS 0, IS 0, IIS 6, BD 1 - ([a5],d0.w*4,$11.w)
		{
			S16 v1 = ( mem[ pos + 2 ] <<  8 | mem[ pos + 3 ] );

			sprintf( outstr, "([%s],%s%s%s,%d.w)",
				Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ],
				( AD ) ? Ax_RegNames[ REG ] : Dx_RegNames[ REG ],
				( WL ) ? ".l" : ".w",
				scale_Names[ SCALE ],
				v1
			);

			rt->rt_CPU.M68k.mt_ArgSize += 4;
			break;
		}

		case 0x0062: // BS 0, IS 0, IIS 6, BD 2 - ([$0086.w,a5],d0.w*4,$11.w)
		{
			S16 v1 = ( mem[ pos + 2 ] <<  8 | mem[ pos + 3 ] );
			S16 v2 = ( mem[ pos + 4 ] <<  8 | mem[ pos + 5 ] );

			sprintf( outstr, "([%d.w,%s],%s%s%s,%d.w)",
				v1,
				Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ],
				( AD ) ? Ax_RegNames[ REG ] : Dx_RegNames[ REG ],
				( WL ) ? ".l" : ".w",
				scale_Names[ SCALE ],
				v2
			);

			rt->rt_CPU.M68k.mt_ArgSize += 6;
			break;
		}

		case 0x0063: // BS 0, IS 0, IIS 6, BD 3 - ([$0086.l,a5],d0.w*4,$11.w)
		{
			S32 v1 = ( mem[ pos + 2 ] << 24 | mem[ pos + 3 ] << 16 | mem[ pos + 4 ] << 8 | mem[ pos + 5 ] );
			S16 v2 = ( mem[ pos + 4 ] <<  8 | mem[ pos + 5 ] );

			sprintf( outstr, "([%d.l,%s],%s%s%s,%d.w)",
				v1,
				Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ],
				( AD ) ? Ax_RegNames[ REG ] : Dx_RegNames[ REG ],
				( WL ) ? ".l" : ".w",
				scale_Names[ SCALE ],
				v2
			);

			rt->rt_CPU.M68k.mt_ArgSize += 8;
			break;
		}

		case 0x0071: // BS 0, IS 0, IIS 7, BD 1 - ([a5],d0.w*4,$11.l)
		{
			S32 v1 = ( mem[ pos + 2 ] << 24 | mem[ pos + 3 ] << 16 | mem[ pos + 4 ] << 8 | mem[ pos + 5 ] );

			sprintf( outstr, "([%s],%s%s%s,%d.l)",
				Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ],
				( AD ) ? Ax_RegNames[ REG ] : Dx_RegNames[ REG ],
				( WL ) ? ".l" : ".w",
				scale_Names[ SCALE ],
				v1
			);

			rt->rt_CPU.M68k.mt_ArgSize += 6;
			break;
		}

		case 0x0072: // BS 0, IS 0, IIS 7, BD 2 - ([$0086.w,a5],d0.w*4,$11.l)
		{
			S16 v1 = ( mem[ pos + 2 ] <<  8 | mem[ pos + 3 ] );
			S32 v2 = ( mem[ pos + 4 ] << 24 | mem[ pos + 5 ] << 16 | mem[ pos + 6 ] << 8 | mem[ pos + 7 ] );

			sprintf( outstr, "([%d.w,%s],%s%s%s,%d.l)",
				v1,
				Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ],
				( AD ) ? Ax_RegNames[ REG ] : Dx_RegNames[ REG ],
				( WL ) ? ".l" : ".w",
				scale_Names[ SCALE ],
				v2
			);

			rt->rt_CPU.M68k.mt_ArgSize += 8;
			break;
		}

		case 0x0073: // BS 0, IS 0, IIS 7, BD 3 - ([$0086.l,a5],d0.w*4,$11.l)
		{
			S32 v1 = ( mem[ pos + 2 ] << 24 | mem[ pos + 3 ] << 16 | mem[ pos + 4 ] << 8 | mem[ pos + 5 ] );
			S32 v2 = ( mem[ pos + 6 ] << 24 | mem[ pos + 7 ] << 16 | mem[ pos + 8 ] << 8 | mem[ pos + 9 ] );

			sprintf( outstr, "([%d.l,%s],%s%s%s,%d.l)",
				v1,
				Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ],
				( AD ) ? Ax_RegNames[ REG ] : Dx_RegNames[ REG ],
				( WL ) ? ".l" : ".w",
				scale_Names[ SCALE ],
				v2
			);

			rt->rt_CPU.M68k.mt_ArgSize += 10;
			break;
		}

		case 0x0111: // BS 0, IS 1, IIS 1, BD 1 - ([A7])
		{
			sprintf( outstr, "([%s])", 
				Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ]
			);

			rt->rt_CPU.M68k.mt_ArgSize += 2;
			break;
		}

		case 0x0112: // BS 0, IS 1, IIS 1, BD 2 - ([$0004.w,A7])
		{
			S16 v1 = ( mem[ pos + 2 ] << 8 | mem[ pos + 3 ] );

			sprintf( outstr, "([%d.w,%s])", 
				v1, 
				Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ]
			);

			rt->rt_CPU.M68k.mt_ArgSize += 4;
			break;
		}

		case 0x0113: // BS 0, IS 1, IIS 1, BD 3 - ([$0004.l,A7])
		{
			S32 v1 = ( mem[ pos + 2 ] << 24 | mem[ pos + 3 ] << 16 | mem[ pos + 4 ] << 8 | mem[ pos + 5 ] );

			sprintf( outstr, "([%d.l,%s])", 
				v1, 
				Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ]
			);

			rt->rt_CPU.M68k.mt_ArgSize += 6;
			break;
		}

		case 0x0121: // BS 0, IS 1, IIS 2, BD 1 - ([A7],$0044.w)
		{
			S16 v1 = ( mem[ pos + 2 ] << 8 | mem[ pos + 3 ] );

			sprintf( outstr, "([%s],%d.w)", 
				Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ],
				v1
			);

			rt->rt_CPU.M68k.mt_ArgSize += 4;
			break;
		}

		case 0x0122: // BS 0, IS 1, IIS 2, BD 2 - ([$0004.w,A7],$0044.w)
		{
			S16 v1 = ( mem[ pos + 2 ] << 8 | mem[ pos + 3 ] );
			S16 v2 = ( mem[ pos + 4 ] << 8 | mem[ pos + 5 ] );

			sprintf( outstr, "([%d.w,%s],%d.w)", 
				v1, 
				Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ],
				v2 
			);

			rt->rt_CPU.M68k.mt_ArgSize += 6;
			break;
		}

		case 0x0123: // BS 0, IS 1, IIS 2, BD 3 - ([$0004.l,A7],$0044.w)
		{
			S32 v1 = ( mem[ pos + 2 ] << 24 | mem[ pos + 3 ] << 16 | mem[ pos + 4 ] << 8 | mem[ pos + 5 ] );
			S16 v2 = ( mem[ pos + 6 ] <<  8 | mem[ pos + 7 ] );

			sprintf( outstr, "([%d.l,%s],%d.w)", 
				v1, 
				Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ],
				v2 
			);

			rt->rt_CPU.M68k.mt_ArgSize += 8;
			break;
		}

		case 0x0131: // BS 0, IS 1, IIS 3, BD 1 - ([A7],$0044.l)
		{
			S32 v1 = ( mem[ pos + 2 ] << 24 | mem[ pos + 3 ] << 16 | mem[ pos + 4 ] << 8 | mem[ pos + 5 ] );

			sprintf( outstr, "([%s],%d.l)", 
				Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ],
				v1
			);

			rt->rt_CPU.M68k.mt_ArgSize += 6;
			break;
		}

		case 0x0132: // BS 0, IS 1, IIS 3, BD 2 - ([$0004.w,A7],$0044.l)
		{
			S16 v1 = ( mem[ pos + 2 ] <<  8 | mem[ pos + 3 ] );
			S32 v2 = ( mem[ pos + 4 ] << 24 | mem[ pos + 5 ] << 16 | mem[ pos + 6 ] << 8 | mem[ pos + 7 ] );

			sprintf( outstr, "([%d.w,%s],%d.l)", 
				v1, 
				Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ],
				v2 
			);

			rt->rt_CPU.M68k.mt_ArgSize += 8;
			break;
		}

		case 0x0133: // BS 0, IS 1, IIS 3, BD 3 - ([$0004.l,A7],$0044.l)
		{
			S32 v1 = ( mem[ pos + 2 ] << 24 | mem[ pos + 3 ] << 16 | mem[ pos + 4 ] << 8 | mem[ pos + 5 ] );
			S32 v2 = ( mem[ pos + 6 ] << 24 | mem[ pos + 7 ] << 16 | mem[ pos + 8 ] << 8 | mem[ pos + 9 ] );

			sprintf( outstr, "([%d.l,%s],%d.l)", 
				v1, 
				Ax_RegNames[ rt->rt_CPU.M68k.mt_ArgEReg ],
				v2 
			);

			rt->rt_CPU.M68k.mt_ArgSize += 10;
			break;
		}

		default:
		{
			#if 0
			printf( "\n" );
			printf( "%s:%04d: EA ........ : 60\n", __FILE__, __LINE__ );
			printf( "%s:%04d: MemoryAdr . : %08x\n", __FILE__, __LINE__,  rt->rt_CurMemAdr );
			printf( "%s:%04d: AD ........ : %s\n", __FILE__, __LINE__, (AD)?"Ax":"Dx" );
			printf( "%s:%04d: REG ....... : %d : %s\n", __FILE__, __LINE__, REG,( AD ) ? Ax_RegNames[ REG ] : Dx_RegNames[ REG ] );
			printf( "%s:%04d: WL ........ : %s\n", __FILE__, __LINE__, (WL)?".l":".w" );
			printf( "%s:%04d: SCALE ..... : %d : %s\n", __FILE__, __LINE__, SCALE, scale_Names[SCALE] );
			printf( "%s:%04d: BS ........ : %s\n", __FILE__, __LINE__, (BS)?"Yes":"No" );
			printf( "%s:%04d: IS ........ : %s\n", __FILE__, __LINE__, (IS)?"Yes":"No" );
			printf( "%s:%04d: BD ........ : %d\n", __FILE__, __LINE__, BD );
			printf( "%s:%04d: IIS ....... : %d\n", __FILE__, __LINE__, IIS );
			printf( "%s:%04d: Mode ...... : 0x%04x\n", __FILE__, __LINE__, mode );
			#endif

			ec = RS4ErrStat_Okay;
			ds = RS4DecodeStat_UnknownCmd;
			goto bailout;
		}
	}

	// --

	ec = RS4ErrStat_Okay;
	ds = RS4DecodeStat_Okay;

	// --

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( ds );
}

// --
