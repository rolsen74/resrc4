
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

#define DDEBUG(x)

// --

enum RS4ErrorCode Misc_Set__Src_is_Exec( 
	struct M68kRegister *dst_reg, 
	RS4Trace *rt,
	MEM mem );

enum RS4ErrorCode Misc_Set__Src_is_Label( 
	struct AmigaOS3_Misc_Move_GetSetStruct *gss,
	struct M68kRegister *dst_reg, 
	RS4Trace *rt,
	MEM mem );

enum RS4ErrorCode Misc_Set__Src_is_Reg( 
	struct AmigaOS3_Misc_Move_GetSetStruct *gss,
	struct M68kRegister *dst_reg, 
	RS4Trace *rt,
	MEM mem );

enum RS4ErrorCode Misc_Set__Src_is_Unknown( 
	struct AmigaOS3_Misc_Move_GetSetStruct *gss,
	struct M68kRegister *dst_reg, 
	RS4Trace *rt,
	MEM mem );

// --

enum RS4FuncStat AmigaOS3_Misc_Move_Set( 
	enum RS4ErrorCode *errcode, 
	RS4Trace *rt, 
	struct M68kRegister *cur, 
	MEM mem, 
	struct AmigaOS3_Misc_Move_GetSetStruct *gss )
{
enum RS4ErrorCode ec;
enum RS4FuncStat fs;

	// --

	DDEBUG( printf( "AmigaOS3_Misc_Move_Set\n" ); )

	ec = RS4ErrStat_Error;
	fs = RS4FuncStat_Error;

	switch( gss->SrcType )
	{
		case _Unset:
		{
			DDEBUG( printf( "AmigaOS3_Misc_Move_Set : _Unset :\n" ); )

			#ifdef DEBUG
			printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
			#endif

			goto bailout;
		}

		default:
		{
			DDEBUG( printf( "AmigaOS3_Misc_Move_Set : Default :\n" ); )

			#ifdef DEBUG
			printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
			#endif

			goto bailout;
		}

		case _Unknown:
		{
			DDEBUG( printf( "AmigaOS3_Misc_Move_Set : _Unknown :\n" ); )

			ec = Misc_Set__Src_is_Unknown( gss, cur, rt, mem );

			#ifdef DEBUG
			if ( ec != RS4ErrStat_Okay )
			{
				printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
				goto bailout;
			}
			#endif

			break;
		}

		case _Reg:
		{
			DDEBUG( printf( "AmigaOS3_Misc_Move_Set : _Reg :\n" ); )

			ec = Misc_Set__Src_is_Reg( gss, cur, rt, mem );

			#ifdef DEBUG
			if ( ec != RS4ErrStat_Okay )
			{
				printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
				goto bailout;
			}
			#endif

			break;
		}

		case _Exec:
		{
			DDEBUG( printf( "AmigaOS3_Misc_Move_Set : _Exec :\n" ); )

			ec = Misc_Set__Src_is_Exec( cur, rt, mem );

			#ifdef DEBUG
			if ( ec != RS4ErrStat_Okay )
			{
				printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
				goto bailout;
			}
			#endif

			break;
		}

		case _Label:
		{
			DDEBUG( printf( "AmigaOS3_Misc_Move_Set : _Label :\n" ); )

			ec = Misc_Set__Src_is_Label( gss, cur, rt, mem );

			#ifdef DEBUG
			if ( ec != RS4ErrStat_Okay )
			{
				printf( "%s:%04d: Error\n", __FILE__, __LINE__ );
				goto bailout;
			}
			#endif

			break;
		}
	}

	if ( ec != RS4ErrStat_Okay )
	{
		goto bailout;
	}

	fs = RS4FuncStat_Okay;

	// --

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return( fs );
}

// --
