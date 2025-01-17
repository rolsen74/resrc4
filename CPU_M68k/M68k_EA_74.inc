
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
// -- Mode 74

inline static enum RS4DecodeStat MODE_74( enum RS4ErrorCode *errcode, RS4Trace *rt, char *outstr )
{
enum RS4DecodeStat ds;
enum RS4ErrorCode ec;
enum RS4FuncStat fs;
RS4Label *rl;
RS4Ref *isRef;
uint8_t *mem;
char labname[ MAX_LabelName + 8 ];
//int type;
int pos;

	// --

	ec = RS4ErrStat_Error;
	ds = RS4DecodeStat_Error;

	// --

	mem = rt->rt_CurMemBuf;
	pos = rt->rt_CPU.M68k.mt_ArgSize;

	switch( rt->rt_CPU.M68k.mt_ArgType )
	{
		case M68KSIZE_Byte:
		{
			if ( rt->rt_Pass != RS4TracePass_Trace )
			{
				uint8_t val;

				val = mem[pos+1] & 0x00ff;

				/**/ if ( rt->rt_CPU.M68k.mt_DecMode == M68kDecMode_Signed )	// Signed
				{
					sprintf( outstr, "#%d", val );
				}
				else if ( rt->rt_CPU.M68k.mt_DecMode == M68kDecMode_Unsigned )	// Unsigned
				{
					sprintf( outstr, "#%u", val );
				}
				else
				{
					sprintf( outstr, "#$%02x", val );
				}
			}
			else
			{
				outstr[0] = 0;
			}

			rt->rt_CPU.M68k.mt_ArgSize += 2;
			break;
		}

		case M68KSIZE_Word:
		{
			if ( rt->rt_Pass != RS4TracePass_Trace )
			{
				uint16_t val;

				val = (( mem[pos] << 8 ) | ( mem[pos+1] << 0 ));

				/**/ if ( rt->rt_CPU.M68k.mt_DecMode == M68kDecMode_Signed )	// Signed
				{
					sprintf( outstr, "#%d", val );
				}
				else if ( rt->rt_CPU.M68k.mt_DecMode == M68kDecMode_Unsigned )	// Unsigned
				{
					sprintf( outstr, "#%u", val );
				}
				else
				{
					sprintf( outstr, "#$%04x", val );
				}
			}
			else
			{
				outstr[0] = 0;
			}

			rt->rt_CPU.M68k.mt_ArgSize += 2;
			break;
		}

		case M68KSIZE_Long:
		{
			uint32_t adr;

			adr = (( mem[pos] << 24 ) | ( mem[pos+1] << 16 ) | ( mem[pos+2] << 8 ) | ( mem[pos+3] << 0 ));

			isRef = RS4FindRef_Sec( rt->rt_Section, rt->rt_CurMemAdr + rt->rt_CPU.M68k.mt_ArgSize );

			if ( isRef )
			{
				isRef->rr_Handled = TRUE;

				// if there is a Ref then the a label have been added
				rl = RS4FindLabel_File( rt->rt_File, adr );

				if ( ! rl )
				{
					ec = RS4ErrStat_Internal;
					ds = RS4DecodeStat_Error;

					#ifdef DEBUG
					printf( "%s:%04d: Error finding label at $%08" PRIx64 "\n", __FILE__, __LINE__, rt->rt_CurMemAdr );
					#endif

					goto bailout;
				}

				if ( rt->rt_Pass != RS4TracePass_Trace )
				{
					if ( rl->rl_Name[0] )
					{
						fs = RS4BuildLabelString( & ec, rl, labname );

						if ( fs != RS4FuncStat_Okay )
						{
							// ec allready set
							ds = RS4DecodeStat_Error;

							#ifdef DEBUG
							printf( "%s:%04d: Error '%s'\n", __FILE__, __LINE__, labname );
							#endif

							goto bailout;
						}

						sprintf( outstr, "#%s", labname );
					}
					else
					{
						/**/ if ( rt->rt_CPU.M68k.mt_DecMode == M68kDecMode_Signed )	// Signed
						{
							sprintf( outstr, "#%d", adr );
						}
						else if ( rt->rt_CPU.M68k.mt_DecMode == M68kDecMode_Unsigned )	// Unsigned
						{
							sprintf( outstr, "#%u", adr );
						}
						else
						{
							sprintf( outstr, "#$%08x", adr );
						}
					}
				}
				else
				{
					outstr[0] = 0;
				}

				if (( rl ) && ( rt->rt_CPU.M68k.mt_CurRegister ))
				{
					rt->rt_CPU.M68k.mt_CurRegister->mr_Type1 = RRT_Label;
					rt->rt_CPU.M68k.mt_CurRegister->mr_Label = rl;
				}
			}
			else
			{
				if ( rt->rt_Pass != RS4TracePass_Trace )
				{
					/**/ if ( rt->rt_CPU.M68k.mt_DecMode == M68kDecMode_Signed )	// Signed
					{
						sprintf( outstr, "#%d", adr );
					}
					else if ( rt->rt_CPU.M68k.mt_DecMode == M68kDecMode_Unsigned )	// Unsigned
					{
						sprintf( outstr, "#%u", adr );
					}
					else
					{
						sprintf( outstr, "#$%08x", adr );
					}
				}
				else
				{
					outstr[0] = 0;
				}

				if ( rt->rt_CPU.M68k.mt_CurRegister )
				{
					rt->rt_CPU.M68k.mt_CurRegister->mr_Type1 = RRT_Address;
					rt->rt_CPU.M68k.mt_CurRegister->mr_Address = adr;
				}
			}

			rt->rt_CPU.M68k.mt_ArgSize += 4;
			break;
		}

		case M68KSIZE_Single:
		{
			// Bits: 1 sign, 8 expo, 23 frac

			if ( rt->rt_Pass != RS4TracePass_Trace )
			{
				sprintf( outstr, "#$" );

				for( int cnt=0 ; cnt<4 ; cnt++ )
				{
					int len = strlen( outstr );

					sprintf( & outstr[len], "%02x", mem[pos+cnt] );
				}
			}
			else
			{
				outstr[0] = 0;
			}

			rt->rt_CPU.M68k.mt_ArgSize += 4;
			break;
		}

		case M68KSIZE_Double:
		{
			// Bits: 1 sign, 11 expo, 52 frac

			if ( rt->rt_Pass != RS4TracePass_Trace )
			{
				sprintf( outstr, "#$" );

				for( int cnt=0 ; cnt<8 ; cnt++ )
				{
					int len = strlen( outstr );

					sprintf( & outstr[len], "%02x", mem[pos+cnt] );
				}
			}
			else
			{
				outstr[0] = 0;
			}

			rt->rt_CPU.M68k.mt_ArgSize += 8;
			break;
		}

		case M68KSIZE_Extended:
		{
			// Bits: 1 sign, 15 expo, 16 ZERO, 64 mantissa

			if ( rt->rt_Pass != RS4TracePass_Trace )
			{
				sprintf( outstr, "#$" );

				for( int cnt=0 ; cnt<12 ; cnt++ )
				{
					int len = strlen( outstr );

					sprintf( & outstr[len], "%02x", mem[pos+cnt] );
				}
			}
			else
			{
				outstr[0] = 0;
			}

			rt->rt_CPU.M68k.mt_ArgSize += 12;
			break;
		}

//	M68KSIZE_Packed,		// fmove.p

		default:
		{
			printf( "%s:%04d: Unsupported EA mode at $%08" PRIx64 "\n", __FILE__, __LINE__, rt->rt_CurMemAdr );

			ec = RS4ErrStat_Internal;
			ds = RS4DecodeStat_Error;
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
