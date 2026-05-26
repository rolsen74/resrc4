
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

#include "ReSrc4.h"

// --
// CRC-16-ANSI (IBM) polynomial

U16
Calc_CRC16 ( U8 * data, U64 len )
{
	U16 crc = 0xffff;

	while ( len-- )
	{
		crc ^= ( *data++ << 8 );

		for ( int cnt = 0; cnt < 8; cnt++ )
		{
			if ( crc & 0x8000 )
			{
				crc = crc << 1;
				crc = crc ^ 0x8005;
			}
			else
			{
				crc = crc << 1;
			}
		}
	}

	return ( crc );
}

// --
