
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

enum RS4FuncStat
RS4ParseConfig_File ( enum RS4ErrorCode * errcode, RS4FileHeader * exefile )
{
	enum RS4ErrorCode ec;
	enum RS4FuncStat  fs;
	RS4FileHeader *	  cfgfile;
	CHR				  response[10];
	S32				  create;
	S32				  loaded;

	fs = RS4FuncStat_Error;
	ec = RS4ErrStat_Error;

	loaded = FALSE;

	cfgfile = NULL;

	create = FALSE;

	// --
	// -- Build ConfigFile name, if not set

	if ( RS4BuildConfigName ( &ec ) )
	{
		// ec already set

#ifdef DEBUG
		printf ( "%s:%04d: Error building name (%d)\n", __FILE__, __LINE__, ec );
#endif

		goto bailout;
	}

	// --
	// -- Load config file

	ERR_CHK ( RS4LoadFile ( &ec, &cfgfile, ConfigFile ) )

	if ( ( ! cfgfile ) && ( ec == RS4ErrStat_FileNotFound ) )
	{
		/**/ if ( AutoAnser == ANSER_Ask )
		{
			printf ( "The config file '%s' does not exist.\n", ConfigFile );
			printf ( "Do you want to create it? (Yes/[NO]): " );

			if ( ! fgets ( response, sizeof ( response ), stdin ) )
			{
				printf ( "Error reading user input\n" );
				ec = RS4ErrStat_FileIO;
				goto bailout;
			}

			create = ( ( response[0] != 'Y' ) && ( response[0] != 'y' ) ) ? FALSE : TRUE;
		}
		else if ( AutoAnser == ANSER_No )
		{
			create = FALSE;
		}
		else if ( AutoAnser == ANSER_Yes )
		{
			create = TRUE;
		}
		else
		{
			printf ( "%s:%04d: Internal : Error :\n", __FILE__, __LINE__ );
			ec = RS4ErrStat_Internal;
			goto bailout;
		}

		if ( ! create )
		{
			// Not an error
			fs = RS4FuncStat_Okay;
			ec = RS4ErrStat_Okay;
			goto bailout;
		}

		// Create an Empty Config File
		if ( RS4CreateConfigFile ( exefile, &ec ) )
		{
			// ec already set

#ifdef DEBUG
			printf ( "%s:%04d: Error creating config file '%s' (%d)\n", __FILE__, __LINE__, ConfigFile, ec );
#endif

			goto bailout;
		}

		// -- Now load Config file
		ERR_CHK ( RS4LoadFile ( &ec, &cfgfile, ConfigFile ) )
	}
	else
	{
		loaded = TRUE;
	}

	if ( ! cfgfile )
	{
		// ec already set

#ifdef DEBUG
		printf ( "%s:%04d: Error loading file '%s' (%d)\n", __FILE__, __LINE__, ConfigFile, ec );
#endif

		goto bailout;
	}

	if ( ( loaded ) && ( DoVerbose > 0 ) )
	{
		printf ( "Loaded '%s' (%" PRId64 " bytes)\n", ConfigFile, cfgfile->rfh_FileSize );
	}

	// -- Parse Config File

	if ( RS4ParseFile ( cfgfile, exefile, &ec ) )
	{
		// ec already set

#ifdef DEBUG
		printf ( "%s:%04d: Error parsing file '%s' (%d)\n", __FILE__, __LINE__, ConfigFile, ec );
#endif

		goto bailout;
	}

	// --

	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

bailout:

	if ( cfgfile )
	{
		RS4FreeFile ( NULL, cfgfile );
	}

	if ( errcode )
	{
		*errcode = ec;
	}

	return ( fs );
}

// --
