
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

static CHR RC4MD5[34];

STR CONFIG_HEADER = "[## ReSrc4 v1 : Configfile ##]";

static const S8 HexData[256] = {
	//	 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 1
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 2
	0,	1,	2,	3,	4,	5,	6,	7,	8,	9,	-1, -1, -1, -1, -1, -1, // 3
	-1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 4
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 5
	-1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 6
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 7
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 8
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 9
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // A
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // B
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // C
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // D
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // E
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // F
};

// --

enum
{
	TYPE_Library,
	TYPE_String,
	TYPE_Struct,
};

// --

struct TypeStruct
{
	S32 NameLen;
	STR Name;
	S32 Type;
	S32 Value;
};

struct TypeStruct myTypes[] = { { 6, "String", TYPE_String, 0 },
#ifdef SUPPORT_AMIGAOS3
								{ 7, "Lib_Asl", TYPE_Library, AOS3_LibType_AslBase },
								{ 7, "Lib_Gfx", TYPE_Library, AOS3_LibType_GraphicsBase },
								{ 7, "Lib_Dos", TYPE_Library, AOS3_LibType_DosBase },
								{ 8, "Lib_Exec", TYPE_Library, AOS3_LibType_ExecBase },
								{ 10, "Lib_Layers", TYPE_Library, AOS3_LibType_LayersBase },
								{ 11, "Lib_Mathffp", TYPE_Library, AOS3_LibType_MathffpBase },
								{ 11, "Lib_Utility", TYPE_Library, AOS3_LibType_UtilityBase },
								{ 12, "Lib_Diskfont", TYPE_Library, AOS3_LibType_DiskfontBase },
								{ 12, "Lib_IFFParse", TYPE_Library, AOS3_LibType_IFFParseBase },
								{ 13, "Lib_Intuition", TYPE_Library, AOS3_LibType_IntuitionBase },
								{ 13, "Lib_Mathtrans", TYPE_Library, AOS3_LibType_MathtransBase },
								{ 13, "Str_IntuiText", TYPE_Struct, RS4StructID_IntuiText },
								{ 13, "Str_NewScreen", TYPE_Struct, RS4StructID_NewScreen },
								{ 10, "Str_BitMap", TYPE_Struct, RS4StructID_BitMap },
								{ 10, "Str_Gadget", TYPE_Struct, RS4StructID_Gadget },
								{ 12, "Str_TextAttr", TYPE_Struct, RS4StructID_TextAttr },
								{ 12, "Str_TextFont", TYPE_Struct, RS4StructID_TextFont },
								{ 12, "Str_RastPort", TYPE_Struct, RS4StructID_RastPort },
								{ 13, "Str_Rectangle", TYPE_Struct, RS4StructID_Rectangle },
								{ 13, "Str_LWGadData", TYPE_Struct, RS4StructID_LWGadData },
#endif
								{ 0, NULL, 0, 0 } };

static enum RS4FuncStat
RS4SetType ( enum RS4ErrorCode * errcode, RS4Label * rl, STR buf, S32 * posptr )
{
	enum RS4ErrorCode ec;
	enum RS4FuncStat  fs;
	S32				  pos;
	S32				  cnt;

	ec	= RS4ErrStat_Error;
	fs	= RS4FuncStat_Error;
	pos = *posptr;

	cnt = 0;

	while ( myTypes[cnt].Name )
	{
		if ( ! strncasecmp ( &buf[pos], myTypes[cnt].Name, myTypes[cnt].NameLen ) )
		{
			break;
		}
		else
		{
			cnt++;
		}
	}

	if ( ! myTypes[cnt].Name )
	{
		ec = RS4ErrStat_InvalidLabelType;
		// fs is set
		goto bailout;
	}

	pos += myTypes[cnt].NameLen;

	switch ( myTypes[cnt].Type )
	{
		case TYPE_Library:
		{
			rl->rl_Type1	  = RS4LabelType_Pointer;
			rl->rl_Type2	  = RS4LabelPtrType_Library;
			rl->rl_Type3	  = myTypes[cnt].Value;
			rl->rl_UserLocked = TRUE;
			break;
		}

		case TYPE_String:
		{
			ERR_CHK ( Mark_NulString ( &ec, rl ) )
			rl->rl_UserLocked = TRUE;
			break;
		}

		case TYPE_Struct:
		{
			ERR_CHK ( Mark_Struct ( &ec, rl, myTypes[cnt].Value, __FILE__ ) )

			rl->rl_UserLocked = TRUE;
			break;
		}

		default:
		{
			printf ( "Setting Label : '%s', At $%08" PRIx64 " to %08x\n", rl->rl_Name, rl->rl_Address, myTypes[cnt].Type );
			break;
		}
	}

	*posptr = pos;

	// --

	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

	// --

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return ( fs );
}

// --

static enum RS4FuncStat
RS4ReadAddress ( enum RS4ErrorCode * errcode, STR buf, S32 * p, S64 * a )
{
	enum RS4ErrorCode ec;
	enum RS4FuncStat  fs;
	S64				  adr;
	S32				  off;
	S32				  pos;
	S32				  hex;
	S32				  cnt;
	S32				  val;

	ec	= RS4ErrStat_Error;
	fs	= RS4FuncStat_Error;
	pos = *p;

	while ( ( buf[pos] == 9 ) || ( buf[pos] == 32 ) )
	{
		pos++;
	}

	if ( buf[pos] != '$' )
	{
		ec = RS4ErrStat_ExpectedHexVal;

#ifdef DEBUG
		printf ( "%s:%04d: Reading address failed (%d)\n", __FILE__, __LINE__, ec );
#endif
		goto bailout;
	}

	pos++;

	// Need atlease one hex digit
	adr = 0;
	off = buf[pos];
	hex = HexData[off];

	if ( hex == -1 )
	{
		ec = RS4ErrStat_NeedOneDigit;

#ifdef DEBUG
		printf ( "%s:%04d: Reading value failed (%d)\n", __FILE__, __LINE__, ec );
#endif
		goto bailout;
	}

	for ( cnt = 0; cnt < 8; cnt++ )
	{
		val = buf[pos];
		hex = HexData[val];

		if ( hex == -1 )
		{
			break;
		}

		adr = adr << 4;
		adr = adr | hex;
		pos++;
	}

	// --

	*a = adr;
	*p = pos;
	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

	// --

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return ( fs );
}

// --

static enum RS4FuncStat
RS4ReadNumber ( enum RS4ErrorCode * errcode, STR buf, S32 * p, S64 * n )
{
	enum RS4ErrorCode ec;
	enum RS4FuncStat  fs;
	S64				  val;
	S32				  pos;
	S32				  c;

	ec	= RS4ErrStat_Error;
	fs	= RS4FuncStat_Error;
	pos = *p;

	while ( ( buf[pos] == 9 ) || ( buf[pos] == 32 ) )
	{
		pos++;
	}

	// Need atlease one digt
	if ( ( buf[pos] < '0' ) || ( buf[pos] >= '9' ) )
	{
		ec = RS4ErrStat_NeedOneDigit;

#ifdef DEBUG
		printf ( "%s:%04d: Error\n", __FILE__, __LINE__ );
#endif

		goto bailout;
	}

	val = 0;

	while ( TRUE )
	{
		c = buf[pos];

		if ( ( c < '0' ) || ( c >= '9' ) )
		{
			break;
		}

		val = ( val * 10 ) + ( c - '0' );
		pos++;
	}

	// --

	*n = val;
	*p = pos;
	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

	// --

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return ( fs );
}

// --

static enum RS4FuncStat
RS4ReadString ( enum RS4ErrorCode * errcode, STR buf, S32 * p, STR str, S32 max )
{
	enum RS4ErrorCode ec;
	enum RS4FuncStat  fs;
	S32				  pos;
	S32				  cnt;
	S32				  c;

	ec	= RS4ErrStat_Error;
	fs	= RS4FuncStat_Error;
	pos = *p;

	while ( ( buf[pos] == 9 ) || ( buf[pos] == 32 ) )
	{
		pos++;
	}

	if ( buf[pos] != '\"' )
	{
		printf ( "Missing String \" Indicator\n" );
		goto bailout;
	}

	pos++;

	cnt = 0;

	while ( TRUE )
	{
		c = buf[pos];

		if ( ( c == 0 ) || ( c == 10 ) || ( c == '\"' ) )
		{
			break;
		}

		pos++;

		if ( cnt >= max )
		{
			printf ( "String to long\n" );
			goto bailout;
		}

		str[cnt++] = c;
	}

	if ( buf[pos] != '\"' )
	{
		printf ( "String \" Terminator\n" );
		goto bailout;
	}

	pos++;

	str[cnt] = 0;

	*p = pos;

	// --

	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

	// --

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return ( fs );
}

// --

static enum RS4FuncStat
RS4Cmd_RELATIVE16 ( RS4FileHeader * cfgfile UNUSED, RS4FileHeader * exefile, enum RS4ErrorCode * errcode, STR buf, S32 linenr )
{
	enum RS4ErrorCode ec;
	enum RS4FuncStat  fs;
	enum RS4JumpStat  js;
	S64				  relative_adr;
	S64				  tabel_adr;
	S64				  entries;
	S32				  pos;

	ec	= RS4ErrStat_Error;
	fs	= RS4FuncStat_Error;
	pos = 11;

	while ( ( buf[pos] == 9 ) || ( buf[pos] == 32 ) )
	{
		pos++;
	}

	ERR_CHK ( RS4ReadAddress ( &ec, buf, &pos, &tabel_adr ) )

	ERR_CHK ( RS4ReadAddress ( &ec, buf, &pos, &relative_adr ) )

	ERR_CHK ( RS4ReadNumber ( &ec, buf, &pos, &entries ) )

	js = RS4JumpTable_Rel16 ( &ec, exefile, tabel_adr, relative_adr, entries );

	if ( js != RS4JumpStat_Handled )
	{
		// ec allready set
		fs = RS4FuncStat_Error;

#ifdef DEBUG
		printf ( "%s:%04d: Error\n", __FILE__, __LINE__ );
#endif

		goto bailout;
	}

	while ( ( buf[pos] == 9 ) || ( buf[pos] == 32 ) )
	{
		pos++;
	}

	if ( ( buf[pos] ) && ( buf[pos] != 10 ) && ( buf[pos] != ';' ) )
	{
		printf ( "Line %d : Relative16 : Error junk at end of line\n", linenr );
		goto bailout;
	}

	// --

	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

	// --

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return ( fs );
}

// --

static enum RS4FuncStat
RS4Cmd_LABELNAME ( RS4FileHeader * cfgfile UNUSED, RS4FileHeader * exefile, enum RS4ErrorCode * errcode, STR buf, S32 linenr )
{
	enum RS4ErrorCode ec;
	enum RS4FuncStat  fs;
	RS4Label *		  TmpLabel;
	RS4Label *		  rl;
	char			  labname[MAX_LabelName];
	S64				  adr;
	S32				  pos;

	// printf( "Label Name\n" );

	ec	= RS4ErrStat_Error;
	fs	= RS4FuncStat_Error;
	pos = 10;

	while ( ( buf[pos] == 9 ) || ( buf[pos] == 32 ) )
	{
		pos++;
	}

	ERR_CHK ( RS4ReadAddress ( &ec, buf, &pos, &adr ) )

	ERR_CHK ( RS4AddLabel_File ( &ec, &rl, exefile, adr, RS4LabelType_Unset, __FILE__ ) )

	if ( ! rl )
	{
		// ec allready set
		fs = RS4FuncStat_Error;
		printf ( "Line %d : LabelName : Error adding label at $%08" PRIx64 "\n", linenr, adr );
		goto bailout;
	}

	labname[0] = 0;
	ERR_CHK ( RS4ReadString ( &ec, buf, &pos, labname, MAX_LabelName ) )

	while ( ( buf[pos] == 9 ) || ( buf[pos] == 32 ) )
	{
		pos++;
	}

	if ( labname[0] )
	{
		// --
		// Make sure the name is Unique

		ERR_CHK ( RS4_Find_LabelName ( &ec, exefile, &TmpLabel, labname ) )

		if ( TmpLabel )
		{
			// --
			// Debug Symbols may have set this allready

			if ( TmpLabel != rl )
			{
				printf ( "Line %" PRId32 " : LabelName : Error label name in use\n", linenr );
				goto bailout;
			}
		}
		else
		{
			// --
			// Add Name

			ERR_CHK ( RS4_Add_LabelName ( &ec, exefile, rl, labname ) )
		}
	}

	// --
	// Check for possible Type setting

	if ( buf[pos] == ',' )
	{
		S32 p = pos;

		p++;

		while ( ( buf[p] == 9 ) || ( buf[p] == 32 ) )
		{
			p++;
		}

		if ( ! strncasecmp ( "type:", &buf[p], 5 ) )
		{
			p += 5;

			while ( ( buf[p] == 9 ) || ( buf[p] == 32 ) )
			{
				p++;
			}

			ERR_CHK ( RS4SetType ( &ec, rl, buf, &p ) )

			while ( ( buf[p] == 9 ) || ( buf[p] == 32 ) )
			{
				p++;
			}
		}

		pos = p;
	}

	// Check no garbage at end

	if ( ( buf[pos] ) && ( buf[pos] != 10 ) && ( buf[pos] != ';' ) )
	{
		printf ( "Line %" PRId32 " : LabelName : Error junk at end of line\n", linenr );
		goto bailout;
	}

	// --

	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

	// --

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return ( fs );
}

// --

static enum RS4FuncStat
RS4Cmd_LABELTYPE ( RS4FileHeader * cfgfile UNUSED, RS4FileHeader * exefile, enum RS4ErrorCode * errcode, STR buf, S32 linenr )
{
	enum RS4ErrorCode ec;
	enum RS4FuncStat  fs;
	RS4Label *		  rl;
	S64				  adr;
	S32				  pos;

	ec	= RS4ErrStat_Error;
	fs	= RS4FuncStat_Error;
	pos = 10;

	while ( ( buf[pos] == 9 ) || ( buf[pos] == 32 ) )
	{
		pos++;
	}

	ERR_CHK ( RS4ReadAddress ( &ec, buf, &pos, &adr ) )

	ERR_CHK ( RS4AddLabel_File ( &ec, &rl, exefile, adr, RS4LabelType_Unset, __FILE__ ) )

	if ( ! rl )
	{
		// ec allready set
		fs = RS4FuncStat_Error;
		printf ( "Line %d : LabelType : Error adding label at $%08" PRIx64 "\n", linenr, adr );
		goto bailout;
	}

	while ( ( buf[pos] == 9 ) || ( buf[pos] == 32 ) )
	{
		pos++;
	}

	if ( ( buf[pos] == 0 ) || ( buf[pos] == 10 ) || ( buf[pos] == 13 ) || ( buf[pos] == ';' ) )
	{
		printf ( "Line %d : LabelType : Error Missing Command at $%08" PRIx64 "\n", linenr, adr );
		goto bailout;
	}

	/**/ if ( ! strncasecmp ( &buf[pos], "Pointer", 7 ) )
	{
		pos += 7;

		while ( ( buf[pos] == 9 ) || ( buf[pos] == 32 ) )
		{
			pos++;
		}

		if ( ( buf[pos] == 0 ) || ( buf[pos] == 10 ) || ( buf[pos] == ';' ) )
		{
			printf ( "Line %d : LabelType : Error Missing Command Argument at $%08" PRIx64 "\n", linenr, adr );
			goto bailout;
		}

		ERR_CHK ( RS4AddLabel_File ( &ec, &rl, exefile, adr, RS4LabelType_Pointer, __FILE__ ) )

		if ( ! rl )
		{
			// ec allready set
			fs = RS4FuncStat_Error;
			printf ( "Line %d : LabelType : Error adding label at $%08" PRIx64 "\n", linenr, adr );
			goto bailout;
		}

		ERR_CHK ( RS4SetType ( &ec, rl, buf, &pos ) )
	}
	else
	{
		printf ( "Line %d : LabelType : Error Unsupported Command at $%08" PRIx64 "\n", linenr, adr );
		goto bailout;
	}

	while ( ( buf[pos] == 9 ) || ( buf[pos] == 32 ) )
	{
		pos++;
	}

	if ( ( buf[pos] ) && ( buf[pos] != 10 ) && ( buf[pos] != ';' ) )
	{
		printf ( "Line %d : LabelType : Error junk at end of line\n", linenr );
		goto bailout;
	}

	// --

	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

	// --

bailout:

	// --

	if ( errcode )
	{
		*errcode = ec;
	}

	return ( fs );
}

// --

static enum RS4FuncStat
RS4Cmd_MD5 ( RS4FileHeader * cfgfile UNUSED, RS4FileHeader * exefile UNUSED, enum RS4ErrorCode * errcode, STR buf, S32 linenr )
{
	enum RS4ErrorCode ec;
	enum RS4FuncStat  fs;
	S32				  stat;
	S32				  pos;
	S32				  cnt;
	S32				  p;

	fs	= RS4FuncStat_Error;
	ec	= RS4ErrStat_Error;
	pos = 4;

	// skip cmd

	while ( ( buf[pos] == 9 ) || ( buf[pos] == 32 ) )
	{
		pos++;
	}

	if ( buf[pos] != '$' )
	{
		printf ( "Line %d : MD5 : Address Argument must start with a $\n", linenr );
		goto bailout;
	}

	pos++;

	p = pos;

	for ( cnt = 0; cnt < 32; cnt++ )
	{
		stat = ( ( ( buf[p] >= '0' ) && ( buf[p] <= '9' ) ) || ( ( buf[p] >= 'a' ) && ( buf[p] <= 'f' ) )
				 || ( ( buf[p] >= 'A' ) && ( buf[p] <= 'F' ) ) );

		if ( ! stat )
		{
			printf ( "Line %d : MD5 : Invalid string\n", linenr );
			goto bailout;
		}

		p++;
	}

	while ( ( buf[p] == 9 ) || ( buf[p] == 32 ) )
	{
		p++;
	}

	if ( ( buf[p] ) && ( buf[p] != 10 ) && ( buf[p] != 13 ) && ( buf[p] != ';' ) )
	{
		printf ( "Line %d : MD5 : Invalid data at end of string\n", linenr );
		goto bailout;
	}

	memcpy ( RC4MD5, &buf[pos], 32 );

	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	return ( fs );
}

// --

static enum RS4FuncStat
RS4Cmd_CODE ( RS4FileHeader * cfgfile UNUSED, RS4FileHeader * exefile, enum RS4ErrorCode * errcode, STR buf, S32 linenr )
{
	enum RS4ErrorCode ec;
	enum RS4FuncStat  fs;
	RS4Brance *		  rb;
	S64				  adr;
	S32				  pos;

	fs	= RS4FuncStat_Error;
	ec	= RS4ErrStat_Error;
	pos = 5;

	while ( ( buf[pos] == 9 ) || ( buf[pos] == 32 ) )
	{
		pos++;
	}

	ERR_CHK ( RS4ReadAddress ( &ec, buf, &pos, &adr ) )

	ERR_CHK ( RS4AddBrance_File ( &ec, &rb, exefile, adr ) )

	if ( ! rb )
	{
		// ec allready set

#ifdef DEBUG
		printf ( "Line %d : Code : Error adding code brance (Adr: $%08" PRIx64 ")\n", linenr, adr );
#endif

		goto bailout;
	}

	while ( ( buf[pos] == 9 ) || ( buf[pos] == 32 ) )
	{
		pos++;
	}

	if ( ( buf[pos] ) && ( buf[pos] != 10 ) && ( buf[pos] != ';' ) )
	{
		ec = RS4ErrStat_LineJunk;

#ifdef DEBUG
		printf ( "%s:%04d: Line %d : Code : Error junk at end of line (%d)\n", __FILE__, __LINE__, linenr, ec );
#endif

		printf ( "Line %d : Code : Error junk at end of line\n", linenr );
		goto bailout;
	}

	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	return ( fs );
}

// --

static enum RS4FuncStat
RS4Cmd_MEMWRITE ( RS4FileHeader * cfgfile UNUSED, RS4FileHeader * exefile, enum RS4ErrorCode * errcode, STR buf, S32 linenr )
{
	enum RS4ErrorCode ec;
	enum RS4FuncStat  fs;
	RS4FileSection *  sec;
	MEM				  mem;
	U32				  adr;
	U8				  chr;
	S32				  off;
	S32				  max;
	S32				  pos;
	S32				  cnt;
	S32				  hex;
	S32				  val;
	S32				  len;

	fs	= RS4FuncStat_Error;
	ec	= RS4ErrStat_Error;
	pos = 9;

	// skip cmd

	while ( ( buf[pos] == 9 ) || ( buf[pos] == 32 ) )
	{
		pos++;
	}

	if ( buf[pos] != '$' )
	{
		ec = RS4ErrStat_ExpectedHexVal;

#ifdef DEBUG
		printf ( "%s:%04d: Line %d : Reading address failed (%d)\n", __FILE__, __LINE__, linenr, ec );
#endif

		printf ( "Line %d : MemWrite : Address Argument must start with a $\n", linenr );
		goto bailout;
	}

	pos++;

	adr = 0;
	off = buf[pos];
	hex = HexData[off];

	if ( hex == -1 )
	{
		ec = RS4ErrStat_NeedOneDigit;

#ifdef DEBUG
		printf ( "%s:%04d: Line %d : Reading value failed (%d)\n", __FILE__, __LINE__, linenr, ec );
#endif

		printf ( "Line %d : MemWrite : Must be atleast one hex number after $\n", linenr );
		goto bailout;
	}

	for ( cnt = 0; cnt < 8; cnt++ )
	{
		val = buf[pos];
		hex = HexData[val];

		if ( hex == -1 )
		{
			break;
		}

		adr = adr << 4;
		adr = adr | hex;
		pos++;
	}

	ERR_CHK ( RS4FindSection_File ( &ec, &sec, exefile, adr ) )

	if ( ! sec )
	{
		ec = RS4ErrStat_Error;
		fs = RS4FuncStat_Error;

#ifdef DEBUG
		printf ( "%s:%04d: Error\n", __FILE__, __LINE__ );
#endif

		printf ( "Line %d : MemWrite : Address $%08x not found\n", linenr, adr );
		goto bailout;
	}

	mem = sec->rfs_MemoryBuf;
	max = sec->rfs_MemorySize;
	off = adr - sec->rfs_MemoryAdr;

	// --

	if ( ( buf[pos] != 9 ) && ( buf[pos] != 32 ) )
	{
		ec = RS4ErrStat_Error;
		fs = RS4FuncStat_Error;

#ifdef DEBUG
		printf ( "%s:%04d: Error\n", __FILE__, __LINE__ );
#endif

		printf ( "Line %d : MemWrite : Expected Space/Tab after address\n", linenr );
		goto bailout;
	}

	while ( ( buf[pos] == 9 ) || ( buf[pos] == 32 ) )
	{
		pos++;
	}

	if ( buf[pos] != '$' )
	{
		ec = RS4ErrStat_Error;
		fs = RS4FuncStat_Error;

#ifdef DEBUG
		printf ( "%s:%04d: Error\n", __FILE__, __LINE__ );
#endif

		printf ( "Line %d : MemWrite : Buffer Argument must start with a $\n", linenr );
		goto bailout;
	}

	pos++;

	// Count mem bytes

	len	   = pos;
	chr	   = 0;
	S32 oo = 1;

	while ( TRUE )
	{
		val = buf[len];
		hex = HexData[val];

		if ( hex == -1 )
		{
			break;
		}

		if ( max <= off )
		{
			ec = RS4ErrStat_Error;
			fs = RS4FuncStat_Error;

#ifdef DEBUG
			printf ( "%s:%04d: Error\n", __FILE__, __LINE__ );
#endif

			printf ( "Line %d : MemWrite : Writting past end of memory block\n", linenr );
			goto bailout;
		}

		chr = ( chr << 4 ) + ( hex );

		oo++;

		if ( oo & 1 )
		{
			// printf( "Write %02x to %08x\n", chr, hn->hn_MemoryAdr + off );
			mem[off++] = chr;
		}

		len++;
	}

	len -= pos;

	if ( len == 0 )
	{
		ec = RS4ErrStat_Error;
		fs = RS4FuncStat_Error;

#ifdef DEBUG
		printf ( "%s:%04d: Error\n", __FILE__, __LINE__ );
#endif

		printf ( "Line %d : MemWrite : Missing Memory Data\n", linenr );
		goto bailout;
	}

	if ( len & 1 )
	{
		ec = RS4ErrStat_Error;
		fs = RS4FuncStat_Error;

#ifdef DEBUG
		printf ( "%s:%04d: Error\n", __FILE__, __LINE__ );
#endif

		printf ( "Line %d : MemWrite : Memory Data, incomplete hex data\n", linenr );
		goto bailout;
	}

	pos += len;

	// --

	while ( ( buf[pos] == 9 ) || ( buf[pos] == 32 ) )
	{
		pos++;
	}

	if ( ( buf[pos] ) && ( buf[pos] != 10 ) && ( buf[pos] != 13 ) && ( buf[pos] != ';' ) )
	{
		ec = RS4ErrStat_Error;
		fs = RS4FuncStat_Error;

#ifdef DEBUG
		printf ( "%s:%04d: Error\n", __FILE__, __LINE__ );
#endif

		printf ( "Line %d : MemWrite : Invalid data at end of Code Argument\n", linenr );
		goto bailout;
	}

	// printf( "Patched Address : %08x\n", adr );

	fs = RS4FuncStat_Okay;
	ec = RS4ErrStat_Okay;

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	return ( fs );
}

// --

struct cmdstr
{
	S32 len;
	STR name;
	enum RS4FuncStat ( *func ) ( RS4FileHeader * cfgfile, RS4FileHeader * exefile, enum RS4ErrorCode * errcode, STR buf,
								 S32 linenr );
};

struct cmdstr cmds[] = {
	{ 5, "Code:", RS4Cmd_CODE },
	{ 11, "Relative16:", RS4Cmd_RELATIVE16 },
	{ 10, "LabelName:", RS4Cmd_LABELNAME },
	{ 10, "LabelType:", RS4Cmd_LABELTYPE },
	{ 9, "MemWrite:", RS4Cmd_MEMWRITE },
	{ 4, "MD5:", RS4Cmd_MD5 },
	{ 0, NULL, NULL },
};

S32
RS4ParseFile ( RS4FileHeader * cfgfile, RS4FileHeader * exefile, enum RS4ErrorCode * errcode )
{
	enum RS4ErrorCode ec;
	enum RS4FuncStat  fs;
	STR				  mem;
	S32				  linenr;
	S32				  size;
	S32				  err;
	S32				  pos;
	S32				  len;
	S32				  cnt;

	ec = RS4ErrStat_Error;

	err = TRUE;

	len = strlen ( CONFIG_HEADER );

	mem = (STR)cfgfile->rfh_FileBuffer;

	size = cfgfile->rfh_FileSize;

	// -- Check File Header

	if ( strncasecmp ( mem, CONFIG_HEADER, len ) )
	{
		ec = RS4ErrStat_InvalidFileType;

#ifdef DEBUG
		printf ( "%s:%04d: Error invalid Config file '%s' (%d)\n", __FILE__, __LINE__, "<file>", ec );
#endif

		goto bailout;
	}

	// -- Clear MD5 in case it do not get set

	memset ( RC4MD5, 'x', 32 );
	RC4MD5[32] = 0;

	// --

	pos = 0;

	linenr = 1;

	while ( ( mem[pos] ) && ( pos < size ) )
	{
		// -- Next Line

		while ( ( mem[pos] ) && ( mem[pos] != 10 ) )
		{
			pos++;
		}

		while ( ( mem[pos] ) && ( mem[pos] == 10 ) )
		{
			linenr++;
			pos++;
		}

		// -- Skip Space and Tab

		while ( ( mem[pos] == 9 ) || ( mem[pos] == 32 ) )
		{
			pos++;
		}

		// --

		if ( ( mem[pos] != 0 ) && ( mem[pos] != 10 ) && ( mem[pos] != 13 ) && ( mem[pos] != ';' ) )
		{
			// Get Command Length

			len = pos;

			while ( ( mem[len] ) && ( mem[len] != 9 ) && ( mem[len] != 32 ) )
			{
				len++;
			}

			len -= pos;

			// --

			cnt = 0;

			while ( cmds[cnt].len )
			{
				if ( ( cmds[cnt].len == len ) && ( ! strncasecmp ( &mem[pos], cmds[cnt].name, cmds[cnt].len ) ) )
				{
					break;
				}
				else
				{
					cnt++;
				}
			}

			// --

			if ( cmds[cnt].func )
			{
				ERR_CHK ( cmds[cnt].func ( cfgfile, exefile, &ec, &mem[pos], linenr ) )
			}
			else
			{
				printf ( "Skipping : LineNr %d : '", linenr );
				S32 ii = 0, ll = 10, c;
				while ( ll > 0 )
				{
					c = mem[pos + ii];

					if ( ( c < 32 ) || ( c > 127 ) )
					{
						break;
					}

					printf ( "%c", c );
					ii++;
				}
				printf ( "'\n" );
			}
		}
	}

	// -- Check MD5

	if ( strncasecmp ( exefile->rfh_FileMD5, RC4MD5, 32 ) )
	{
		ec = RS4ErrStat_MD5Fail;

#ifdef DEBUG
		printf ( "%s:%04d: MD5 Fail (%d)\n", __FILE__, __LINE__, ec );
#endif

		printf ( "Input file MD5 sum do not match the Config file sum\n" );
		printf ( "Cfg  : %s\n", RC4MD5 );
		printf ( "File : %s\n", exefile->rfh_FileMD5 );
		goto bailout;
	}

	// --

	ec = RS4ErrStat_Okay;

	err = FALSE;

bailout:

	if ( errcode )
	{
		*errcode = ec;
	}

	return ( err );
}

// --
