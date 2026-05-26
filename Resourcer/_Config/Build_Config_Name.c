
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

S32
RS4BuildConfigName ( enum RS4ErrorCode * errcode )
{
	enum RS4ErrorCode ec;
	enum RS4FuncStat  fs;
	S32				  err;
	S32				  stat;
	S32				  len;

	ec = RS4ErrStat_Error;

	err = TRUE;

	// -- Build Config Filename

	if ( ! ConfigFile )
	{
		len = strlen ( InputFile );

		if ( ( ( len > 4 ) && ( ! strncasecmp ( &InputFile[len - 4], ".exe", 4 ) ) )
			 || ( ( len > 4 ) && ( ! strncasecmp ( &InputFile[len - 4], ".bin", 4 ) ) ) )
		{
			ERR_CHK ( RS4Strdup ( &ConfigFile, InputFile ) )

			if ( ! ConfigFile )
			{
				ec = RS4ErrStat_OutOfMemory;

#ifdef DEBUG
				printf ( "%s:%04d: Error allocating memory (%d)\n", __FILE__, __LINE__, ec );
#endif

				goto bailout;
			}

			ConfigFile[len - 3] = 'c';
			ConfigFile[len - 2] = 'f';
			ConfigFile[len - 1] = 'g';
		}
		else
		{
			stat = asprintf ( &ConfigFile, "%s.cfg", InputFile );

			if ( ( stat < 0 ) || ( ! ConfigFile ) )
			{
				ec = RS4ErrStat_OutOfMemory;

#ifdef DEBUG
				printf ( "%s:%04d: Error allocating memory (%d)\n", __FILE__, __LINE__, ec );
#endif

				goto bailout;
			}
		}
	}

	err = FALSE;

	ec = RS4ErrStat_Okay;

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	return ( err );
}

// --
