
/*
** Copyright (c) 2014-2026 Rene W. Olsen
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

struct EAMode
{
	U16 Mode;
	STR Format;
};

// %a = Ax Reg
// %d = Dx Reg
// %x = Xn (Ax/Dx) Reg
// %2 = 2 Bytes / 16Bit
// %4 = 4 Bytes / 32Bit
static struct EAMode Modes[] = {
	{ 0x0111, "( [ %a     , %x.w   ]            )" }, //	Pea		([A0,D0.w])					; 4870 0111
	{ 0x0112, "( [ %a     , %x.w   ] , %2.w     )" }, //	Pea		([A0,D0.w],4.w)				; 4870 0112 0004
	{ 0x0113, "( [ %a     , %x.w   ] , %4.l     )" }, //	Pea		([A0,D0.w],8.l)				; 4870 0113 0000 0008
	{ 0x0115, "( [ %a   ] , %x.w                )" }, //	Pea		([A0],D0.w)					; 4870 0115
	{ 0x0116, "( [ %a   ] , %x.w     , %2.w     )" }, //	Pea		([A0],D0.w,4.w)				; 4870 0116 0004
	{ 0x0117, "( [ %a   ] , %x.w     , %4.l     )" }, //	Pea		([A0],D0.w,8.l)				; 4870 0117 0000 0008
	{ 0x0120, "(   %2.w   , %a       , %x.w     )" }, //	Pea		(4.w,A0,D0.w)				; 4870 0120 0004
	{ 0x0121, "( [ %2.w   , %a       , %x.w   ] )" }, //	Pea		([4.w,A0,D0.w])				; 4870 0121 0004
	{ 0x0125, "( [ %2.w   , %a     ] , %x.w     )" }, //	Pea		([4.w,A0],D0.w)				; 4870 0125 0004
	{ 0x0130, "(   %4.l   , %a       , %x.w     )" }, //	Pea		(8.l,A0,D0.w)				; 4870 0130 0000 0008
	{ 0x0131, "( [ %4.l   , %a       , %x.w   ] )" }, //	Pea		([8.l,A0,D0.w])				; 4870 0131 0000 0008
	{ 0x0135, "( [ %4.l   , %a     ] , %x.w     )" }, //	Pea		([8.l,A0],D0.w)				; 4870 0135 0000 0008
	{ 0x0151, "( [ %a   ]                       )" }, //	Pea		([A0])						; 4870 0151
	{ 0x0152, "( [ %a   ] , %2.w                )" }, //	Pea		([A0],4.w)					; 4870 0152 0004
	{ 0x0153, "( [ %a   ] , %4.l                )" }, //	Pea		([A0],8.l)					; 4870 0153 0000 0008
	{ 0x0161, "( [ %2.w   , %a     ]            )" }, //	Pea		([4.w,A0])					; 4870 0161 0004
	{ 0x0170, "(   %4.l   , %a                  )" }, //	Pea		(8.l,A0)					; 4870 0170 0000 0008
	{ 0x0171, "( [ %4.l   , %a     ]            )" }, //	Pea		([8.l,A0])					; 4870 0171 0000 0008
	{ 0x01a0, "(   %2.w   , %d.w                )" }, //	Pea		(4.w,D0.w)					; 4870 01a0 0004
	{ 0x01a2, "( [ %2.w   , %d.w   ] , %2.w     )" }, //	Pea		([4.w,D0.w],4.w)			; 4870 01a2 0004 0004
	{ 0x01a3, "( [ %2.w   , %d.w   ] , %4.l     )" }, //	Pea		([4.w,D0.w],8.l)			; 4870 01a3 0004 0000 0008
	{ 0x01b0, "(   %4.l   , %d.w                )" }, //	Pea		(8.l,D0.w)					; 4870 01b0 0000 0008
	{ 0x01b2, "( [ %4.l   , %d.w   ] , %2.w     )" }, //	Pea		([8.l,D0.w],4.w)			; 4870 01b2 0000 0008 0004
	{ 0x01b3, "( [ %4.l   , %d.w   ] , %4.l     )" }, //	Pea		([8.l,D0.w],8.l)			; 4870 01b3 0000 0008 0000 0008
	{ 0x01e1, "( [ %2.w ]                       )" }, //	Pea		([4.w])						; 4870 01e1 0004
	{ 0x01e2, "( [ %2.w ] , %2.w                )" }, //	Pea		([4.w],4.w)					; 4870 01e2 0004 0004
	{ 0x01e3, "( [ %2.w ] , %4.l                )" }, //	Pea		([4.w],8.l)					; 4870 01e3 0004 0000 0008
	{ 0x01f1, "( [ %4.l ]                       )" }, //	Pea		([8.l])						; 4870 01f1 0000 0008
	{ 0x01f2, "( [ %4.l ] , %2.w                )" }, //	Pea		([8.l],4.w)					; 4870 01f2 0000 0008 0004
	{ 0x01f3, "( [ %4.l ] , %4.l                )" }, //	Pea		([8.l],8.l)					; 4870 01f3 0000 0008 0000 0008
	{ 0x0311, "( [ %a     , %x.w*2 ]            )" }, //	Pea		([A0,D0.w*2])				; 4870 0311
	{ 0x0312, "( [ %a     , %x.w*2 ] , %2.w     )" }, //	Pea		([A0,D0.w*2],4.w)			; 4870 0312 0004
	{ 0x0313, "( [ %a     , %x.w*2 ] , %4.l     )" }, //	Pea		([A0,D0.w*2],8.l)			; 4870 0313 0000 0008
	{ 0x0315, "( [ %a   ] , %x.w*2              )" }, //	Pea		([A0],D0.w*2)				; 4870 0315
	{ 0x0316, "( [ %a   ] , %x.w*2   , %2.w     )" }, //	Pea		([A0],D0.w*2,4.w)			; 4870 0316 0004
	{ 0x0317, "( [ %a   ] , %x.w*2   , %4.l     )" }, //	Pea		([A0],D0.w*2,8.l)			; 4870 0317 0000 0008
	{ 0x0320, "(   %2.w   , %a       , %x.w*2   )" }, //	Pea		(4.w,A0,D0.w*2)				; 4870 0320 0004
	{ 0x0321, "( [ %2.w   , %a       , %x.w*2 ] )" }, //	Pea		([4.w,A0,D0.w*2])			; 4870 0321 0004
	{ 0x0325, "( [ %2.w   , %a     ] , %x.w*2   )" }, //	Pea		([4.w,A0],D0.w*2)			; 4870 0325 0004
	{ 0x0330, "(   %4.l   , %a       , %x.w*2   )" }, //	Pea		(8.l,A0,D0.w*2)				; 4870 0330 0000 0008
	{ 0x0331, "( [ %4.l   , %a       , %x.w*2 ] )" }, //	Pea		([8.l,A0,D0.w*2])			; 4870 0331 0000 0008
	{ 0x0335, "( [ %4.l   , %a     ] , %x.w*2   )" }, //	Pea		([8.l,A0],D0.w*2)			; 4870 0335 0000 0008
	{ 0x03a0, "(   %2.w   , %x.w*2              )" }, //	Pea		(4.w,D0.w*2)				; 4870 03a0 0004
	{ 0x03a2, "( [ %2.w   , %x.w*2 ] , %2.w     )" }, //	Pea		([4.w,D0.w*2],4.w)			; 4870 03a2 0004 0004
	{ 0x03a3, "( [ %2.w   , %x.w*2 ] , %4.l     )" }, //	Pea		([4.w,D0.w*2],8.l)			; 4870 03a3 0004 0000 0008
	{ 0x03b0, "(   %4.l   , %x.w*2              )" }, //	Pea		(8.l,D0.w*2)				; 4870 03b0 0000 0008
	{ 0x03b2, "( [ %4.l   , %x.w*2 ] , %2.w     )" }, //	Pea		([8.l,D0.w*2],4.w)			; 4870 03b2 0000 0008 0004
	{ 0x03b3, "( [ %4.l   , %x.w*2 ] , %4.l     )" }, //	Pea		([8.l,D0.w*2],8.l)			; 4870 03b3 0000 0008 0000 0008
	{ 0x0511, "( [ %a     , %x.w*4 ]            )" }, //	Pea		([A0,D0.w*4])				; 4870 0511
	{ 0x0512, "( [ %a     , %x.w*4 ] , %2.w     )" }, //	Pea		([A0,D0.w*4],4.w)			; 4870 0512 0004
	{ 0x0513, "( [ %a     , %x.w*4 ] , %4.l     )" }, //	Pea		([A0,D0.w*4],8.l)			; 4870 0513 0000 0008
	{ 0x0515, "( [ %a   ] , %x.w*4              )" }, //	Pea		([A0],D0.w*4)				; 4870 0515
	{ 0x0516, "( [ %a   ] , %x.w*4   , %2.w     )" }, //	Pea		([A0],D0.w*4,4.w)			; 4870 0516 0004
	{ 0x0517, "( [ %a   ] , %x.w*4   , %4.l     )" }, //	Pea		([A0],D0.w*4,8.l)			; 4870 0517 0000 0008
	{ 0x0520, "(   %2.w   , %a       , %x.w*4   )" }, //	Pea		(4.w,A0,D0.w*4)				; 4870 0520 0004
	{ 0x0521, "( [ %2.w   , %a       , %x.w*4 ] )" }, //	Pea		([4.w,A0,D0.w*4])			; 4870 0521 0004
	{ 0x0525, "( [ %2.w   , %a     ] , %x.w*4   )" }, //	Pea		([4.w,A0],D0.w*4)			; 4870 0525 0004
	{ 0x0530, "(   %4.l   , %a       , %x.w*4   )" }, //	Pea		(8.l,A0,D0.w*4)				; 4870 0530 0000 0008
	{ 0x0531, "( [ %4.l   , %a       , %x.w*4 ] )" }, //	Pea		([8.l,A0,D0.w*4])			; 4870 0531 0000 0008
	{ 0x0535, "( [ %4.l   , %a     ] , %x.w*4   )" }, //	Pea		([8.l,A0],D0.w*4)			; 4870 0535 0000 0008
	{ 0x05a0, "(   %2.w   , %x.w*4              )" }, //	Pea		(4.w,D0.w*4)				; 4870 05a0 0004
	{ 0x05a2, "( [ %2.w   , %x.w*4 ] , %2.w     )" }, //	Pea		([4.w,D0.w*4],4.w)			; 4870 05a2 0004 0004
	{ 0x05a3, "( [ %2.w   , %x.w*4 ] , %4.l     )" }, //	Pea		([4.w,D0.w*4],8.l)			; 4870 05a3 0004 0000 0008
	{ 0x05b0, "(   %4.l   , %x.w*4              )" }, //	Pea		(8.l,D0.w*4)				; 4870 05b0 0000 0008
	{ 0x05b2, "( [ %4.l   , %x.w*4 ] , %2.w     )" }, //	Pea		([8.l,D0.w*4],4.w)			; 4870 05b2 0000 0008 0004
	{ 0x05b3, "( [ %4.l   , %x.w*4 ] , %4.l     )" }, //	Pea		([8.l,D0.w*4],8.l)			; 4870 05b3 0000 0008 0000 0008
	{ 0x0711, "( [ %a     , %x.w*8 ]            )" }, //	Pea		([A0,D0.w*8])				; 4870 0711
	{ 0x0712, "( [ %a     , %x.w*8 ] , %2.w     )" }, //	Pea		([A0,D0.w*8],4.w)			; 4870 0712 0004
	{ 0x0713, "( [ %a     , %x.w*8 ] , %4.l     )" }, //	Pea		([A0,D0.w*8],8.l)			; 4870 0713 0000 0008
	{ 0x0715, "( [ %a   ] , %x.w*8              )" }, //	Pea		([A0],D0.w*8)				; 4870 0715
	{ 0x0716, "( [ %a   ] , %x.w*8   , %2.w     )" }, //	Pea		([A0],D0.w*8,4.w)			; 4870 0716 0004
	{ 0x0717, "( [ %a   ] , %x.w*8   , %4.l     )" }, //	Pea		([A0],D0.w*8,8.l)			; 4870 0717 0000 0008
	{ 0x0720, "(   %2.w   , %a       , %x.w*8   )" }, //	Pea		(4.w,A0,D0.w*8)				; 4870 0720 0004
	{ 0x0721, "( [ %2.w   , %a       , %x.w*8 ] )" }, //	Pea		([4.w,A0,D0.w*8])			; 4870 0721 0004
	{ 0x0725, "( [ %2.w   , %a     ] , %x.w*8   )" }, //	Pea		([4.w,A0],D0.w*8)			; 4870 0725 0004
	{ 0x0730, "(   %4.l   , %a       , %x.w*8   )" }, //	Pea		(8.l,A0,D0.w*8)				; 4870 0730 0000 0008
	{ 0x0731, "( [ %4.l   , %a       , %x.w*8 ] )" }, //	Pea		([8.l,A0,D0.w*8])			; 4870 0731 0000 0008
	{ 0x0735, "( [ %4.l   , %a     ] , %x.w*8   )" }, //	Pea		([8.l,A0],D0.w*8)			; 4870 0735 0000 0008
	{ 0x07a0, "(   %2.w   , %x.w*8              )" }, //	Pea		(4.w,D0.w*8)				; 4870 07a0 0004
	{ 0x07a2, "( [ %2.w   , %x.w*8 ] , %2.w     )" }, //	Pea		([4.w,D0.w*8],4.w)			; 4870 07a2 0004 0004
	{ 0x07a3, "( [ %2.w   , %x.w*8 ] , %4.l     )" }, //	Pea		([4.w,D0.w*8],8.l)			; 4870 07a3 0004 0000 0008
	{ 0x07b0, "(   %4.l   , %x.w*8              )" }, //	Pea		(8.l,D0.w*8)				; 4870 07b0 0000 0008
	{ 0x07b2, "( [ %4.l   , %x.w*8 ] , %2.w     )" }, //	Pea		([8.l,D0.w*8],4.w)			; 4870 07b2 0000 0008 0004
	{ 0x07b3, "( [ %4.l   , %x.w*8 ] , %4.l     )" }, //	Pea		([8.l,D0.w*8],8.l)			; 4870 07b3 0000 0008 0000 0008
	{ 0x0911, "( [ %a     , %d.l   ]            )" }, //	Pea		([A0,D0.l])					; 4870 0911
	{ 0x0912, "( [ %a     , %d.l   ] , %2.w     )" }, //	Pea		([A0,D0.l],4.w)				; 4870 0912 0004
	{ 0x0913, "( [ %a     , %d.l   ] , %4.l     )" }, //	Pea		([A0,D0.l],8.l)				; 4870 0913 0000 0008
	{ 0x0915, "( [ %a   ] , %d.l                )" }, //	Pea		([A0],D0.l)					; 4870 0915
	{ 0x0916, "( [ %a   ] , %d.l     , %2.w     )" }, //	Pea		([A0],D0.l,4.w)				; 4870 0916 0004
	{ 0x0917, "( [ %a   ] , %d.l     , %4.l     )" }, //	Pea		([A0],D0.l,8.l)				; 4870 0917 0000 0008
	{ 0x0920, "(   %2.w   , %a       , %x.l     )" }, //	Pea		(4.w,A0,D0.l)				; 4870 0920 0004
	{ 0x0921, "( [ %2.w   , %a       , %x.l   ] )" }, //	Pea		([4.w,A0,D0.l])				; 4870 0921 0004
	{ 0x0925, "( [ %2.w   , %a     ] , %x.l     )" }, //	Pea		([4.w,A0],D0.l)				; 4870 0925 0004
	{ 0x0930, "(   %4.l   , %a       , %x.l     )" }, //	Pea		(8.l,A0,D0.l)				; 4870 0930 0000 0008
	{ 0x0931, "( [ %4.l   , %a       , %x.l   ] )" }, //	Pea		([8.l,A0,D0.l])				; 4870 0931 0000 0008
	{ 0x0935, "( [ %4.l   , %a     ] , %x.l     )" }, //	Pea		([8.l,A0],D0.l)				; 4870 0935 0000 0008
	{ 0x09a0, "(   %2.w   , %d.l                )" }, //	Pea		(4.w,D0.l)					; 4870 09a0 0004
	{ 0x09a2, "( [ %2.w   , %d.l   ] , %2.w     )" }, //	Pea		([4.w,D0.l],4.w)			; 4870 09a2 0004 0004
	{ 0x09a3, "( [ %2.w   , %d.l   ] , %4.l     )" }, //	Pea		([4.w,D0.l],8.l)			; 4870 09a3 0004 0000 0008
	{ 0x09b0, "(   %4.l   , %d.l                )" }, //	Pea		(8.l,D0.l)					; 4870 09b0 0000 0008
	{ 0x09b2, "( [ %4.l   , %d.l   ] , %2.w     )" }, //	Pea		([8.l,D0.l],4.w)			; 4870 09b2 0000 0008 0004
	{ 0x09b3, "( [ %4.l   , %d.l   ] , %4.l     )" }, //	Pea		([8.l,D0.l],8.l)			; 4870 09b3 0000 0008 0000 0008
	{ 0x0b11, "( [ %a     , %x.l*2 ]            )" }, //	Pea		([A0,D0.l*2])				; 4870 0b11
	{ 0x0b12, "( [ %a     , %x.l*2 ] , %2.w     )" }, //	Pea		([A0,D0.l*2],4.w)			; 4870 0b12 0004
	{ 0x0b13, "( [ %a     , %x.l*2 ] , %4.l     )" }, //	Pea		([A0,D0.l*2],8.l)			; 4870 0b13 0000 0008
	{ 0x0b15, "( [ %a   ] , %x.l*2              )" }, //	Pea		([A0],D0.l*2)				; 4870 0b15
	{ 0x0b16, "( [ %a   ] , %x.l*2   , %2.w     )" }, //	Pea		([A0],D0.l*2,4.w)			; 4870 0b16 0004
	{ 0x0b17, "( [ %a   ] , %x.l*2   , %4.l     )" }, //	Pea		([A0],D0.l*2,8.l)			; 4870 0b17 0000 0008
	{ 0x0b20, "(   %2.w   , %a       , %x.l*2   )" }, //	Pea		(4.w,A0,D0.l*2)				; 4870 0b20 0004
	{ 0x0b21, "( [ %2.w   , %a       , %x.l*2 ] )" }, //	Pea		([4.w,A0,D0.l*2])			; 4870 0b21 0004
	{ 0x0b25, "( [ %2.w   , %a     ] , %x.l*2   )" }, //	Pea		([4.w,A0],D0.l*2)			; 4870 0b25 0004
	{ 0x0b30, "(   %4.l   , %a       , %x.l*2   )" }, //	Pea		(8.l,A0,D0.l*2)				; 4870 0b30 0000 0008
	{ 0x0b31, "( [ %4.l   , %a       , %x.l*2 ] )" }, //	Pea		([8.l,A0,D0.l*2])			; 4870 0b31 0000 0008
	{ 0x0b35, "( [ %4.l   , %a     ] , %x.l*2   )" }, //	Pea		([8.l,A0],D0.l*2)			; 4870 0b35 0000 0008
	{ 0x0ba0, "(   %2.w   , %x.l*2              )" }, //	Pea		(4.w,D0.l*2)				; 4870 0ba0 0004
	{ 0x0ba2, "( [ %2.w   , %x.l*2 ] , %2.w     )" }, //	Pea		([4.w,D0.l*2],4.w)			; 4870 0ba2 0004 0004
	{ 0x0ba3, "( [ %2.w   , %x.l*2 ] , %4.l     )" }, //	Pea		([4.w,D0.l*2],8.l)			; 4870 0ba3 0004 0000 0008
	{ 0x0bb0, "(   %4.l   , %x.l*2              )" }, //	Pea		(8.l,D0.l*2)				; 4870 0bb0 0000 0008
	{ 0x0bb2, "( [ %4.l   , %x.l*2 ] , %2.w     )" }, //	Pea		([8.l,D0.l*2],4.w)			; 4870 0bb2 0000 0008 0004
	{ 0x0bb3, "( [ %4.l   , %x.l*2 ] , %4.l     )" }, //	Pea		([8.l,D0.l*2],8.l)			; 4870 0bb3 0000 0008 0000 0008
	{ 0x0d11, "( [ %a     , %x.l*4 ]            )" }, //	Pea		([A0,D0.l*4])				; 4870 0d11
	{ 0x0d12, "( [ %a     , %x.l*4 ] , %2.w     )" }, //	Pea		([A0,D0.l*4],4.w)			; 4870 0d12 0004
	{ 0x0d13, "( [ %a     , %x.l*4 ] , %4.l     )" }, //	Pea		([A0,D0.l*4],8.l)			; 4870 0d13 0000 0008
	{ 0x0d15, "( [ %a   ] , %x.l*4              )" }, //	Pea		([A0],D0.l*4)				; 4870 0d15
	{ 0x0d16, "( [ %a   ] , %x.l*4   , %2.w     )" }, //	Pea		([A0],D0.l*4,4.w)			; 4870 0d16 0004
	{ 0x0d17, "( [ %a   ] , %x.l*4   , %4.l     )" }, //	Pea		([A0],D0.l*4,8.l)			; 4870 0d17 0000 0008
	{ 0x0d20, "(   %2.w   , %a       , %x.l*4   )" }, //	Pea		(4.w,A0,D0.l*4)				; 4870 0d20 0004
	{ 0x0d21, "( [ %2.w   , %a       , %x.l*4 ] )" }, //	Pea		([4.w,A0,D0.l*4])			; 4870 0d21 0004
	{ 0x0d25, "( [ %2.w   , %a     ] , %x.l*4   )" }, //	Pea		([4.w,A0],D0.l*4)			; 4870 0d25 0004
	{ 0x0d30, "(   %4.l   , %a       , %x.l*4   )" }, //	Pea		(8.l,A0,D0.l*4)				; 4870 0d30 0000 0008
	{ 0x0d31, "( [ %4.l   , %a       , %x.l*4 ] )" }, //	Pea		([8.l,A0,D0.l*4])			; 4870 0d31 0000 0008
	{ 0x0d35, "( [ %4.l   , %a     ] , %x.l*4   )" }, //	Pea		([8.l,A0],D0.l*4)			; 4870 0d35 0000 0008
	{ 0x0da0, "(   %2.w   , %x.l*4              )" }, //	Pea		(4.w,D0.l*4)				; 4870 0da0 0004
	{ 0x0da2, "( [ %2.w   , %x.l*4 ] , %2.w     )" }, //	Pea		([4.w,D0.l*4],4.w)			; 4870 0da2 0004 0004
	{ 0x0da3, "( [ %2.w   , %x.l*4 ] , %4.l     )" }, //	Pea		([4.w,D0.l*4],8.l)			; 4870 0da3 0004 0000 0008
	{ 0x0db0, "(   %4.l   , %x.l*4              )" }, //	Pea		(8.l,D0.l*4)				; 4870 0db0 0000 0008
	{ 0x0db2, "( [ %4.l   , %x.l*4 ] , %2.w     )" }, //	Pea		([8.l,D0.l*4],4.w)			; 4870 0db2 0000 0008 0004
	{ 0x0db3, "( [ %4.l   , %x.l*4 ] , %4.l     )" }, //	Pea		([8.l,D0.l*4],8.l)			; 4870 0db3 0000 0008 0000 0008
	{ 0x0f11, "( [ %a     , %x.l*8 ]            )" }, //	Pea		([A0,D0.l*8])				; 4870 0f11
	{ 0x0f12, "( [ %a     , %x.l*8 ] , %2.w     )" }, //	Pea		([A0,D0.l*8],4.w)			; 4870 0f12 0004
	{ 0x0f13, "( [ %a     , %x.l*8 ] , %4.l     )" }, //	Pea		([A0,D0.l*8],8.l)			; 4870 0f13 0000 0008
	{ 0x0f15, "( [ %a   ] , %x.l*8              )" }, //	Pea		([A0],D0.l*8)				; 4870 0f15
	{ 0x0f16, "( [ %a   ] , %x.l*8   , %2.w     )" }, //	Pea		([A0],D0.l*8,4.w)			; 4870 0f16 0004
	{ 0x0f17, "( [ %a   ] , %x.l*8   , %4.l     )" }, //	Pea		([A0],D0.l*8,8.l)			; 4870 0f17 0000 0008
	{ 0x0f20, "(   %2.w   , %a       , %x.l*8   )" }, //	Pea		(4.w,A0,D0.l*8)				; 4870 0f20 0004
	{ 0x0f21, "( [ %2.w   , %a       , %x.l*8 ] )" }, //	Pea		([4.w,A0,D0.l*8])			; 4870 0f21 0004
	{ 0x0f25, "( [ %2.w   , %a     ] , %x.l*8   )" }, //	Pea		([4.w,A0],D0.l*8)			; 4870 0f25 0004
	{ 0x0f30, "(   %4.l   , %a       , %x.l*8   )" }, //	Pea		(8.l,A0,D0.l*8)				; 4870 0f30 0000 0008
	{ 0x0f31, "( [ %4.l   , %a       , %x.l*8 ] )" }, //	Pea		([8.l,A0,D0.l*8])			; 4870 0f31 0000 0008
	{ 0x0f35, "( [ %4.l   , %a     ] , %x.l*8   )" }, //	Pea		([8.l,A0],D0.l*8)			; 4870 0f35 0000 0008
	{ 0x0fa0, "(   %2.w   , %x.l*8              )" }, //	Pea		(4.w,D0.l*8)				; 4870 0fa0 0004
	{ 0x0fa2, "( [ %2.w   , %x.l*8 ] , %2.w     )" }, //	Pea		([4.w,D0.l*8],4.w)			; 4870 0fa2 0004 0004
	{ 0x0fa3, "( [ %2.w   , %x.l*8 ] , %4.l     )" }, //	Pea		([4.w,D0.l*8],8.l)			; 4870 0fa3 0004 0000 0008
	{ 0x0fb0, "(   %4.l   , %x.l*8              )" }, //	Pea		(8.l,D0.l*8)				; 4870 0fb0 0000 0008
	{ 0x0fb2, "( [ %4.l   , %x.l*8 ] , %2.w     )" }, //	Pea		([8.l,D0.l*8],4.w)			; 4870 0fb2 0000 0008 0004
	{ 0x0fb3, "( [ %4.l   , %x.l*8 ] , %4.l     )" }, //	Pea		([8.l,D0.l*8],8.l)			; 4870 0fb3 0000 0008 0000 0008
	{ 0x0000, NULL }
};

static U8 HexData[] = "0123456789ABCDEF";

// --
// -- Mode 60 - Full Extension Word Format00

enum RS4DecodeStat
MODE_60_Full_0 ( enum RS4ErrorCode * errcode, RS4Trace * rt, STR outstr )
{
	enum RS4DecodeStat ds;
	enum RS4ErrorCode  ec;
	enum RS4FuncStat   fs;

	RS4Label * rl;
	RS4Ref *   isRef;

	CHR labname[MAX_LabelName + 8];
	S32 handled;
	U16 val16;
	U32 val32;
	U16 mode;
	MEM mem;
	S32 cnt;
	S32 REG;
	S32 pos;
	STR fmt;
	S32 AD;
	S32 c;

	// --
	// Full Extension Word Format

	// [A/D] [REG] [W/L] [Scale] [1]  [BS] [IS] [BD SIZE]  [0] [I/IS]
	//   1     3     1      2     1    1    1       2       1    3
	// [  4 bit  ] [     4 bit     ]  [      4 bit      ]  [  4bit  ]
	// --

	cnt	 = 0;
	ec	 = RS4ErrStat_Error;
	ds	 = RS4DecodeStat_Error;
	mem	 = rt->rt_CurMemBuf;
	pos	 = rt->rt_CPU.M68k.mt_ArgSize;
	AD	 = ( mem[pos] & 0x80 ) >> 7;
	REG	 = ( mem[pos] & 0x70 ) >> 4;
	mode = ( ( mem[pos] << 8 ) | ( mem[pos + 1] ) ) & 0x0fff;

	while ( Modes[cnt].Mode )
	{
		if ( Modes[cnt].Mode == mode )
		{
			break;
		}
		else
		{
			cnt++;
		}
	}

	if ( Modes[cnt].Mode == 0x0000 )
	{
		ec = RS4ErrStat_Okay;
		ds = RS4DecodeStat_UnknownCmd;
		printf ( "Invalid EA 60 Mode ($%04" PRIx16 ")\n", mode );
		goto bailout;
	}

	rt->rt_CPU.M68k.mt_ArgSize += 2;

	fmt = Modes[cnt].Format;

	while ( true )
	{
		c = *fmt++;

		if ( ! c )
		{
			break;
		}

		if ( c != '%' )
		{
			if ( ( c != 9 ) && ( c != 32 ) )
			{
				*outstr++ = c;
			}
			continue;
		}

		c = *fmt++;

		/**/ if ( c == 'a' ) // Ax Reg
		{
			*outstr++ = 'A';
			*outstr++ = '0' + rt->rt_CPU.M68k.mt_ArgEReg;
			continue;
		}
		else if ( c == 'd' ) // Dx Reg
		{
			*outstr++ = 'D';
			*outstr++ = '0' + rt->rt_CPU.M68k.mt_ArgEReg;
			continue;
		}
		else if ( c == 'x' ) // Xn Reg
		{
			*outstr++ = ( AD ) ? 'A' : 'D';
			*outstr++ = '0' + REG;
			continue;
		}
		else if ( c == '2' ) // 16bit
		{
			val16 = 0;
			val16 |= mem[rt->rt_CPU.M68k.mt_ArgSize++] << 8;
			val16 |= mem[rt->rt_CPU.M68k.mt_ArgSize++] << 0;

			*outstr++ = '$';
			*outstr++ = HexData[( ( val16 >> 12 ) & 0x0f )];
			*outstr++ = HexData[( ( val16 >> 8 ) & 0x0f )];
			*outstr++ = HexData[( ( val16 >> 4 ) & 0x0f )];
			*outstr++ = HexData[( ( val16 >> 0 ) & 0x0f )];
			continue;
		}
		else if ( c == '4' ) // 32bit
		{
			val32 = 0;
			val32 |= mem[rt->rt_CPU.M68k.mt_ArgSize++] << 24;
			val32 |= mem[rt->rt_CPU.M68k.mt_ArgSize++] << 16;
			val32 |= mem[rt->rt_CPU.M68k.mt_ArgSize++] << 8;
			val32 |= mem[rt->rt_CPU.M68k.mt_ArgSize++] << 0;
			handled = false;

			ERR_CHK ( RS4FindRef_Sec ( &ec, &isRef, rt->rt_Section, rt->rt_CurMemAdr + rt->rt_CPU.M68k.mt_ArgSize - 4 ) )

			if ( isRef )
			{
				isRef->rr_Handled = TRUE;

				// if there is a Ref then the a label have been added
				ERR_CHK ( RS4_Find_LabelAdr ( &ec, rt->rt_File, &rl, val32, __FILE__ ) )

				if ( ! rl )
				{
					ec = RS4ErrStat_Internal;
					ds = RS4DecodeStat_Error;
					goto bailout;
				}

				if ( rt->rt_Pass != RS4TracePass_Trace )
				{
					if ( rl->rl_Name[0] )
					{
						ERR_CHK ( RS4BuildLabelString ( &ec, rl, labname ) )
						sprintf ( outstr, "%s", labname );
						outstr += strlen ( labname );
						handled = true;
					}
				}
			}

			if ( ! handled )
			{
				*outstr++ = '$';
				*outstr++ = HexData[( ( val32 >> 28 ) & 0x0f )];
				*outstr++ = HexData[( ( val32 >> 24 ) & 0x0f )];
				*outstr++ = HexData[( ( val32 >> 20 ) & 0x0f )];
				*outstr++ = HexData[( ( val32 >> 16 ) & 0x0f )];
				*outstr++ = HexData[( ( val32 >> 12 ) & 0x0f )];
				*outstr++ = HexData[( ( val32 >> 8 ) & 0x0f )];
				*outstr++ = HexData[( ( val32 >> 4 ) & 0x0f )];
				*outstr++ = HexData[( ( val32 >> 0 ) & 0x0f )];
			}
			continue;
		}
		else
		{
			goto bailout;
		}
	}

	*outstr = 0;

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

	return ( ds );
}

// --
