
/*
 * Copyright (c) 2014-2024 Rene W. Olsen < renewolsen @ gmail . com >
 *
 * This software is released under the GNU General Public License, version 3.
 * For the full text of the license, please visit:
 * https://www.gnu.org/licenses/gpl-3.0.html
 *
 * You can also find a copy of the license in the LICENSE file included with this software.
 */

// --

#include "ReSrc4.h"

// --

void Cmd_TRAPcc( struct M68kStruct *ms )
{
char labname[ MAX_LabelName + 8 ];
uint8_t *mem;
int cond;
int size;

	mem = ms->ms_MemoryBuf;

	cond = ( ms->ms_Opcode & 0x0f000000 ) >> 24;
	size = ( ms->ms_Opcode & 0x00070000 ) >> 16;

	switch( size )
	{
		case 2: // Trapcc.w
		{
			uint16_t val;

			val = (( mem[2] << 8 ) | ( mem[3] << 0 ));

			static const char *trapcc_RegNames[16] =
			{
				"Trapt.w",  "Trapf.w",  "Traphi.w", "Trapls.w",
				"Trapcc.w", "Trapcs.w", "Trapne.w", "Trapeq.w",
				"Trapvc.w", "Trapvs.w", "Trappl.w", "Trapmi.w",
				"Trapge.w", "Traplt.w", "Trapgt.w", "Traple.w"
			};

			ms->ms_Str_Opcode = trapcc_RegNames[cond];
			ms->ms_OpcodeSize = 4;
			ms->ms_ArgType  = OS_Word;

			snprintf( ms->ms_Buf_Argument, 64, "#$%04x", val );
			break;
		}

		case 3: // Trapcc.l
		{
			uint32_t val;

			val = (( mem[2] << 24 ) | ( mem[3] << 16 ) | ( mem[4] << 8 ) | ( mem[5] << 0 ));

			static const char *trapcc_RegNames[16] =
			{
				"Trapt.l",  "Trapf.l",  "Traphi.l", "Trapls.l",
				"Trapcc.l", "Trapcs.l", "Trapne.l", "Trapeq.l",
				"Trapvc.l", "Trapvs.l", "Trappl.l", "Trapmi.l",
				"Trapge.l", "Traplt.l", "Trapgt.l", "Traple.l"
			};

			ms->ms_Str_Opcode = trapcc_RegNames[cond];
			ms->ms_OpcodeSize = 6;
			ms->ms_ArgType  = OS_Long;

			if ( BuildLabelString2( ms, labname, ms->ms_MemoryAdr + ms->ms_ArgSize, val ))
			{
				printf( "%s:%04d: Label error\n", __FILE__, __LINE__ );
				goto bailout;
			}

			snprintf( ms->ms_Buf_Argument, 64, "#%s", labname );
			break;
		}

		case 4: // Trapcc
		{
			static const char *trapcc_RegNames[16] =
			{
				"Trapt",  "Trapf",  "Traphi", "Trapls",
				"Trapcc", "Trapcs", "Trapne", "Trapeq",
				"Trapvc", "Trapvs", "Trappl", "Trapmi",
				"Trapge", "Traplt", "Trapgt", "Traple"
			};

			ms->ms_Str_Opcode = trapcc_RegNames[cond];
			ms->ms_OpcodeSize = 2;
			break;
		}

		default:
		{
			printf( "Unsupported 'Trapcc' Opcode (Mode: %d)\n", size );
			ms->ms_DecodeStatus = MSTAT_Error;
			goto bailout;
		}
	}

bailout:

	return;
}

// --
