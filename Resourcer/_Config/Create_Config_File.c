
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

static CHR CONFIG_BODY_FMT[] = "\n" // Header Newline
							   "\n"
							   ";\n"
							   "; Amiga Hunk file MD5 check sum is a mandatory entry\n"
							   ";\n"
							   "; MD5: $xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"
							   ";\n"
							   "\n"
							   "MD5: $%s\n"
							   "\n"
							   "\n"
							   ";\n"
							   "; MemWrite writes to memory before start of Tracing\n"
							   ";\n"
							   "; MemWrite: $12345678 $xx ..\n"
							   ";\n"
							   "\n"
							   "\n"
							   ";\n"
							   "; The Code command starts a Trace at the Address given.\n"
							   ";\n"
							   "; Code: $12345678\n"
							   ";\n"
							   "\n"
							   "\n"
							   ";\n"
							   "; Relative16 is normal a JumpTable used by C Compilers\n"
							   ";\n"
							   ";             TableAddress RelativeAddress Entries\n"
							   "; Relative16: $12345678    $12345678       x\n"
							   ";\n"
							   "\n"
							   "\n"
							   ";\n"
							   "; LabelName creates a label at address with given name.\n"
							   ";\n"
							   "; The name can be set as \"\", and the resourcer will give\n"
							   "; it a general name.\n"
							   ";\n"
							   "; LabelName: $12345678 \"Name\"\n"
							   ";\n"
							   "\n"
							   "\n";

// --

S32
RS4CreateConfigFile ( RS4FileHeader * fh, enum RS4ErrorCode * errcode )
{
	enum RS4ErrorCode ec;
	CHR				  buf[1024];
	FILE *			  file;
	S32				  total;
	S32				  fsize;
	S32				  len;
	S32				  err;

	ec = RS4ErrStat_Error;

	err = TRUE;

	file = NULL;

	total = 0;

	// --

	file = fopen ( ConfigFile, "wb" );

	if ( ! file )
	{
		/**/ if ( errno == ENOENT )
		{
			ec = RS4ErrStat_FileNotFound;
		}
		else if ( errno == EISDIR )
		{
			ec = RS4ErrStat_FileIsADir;
		}
		else
		{
			ec = RS4ErrStat_OpeningFile;
		}
		goto bailout;
	}

	// -- Write Header

	len	  = strlen ( CONFIG_HEADER );
	fsize = fwrite ( CONFIG_HEADER, 1, len, file );

	if ( fsize != len )
	{
		ec = RS4ErrStat_FileIO;

#ifdef DEBUG
		printf ( "Error writing to file '%s' (%d)\n", ConfigFile, ec );
#endif

		goto bailout;
	}

	total += fsize;

	// -- Body Header

	snprintf ( buf, 1024, CONFIG_BODY_FMT, fh->rfh_FileMD5 );

	len	  = strlen ( buf );
	fsize = fwrite ( buf, 1, len, file );

	if ( fsize != len )
	{
		printf ( "Error writting data\n" );
		goto bailout;
	}

	total += fsize;

	// --

	printf ( "Created '%s' config file (%d bytes)\n", ConfigFile, total );

	// --

	err = FALSE;

	ec = RS4ErrStat_Okay;

bailout:

	if ( file )
	{
		fclose ( file );
	}

	if ( ( file ) && ( err ) )
	{
		remove ( ConfigFile );
	}

	if ( errcode )
	{
		*errcode = ec;
	}

	return ( err );
}

// --
