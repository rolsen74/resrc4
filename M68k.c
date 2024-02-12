
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

const char *	FPx_RegNames[8] = { "FP0","FP1","FP2","FP3","FP4","FP5","FP6","FP7" };
const char *	Ax_RegNames[8] = { "A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7" };
const char *	Dx_RegNames[8] = { "D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7" };
const char *	scale_Names[4] = { "",   "*2", "*4", "*8" };

// --

struct CmdStruct
{
	void		(*cs_Function)( struct M68kStruct *ms );
	uint32_t	cs_Mask;
	uint32_t	cs_Opcode;
};

// --

static const struct CmdStruct m68kCmds_0000[] =
{
//  { Cmd_ORI_CCR,  wwwwwwwwww, wwwwwwwwww },   // ORI      0000 0000 0011 1100 0000 0000 xxxx xxxx
//  { Cmd_ORI_SR,   wwwwwwwwww, wwwwwwwwww },   // ORI      0000 0000 0111 1100 xxxx xxxx xxxx xxxx

	{ Cmd_ORI,		0xffc00000, 0x00000000 },	// ORI.b	0000 0000 00xx xxxx
	{ Cmd_ORI,		0xffc00000, 0x00400000 },	// ORI.w	0000 0000 01xx xxxx
	{ Cmd_ORI,		0xffc00000, 0x00800000 },	// ORI.l	0000 0000 10xx xxxx
 	{ Cmd_CMP2,		0xffc00fff, 0x00c00000 },	// Cmp2.b	0000 0000 11xx xxxx xxxx 0000 0000 0000
 	{ Cmd_CHK2,		0xffc00fff, 0x00c00800 },	// Chk2.b	0000 0000 11xx xxxx xxxx 1000 0000 0000

//  { Cmd_ANDI_CCR, wwwwwwwwww, wwwwwwwwww },   // ANDI     0000 0010 0011 1100 0000 0000 xxxx xxxx
//  { Cmd_ANDI_SR,  wwwwwwwwww, wwwwwwwwww },   // ANDI     0000 0010 0111 1100 xxxx xxxx xxxx xxxx

	{ Cmd_ANDI,		0xffc00000, 0x02000000 },	// ANDI.b	0000 0010 00xx xxxx
	{ Cmd_ANDI,		0xffc00000, 0x02400000 },	// ANDI.w	0000 0010 01xx xxxx
	{ Cmd_ANDI,		0xffc00000, 0x02800000 },	// ANDI.l	0000 0010 10xx xxxx
 	{ Cmd_CMP2,		0xffc00fff, 0x02c00000 },	// Cmp2.w	0000 0010 11xx xxxx xxxx 0000 0000 0000
 	{ Cmd_CHK2,		0xffc00fff, 0x02c00800 },	// Chk2.w	0000 0010 11xx xxxx xxxx 1000 0000 0000

	{ Cmd_SUBI,		0xffc00000, 0x04000000 },	// SUBI.b	0000 0100 00xx xxxx
	{ Cmd_SUBI,		0xffc00000, 0x04400000 },	// SUBI.w	0000 0100 01xx xxxx
	{ Cmd_SUBI,		0xffc00000, 0x04800000 },	// SUBI.l	0000 0100 10xx xxxx
 	{ Cmd_CMP2,		0xffc00fff, 0x04c00000 },	// Cmp2.l	0000 0100 11xx xxxx xxxx 0000 0000 0000
 	{ Cmd_CHK2,		0xffc00fff, 0x04c00800 },	// Chk2.l	0000 0100 11xx xxxx xxxx 1000 0000 0000

	{ Cmd_ADDI,		0xffc00000, 0x06000000 },	// ADDI.b	0000 0110 00xx xxxx
	{ Cmd_ADDI,		0xffc00000, 0x06400000 },	// ADDI.w	0000 0110 01xx xxxx
	{ Cmd_ADDI,		0xffc00000, 0x06800000 },	// ADDI.l	0000 0110 10xx xxxx
	{ Cmd_RTM,		0xfff00000, 0x06c00000 },	// RTM		0000 0110 1100 xxxx
	{ Cmd_CALLM,	0xffc0ff00, 0x06c00000 },	// CALLM	0000 0110 11xx xxxx 0000 0000 xxxx xxxx

	{ Cmd_BTST,		0xffc0ff00, 0x08000000 },	// BTST     0000 1000 00xx xxxx 0000 0000 xxxx xxxx
	{ Cmd_BCHG,		0xffc0ff00, 0x08400000 },	// BCHG     0000 1000 01xx xxxx 0000 0000 xxxx xxxx
	{ Cmd_BCLR,		0xffc0ff00, 0x08800000 },	// BCLR     0000 1000 10xx xxxx 0000 0000 xxxx xxxx
	{ Cmd_BSET,		0xffc0ff00, 0x08c00000 },	// BSET     0000 1000 11xx xxxx 0000 0000 xxxx xxxx

//  { Cmd_EORI_CCR, wwwwwwwwww, wwwwwwwwww },   // EORI     0000 1010 0011 1100 0000 0000 xxxx xxxx
	{ Cmd_EORI,		0xffc00000, 0x0a000000 },	// EORI.b	0000 1010 00xx xxxx
//  { Cmd_EORI_SR,  wwwwwwwwww, wwwwwwwwww },   // EORI     0000 1010 0111 1100 xxxx xxxx xxxx xxxx
	{ Cmd_EORI,		0xffc00000, 0x0a400000 },	// EORI.w	0000 1010 01xx xxxx
	{ Cmd_EORI,		0xffc00000, 0x0a800000 },	// EORI.l	0000 1010 10xx xxxx

	{ Cmd_CMPI,		0xffc00000, 0x0c000000 },	// CMPI.b	0000 1100 00xx xxxx
	{ Cmd_CMPI,		0xffc00000, 0x0c400000 },	// CMPI.w	0000 1100 01xx xxxx
	{ Cmd_CMPI,		0xffc00000, 0x0c800000 },	// CMPI.l	0000 1100 10xx xxxx

//  { Cmd_MOVES,    wwwwwwwwww, wwwwwwwwww },   // MOVES    0000 1110 xxxx xxxx xxxx x000 0000 0000
//  { Cmd_CAS2,     wwwwwwwwww, wwwwwwwwww },   // CAS2     0000 1xx0 1111 1100 xxxx 000x xx00 0xxx xxxx 000x xx00 0xxxx
//  { Cmd_CAS,      wwwwwwwwww, wwwwwwwwww },   // CAS      0000 1xx0 11xx xxxx 0000 000x xx00 0xxx

	{ Cmd_MOVEP,	0xf1f80000, 0x01080000 },	// MOVEP.w	0000 xxx1 0000 1xxx
	{ Cmd_BTST2,	0xf1c00000, 0x01000000 },	// BTST     0000 xxx1 00xx xxxx
	{ Cmd_MOVEP,	0xf1f80000, 0x01480000 },	// MOVEP.l	0000 xxx1 0100 1xxx
	{ Cmd_BCHG2,	0xf1c00000, 0x01400000 },	// BCHG     0000 xxx1 01xx xxxx
	{ Cmd_MOVEP,	0xf1f80000, 0x01880000 },	// MOVEP.w	0000 xxx1 1000 1xxx
	{ Cmd_BCLR2,	0xf1c00000, 0x01800000 },	// BCLR     0000 xxx1 10xx xxxx
	{ Cmd_MOVEP,	0xf1f80000, 0x01c80000 },	// MOVEP.l	0000 xxx1 1100 1xxx
	{ Cmd_BSET2,	0xf1c00000, 0x01c00000 },	// BSET     0000 xxx1 11xx xxxx

	{ NULL,			0x00000000, 0x00000000 },
};

static const struct CmdStruct m68kCmds_0001[] =
{
	{ Cmd_MOVE,		0xf0000000, 0x10000000 },	// MOVE.b	0001 xxxx xxxx xxxx
	{ NULL,			0x00000000, 0x00000000 },
};

static const struct CmdStruct m68kCmds_0010[] =
{
	{ Cmd_MOVE,		0xf1c00000, 0x20000000 },	// MOVE.l	0010 xxx0 00xx xxxx
	{ Cmd_MOVEA,	0xf1c00000, 0x20400000 },	// MOVEA.l	0010 xxx0 01xx xxxx
	{ Cmd_MOVE,		0xf1c00000, 0x20800000 },	// MOVE.l	0010 xxx0 10xx xxxx
	{ Cmd_MOVE,		0xf1c00000, 0x20c00000 },	// MOVE.l	0010 xxx0 11xx xxxx
	{ Cmd_MOVE,		0xf1c00000, 0x21000000 },	// MOVE.l	0010 xxx1 00xx xxxx
	{ Cmd_MOVE,		0xf1c00000, 0x21400000 },	// MOVE.l	0010 xxx1 01xx xxxx
	{ Cmd_MOVE,		0xf1c00000, 0x21800000 },	// MOVE.l	0010 xxx1 10xx xxxx
	{ Cmd_MOVE,		0xf1c00000, 0x21c00000 },	// MOVE.l	0010 xxx1 11xx xxxx
	{ NULL,			0x00000000, 0x00000000 },
};

static const struct CmdStruct m68kCmds_0011[] =
{
	{ Cmd_MOVE,		0xf1c00000, 0x30000000 },	// MOVE.w	0010 xxx0 00xx xxxx
	{ Cmd_MOVEA,	0xf1c00000, 0x30400000 },	// MOVEA.w	0010 xxx0 01xx xxxx
	{ Cmd_MOVE,		0xf1c00000, 0x30800000 },	// MOVE.w	0010 xxx0 10xx xxxx
	{ Cmd_MOVE,		0xf1c00000, 0x30c00000 },	// MOVE.w	0010 xxx0 11xx xxxx
	{ Cmd_MOVE,		0xf1c00000, 0x31000000 },	// MOVE.w	0010 xxx1 00xx xxxx
	{ Cmd_MOVE,		0xf1c00000, 0x31400000 },	// MOVE.w	0010 xxx1 01xx xxxx
	{ Cmd_MOVE,		0xf1c00000, 0x31800000 },	// MOVE.w	0010 xxx1 10xx xxxx
	{ Cmd_MOVE,		0xf1c00000, 0x31c00000 },	// MOVE.w	0010 xxx1 11xx xxxx
	{ NULL,			0x00000000, 0x00000000 },
};

static const struct CmdStruct m68kCmds_0100[] =
{
//    { Cmd_NEGX,     0xffc00000, 0x40000000 },   // NEGX.b   0100 0000 00xx xxxx
//    { Cmd_NEGX,     0xffc00000, 0x40400000 },   // NEGX.w   0100 0000 01xx xxxx
//    { Cmd_NEGX,     0xffc00000, 0x40800000 },   // NEGX.l   0100 0000 10xx xxxx
//  { Cmd_MOVE_SR,  wwwwwwwwww, wwwwwwwwww },   // MOVE     0100 0000 11xx xxxx

	{ Cmd_CHK,		0xf1c00000, 0x41000000 },	// CHK.l	0100 xxx1 00xx xxxx
	{ Cmd_CHK,		0xf1c00000, 0x41800000 },	// CHK.w	0100 xxx1 10xx xxxx

	{ Cmd_CLR,		0xffc00000, 0x42000000 },	// CLR.b    0100 0010 00xx xxxx
	{ Cmd_CLR,		0xffc00000, 0x42400000 },	// CLR.w    0100 0010 01xx xxxx
	{ Cmd_CLR,		0xffc00000, 0x42800000 },	// CLR.l    0100 0010 10xx xxxx
//  { Cmd_MOVE_CCR, wwwwwwwwww, wwwwwwwwww },   // MOVE     0100 0010 11xx xxxx

	{ Cmd_NEG,		0xffc00000, 0x44000000 },	// NEG.b	0100 0100 00xx xxxx
	{ Cmd_NEG,		0xffc00000, 0x44400000 },	// NEG.w	0100 0100 01xx xxxx
	{ Cmd_NEG,		0xffc00000, 0x44800000 },	// NEG.l	0100 0100 10xx xxxx
//  { Cmd_MOVE_CCR, 0xffc00000, 0x44c00000 },   // MOVE     0100 0100 11xx xxxx

	{ Cmd_NOT,		0xffc00000, 0x46000000 },	// NOT.b	0100 0110 00xx xxxx
	{ Cmd_NOT,		0xffc00000, 0x46400000 },	// NOT.w	0100 0110 01xx xxxx
	{ Cmd_NOT,		0xffc00000, 0x46800000 },	// NOT.l	0100 0110 10xx xxxx
//  { Cmd_MOVE_SR,  wwwwwwwwww, wwwwwwwwww },   // MOVE     0100 0110 11xx xxxx

	{ Cmd_LINK_L,	0xfff80000, 0x48080000 },	// LINK.l	0100 1000 0000 1xxx
	{ Cmd_NBCD,		0xffc00000, 0x48000000 },	// NBCD		0100 1000 00xx xxxx
	{ Cmd_SWAP,		0xfff80000, 0x48400000 },	// SWAP		0100 1000 0100 0xxx
	{ Cmd_BKPT,		0xfff80000, 0x48480000 },	// BKPT		0100 1000 0100 1xxx
	{ Cmd_PEA,		0xffc00000, 0x48400000 },	// PEA		0100 1000 01xx xxxx
	{ Cmd_EXT,		0xfff80000, 0x48800000 },	// EXT      0100 1000 1000 0xxx
	{ Cmd_MOVEM,	0xffc00000, 0x48800000 },	// MOVEM    0100 1000 10xx xxxx
	{ Cmd_EXT,		0xfff80000, 0x48c00000 },	// EXT      0100 1000 1100 0xxx
	{ Cmd_EXT,		0xfff80000, 0x49c00000 },	// EXT      0100 1001 1100 0xxx
	{ Cmd_MOVEM,	0xffc00000, 0x48c00000 },	// MOVEM    0100 1000 11xx xxxx

	{ Cmd_TST,		0xffc00000, 0x4a000000 },	// TST.b    0100 1010 00xx xxxx
	{ Cmd_TST,		0xffc00000, 0x4a400000 },	// TST.w    0100 1010 01xx xxxx
	{ Cmd_TST,		0xffc00000, 0x4a800000 },	// TST.l    0100 1010 10xx xxxx
	{ Cmd_BGND,		0xffff0000, 0x4afa0000 },	// BGND		0100 1010 1111 1010
	{ Cmd_ILLEGAL,	0xffff0000, 0x4afc0000 },	// ILLEGAL	0100 1010 1111 1100
//  { Cmd_TAS,      wwwwwwwwww, wwwwwwwwww },   // TAS      0100 1010 11xx xxxx

	{ Cmd_MULU_L,	0xffc08bf8, 0x4c000000 },	// MULU_L	0100 1100 00xx xxxx 0xxx 0x00 0000 0xxx
	{ Cmd_MULS_L,	0xffc08bf8, 0x4c000800 },	// MULS_L	0100 1100 00xx xxxx 0xxx 1x00 0000 0xxx
	{ Cmd_DIVU_L,	0xffc08bf8, 0x4c400000 },	// DIVU_L	0100 1100 01xx xxxx 0xxx 0x00 0000 0xxx
	{ Cmd_DIVS_L,	0xffc08bf8, 0x4c400800 },	// DIVS_L	0100 1100 01xx xxxx 0xxx 1x00 0000 0xxx
	{ Cmd_MOVEM,	0xffc00000, 0x4c800000 },	// MOVEM	0100 1100 10xx xxxx
	{ Cmd_MOVEM,	0xffc00000, 0x4cc00000 },	// MOVEM	0100 1100 11xx xxxx

	{ Cmd_TRAP,		0xfff00000, 0x4e400000 },	// TRAP		0100 1110 0100 xxxx
	{ Cmd_LINK,		0xfff80000, 0x4e500000 },	// LINK.w	0100 1110 0101 0xxx
	{ Cmd_UNLK,		0xfff80000, 0x4e580000 },	// UNLK		0100 1110 0101 1xxx
//  { Cmd_MOVE_USP, wwwwwwwwww, wwwwwwwwww },   // MOVE     0100 1110 0110 xxxx
	{ Cmd_RESET,	0xffff0000, 0x4e700000 },	// RESET	0100 1110 0111 0000
	{ Cmd_NOP,		0xffff0000, 0x4e710000 },	// NOP		0100 1110 0111 0001
	{ Cmd_RTE,		0xffff0000, 0x4e730000 },	// RTE		0100 1110 0111 0011
	{ Cmd_RTD,		0xffff0000, 0x4e740000 },	// RTD		0100 1110 0111 0100
	{ Cmd_RTS,		0xffff0000, 0x4e750000 },	// RTS      0100 1110 0111 0101
	{ Cmd_TRAPV,	0xffff0000, 0x4e760000 },	// TRAPV	0100 1110 0111 0110
	{ Cmd_RTR,		0xffff0000, 0x4e770000 },	// RTR		0100 1110 0111 0111
//	{ Cmd_MOVEC,    wwwwwwwwww, wwwwwwwwww },   // MOVEC    0100 1110 0111 101x
	{ Cmd_JSR,		0xffc00000, 0x4e800000 },	// JSR      0100 1110 10xx xxxx
	{ Cmd_JMP,		0xffc00000, 0x4ec00000 },	// JMP      0100 1110 11xx xxxx
	{ Cmd_LEA,		0xf1c00000, 0x41c00000 },	// LEA      0100 xxx1 11xx xxxx
	{ NULL,			0x00000000, 0x00000000 },
};

static const struct CmdStruct m68kCmds_0101[] =
{
	{ Cmd_ADDQ,		0xf1c00000, 0x50000000 },	// ADDQ.b	0101 xxx0 00xx xxxx
	{ Cmd_ADDQ,		0xf1c00000, 0x50400000 },	// ADDQ.w	0101 xxx0 01xx xxxx
	{ Cmd_ADDQ,		0xf1c00000, 0x50800000 },	// ADDQ.l	0101 xxx0 10xx xxxx

	{ Cmd_SUBQ,		0xf1c00000, 0x51000000 },	// SUBQ.b	0101 xxx1 00xx xxxx
	{ Cmd_SUBQ,		0xf1c00000, 0x51400000 },	// SUBQ.w	0101 xxx1 01xx xxxx
	{ Cmd_SUBQ,		0xf1c00000, 0x51800000 },	// SUBQ.l	0101 xxx1 10xx xxxx

	#if 0
	{ Cmd_DBcc,		0xfff80000, 0x50c80000 },	// DBT		0101 0000 1100 1xxx
	{ Cmd_DBcc,		0xfff80000, 0x51c80000 },	// DBF		0101 0001 1100 1xxx
	{ Cmd_DBcc,		0xfff80000, 0x52c80000 },	// DBHI		0101 0010 1100 1xxx
	{ Cmd_DBcc,		0xfff80000, 0x53c80000 },	// DBLS		0101 0011 1100 1xxx
	{ Cmd_DBcc,		0xfff80000, 0x54c80000 },	// DBCC		0101 0100 1100 1xxx
	{ Cmd_DBcc,		0xfff80000, 0x55c80000 },	// DBCS		0101 0101 1100 1xxx
	{ Cmd_DBcc,		0xfff80000, 0x56c80000 },	// DBNE		0101 0110 1100 1xxx
	{ Cmd_DBcc,		0xfff80000, 0x57c80000 },	// DBEQ		0101 0111 1100 1xxx
	{ Cmd_DBcc,		0xfff80000, 0x58c80000 },	// DBVC		0101 1000 1100 1xxx
	{ Cmd_DBcc,		0xfff80000, 0x59c80000 },	// DBVS		0101 1001 1100 1xxx
	{ Cmd_DBcc,		0xfff80000, 0x5ac80000 },	// DBPL		0101 1010 1100 1xxx
	{ Cmd_DBcc,		0xfff80000, 0x5bc80000 },	// DBMI		0101 1011 1100 1xxx
	{ Cmd_DBcc,		0xfff80000, 0x5cc80000 },	// DBGE		0101 1100 1100 1xxx
	{ Cmd_DBcc,		0xfff80000, 0x5dc80000 },	// DBLT		0101 1101 1100 1xxx
	{ Cmd_DBcc,		0xfff80000, 0x5ec80000 },	// DBGT		0101 1110 1100 1xxx
	{ Cmd_DBcc,		0xfff80000, 0x5fc80000 },	// DBLE		0101 1111 1100 1xxx
	#else
	{ Cmd_DBcc,		0xf0f80000, 0x50c80000 },	// DBcc		0101 xxxx 1100 1xxx
	#endif

//  { Cmd_TRAPcc,   0xfff80000, 0x50c80000 },   // TRAPT    0101 0000 1111 1xxx
//  { Cmd_TRAPcc,   0xfff80000, 0x51c80000 },   // TRAPF    0101 0001 1111 1xxx
//  { Cmd_TRAPcc,   0xfff80000, 0x52c80000 },   // TRAPHI   0101 0010 1111 1xxx
//  { Cmd_TRAPcc,   0xfff80000, 0x53c80000 },   // TRAPLS   0101 0011 1111 1xxx
//  { Cmd_TRAPcc,   0xfff80000, 0x54c80000 },   // TRAPCC   0101 0100 1111 1xxx
//  { Cmd_TRAPcc,   0xfff80000, 0x55c80000 },   // TRAPCS   0101 0101 1111 1xxx
//  { Cmd_TRAPcc,   0xfff80000, 0x56c80000 },   // TRAPNE   0101 0110 1111 1xxx
//  { Cmd_TRAPcc,   0xfff80000, 0x57c80000 },   // TRAPEQ   0101 0111 1111 1xxx
//  { Cmd_TRAPcc,   0xfff80000, 0x58c80000 },   // TRAPVC   0101 1000 1111 1xxx
//  { Cmd_TRAPcc,   0xfff80000, 0x59c80000 },   // TRAPVS   0101 1001 1111 1xxx
//  { Cmd_TRAPcc,   0xfff80000, 0x5ac80000 },   // TRAPPL   0101 1010 1111 1xxx
//  { Cmd_TRAPcc,   0xfff80000, 0x5bc80000 },   // TRAPMI   0101 1011 1111 1xxx
//  { Cmd_TRAPcc,   0xfff80000, 0x5cc80000 },   // TRAPGE   0101 1100 1111 1xxx
//  { Cmd_TRAPcc,   0xfff80000, 0x5dc80000 },   // TRAPLT   0101 1101 1111 1xxx
//  { Cmd_TRAPcc,   0xfff80000, 0x5ec80000 },   // TRAPGT   0101 1110 1111 1xxx
//  { Cmd_TRAPcc,   0xfff80000, 0x5fc80000 },   // TRAPLE   0101 1111 1111 1xxx

	#if 0
	{ Cmd_Scc,		0xffc00000, 0x50c00000 },	// ST		0101 0000 11xx xxxx
	{ Cmd_Scc,		0xffc00000, 0x51c00000 },	// SF		0101 0001 11xx xxxx
	{ Cmd_Scc,		0xffc00000, 0x52c00000 },	// SHI		0101 0010 11xx xxxx
	{ Cmd_Scc,		0xffc00000, 0x53c00000 },	// SLS		0101 0011 11xx xxxx
	{ Cmd_Scc,		0xffc00000, 0x54c00000 },	// SCC		0101 0100 11xx xxxx
	{ Cmd_Scc,		0xffc00000, 0x55c00000 },	// SCS		0101 0101 11xx xxxx
	{ Cmd_Scc,		0xffc00000, 0x56c00000 },	// SNE		0101 0110 11xx xxxx
	{ Cmd_Scc,		0xffc00000, 0x57c00000 },	// SEQ		0101 0111 11xx xxxx
	{ Cmd_Scc,		0xffc00000, 0x58c00000 },	// SVC		0101 1000 11xx xxxx
	{ Cmd_Scc,		0xffc00000, 0x59c00000 },	// SVS		0101 1001 11xx xxxx
	{ Cmd_Scc,		0xffc00000, 0x5ac00000 },	// SPL		0101 1010 11xx xxxx
	{ Cmd_Scc,		0xffc00000, 0x5bc00000 },	// SMI		0101 1011 11xx xxxx
	{ Cmd_Scc,		0xffc00000, 0x5cc00000 },	// SGE		0101 1100 11xx xxxx
	{ Cmd_Scc,		0xffc00000, 0x5dc00000 },	// SLT		0101 1101 11xx xxxx
	{ Cmd_Scc,		0xffc00000, 0x5ec00000 },	// SGT		0101 1110 11xx xxxx
	{ Cmd_Scc,		0xffc00000, 0x5fc00000 },	// SLE		0101 1111 11xx xxxx
	#else
	{ Cmd_Scc,		0xf0c00000, 0x50c00000 },	// Scc		0101 1111 11xx xxxx
	#endif

	{ NULL,			0x00000000, 0x00000000 },
};

static const struct CmdStruct m68kCmds_0110[] =
{
	{ Cmd_Bcc,		0xf0000000, 0x60000000 },	// Bcc      0110 xxxx xxxx xxxx
	{ NULL,			0x00000000, 0x00000000 },
};

static const struct CmdStruct m68kCmds_0111[] =
{
	{ Cmd_MOVEQ,	0xf1000000, 0x70000000 },	// MOVEQ    0111 xxx0 xxxx xxxx
	{ NULL,			0x00000000, 0x00000000 },
};

static const struct CmdStruct m68kCmds_1000[] =
{
	{ Cmd_OR,		0xf1c00000, 0x80000000 },	// OR       1000 xxx0 00xx xxxx
	{ Cmd_OR,		0xf1c00000, 0x80400000 },	// OR       1000 xxx0 01xx xxxx
	{ Cmd_OR,		0xf1c00000, 0x80800000 },	// OR       1000 xxx0 10xx xxxx
	{ Cmd_DIV,		0xf1c00000, 0x80c00000 },	// DIVU.w   1000 xxx0 11xx xxxx
	{ Cmd_SBCD,		0xf1f00000, 0x81000000 },	// SBCD		1000 xxx1 0000 xxxx
	{ Cmd_OR,		0xf1c00000, 0x81000000 },	// OR       1000 xxx1 00xx xxxx
	{ Cmd_PACK,		0xf1f00000, 0x81400000 },	// PACK		1000 xxx1 0100 xxxx
	{ Cmd_OR,		0xf1c00000, 0x81400000 },	// OR       1000 xxx1 01xx xxxx
	{ Cmd_UNPK,		0xf1f00000, 0x81800000 },	// UNPK		1000 xxx1 1000 xxxx
	{ Cmd_OR,		0xf1c00000, 0x81800000 },	// OR       1000 xxx1 10xx xxxx
	{ Cmd_DIV,		0xf1c00000, 0x81c00000 },	// DIVS.w   1000 xxx1 11xx xxxx
	{ NULL,			0x00000000, 0x00000000 },
};

static const struct CmdStruct m68kCmds_1001[] =
{
	{ Cmd_SUB,		0xf1c00000, 0x90000000 },	// SUB		1001 xxx0 00xx xxxx
	{ Cmd_SUB,		0xf1c00000, 0x90400000 },	// SUB		1001 xxx0 01xx xxxx
	{ Cmd_SUB,		0xf1c00000, 0x90800000 },	// SUB		1001 xxx0 10xx xxxx
	{ Cmd_SUBA,		0xf1c00000, 0x90c00000 },	// SUBA		1001 xxx0 11xx xxxx
	{ Cmd_SUBX,		0xf1f00000, 0x91000000 },	// SUBX.b	1001 xxx1 0000 xxxx
	{ Cmd_SUB,		0xf1c00000, 0x91000000 },	// SUB		1001 xxx1 00xx xxxx
	{ Cmd_SUBX,		0xf1f00000, 0x91400000 },	// SUBX.w	1001 xxx1 0100 xxxx
	{ Cmd_SUB,		0xf1c00000, 0x91400000 },	// SUB		1001 xxx1 01xx xxxx
	{ Cmd_SUBX,		0xf1f00000, 0x91800000 },	// SUBX.l	1001 xxx1 1000 xxxx
	{ Cmd_SUB,		0xf1c00000, 0x91800000 },	// SUB		1001 xxx1 10xx xxxx
	{ Cmd_SUBA,		0xf1c00000, 0x91c00000 },	// SUBA		1001 xxx1 11xx xxxx
	{ NULL,			0x00000000, 0x00000000 },
};

static const struct CmdStruct m68kCmds_1010[] =
{
	{ NULL,			0x00000000, 0x00000000 },
};

static const struct CmdStruct m68kCmds_1011[] =
{
	{ Cmd_CMP,		0xf1c00000, 0xb0000000 },	// CMP.b	1011 xxx0 00xx xxxx
	{ Cmd_CMP,		0xf1c00000, 0xb0400000 },	// CMP.w	1011 xxx0 01xx xxxx
	{ Cmd_CMP,		0xf1c00000, 0xb0800000 },	// CMP.l	1011 xxx0 10xx xxxx
	{ Cmd_CMPA,		0xf1c00000, 0xb0c00000 },	// CMPA.w	1011 xxx0 11xx xxxx
	{ Cmd_CMPM,		0xf1f80000, 0xb1080000 },	// CMPM.b	1011 xxx1 0000 1xxx
	{ Cmd_EOR,		0xf1c00000, 0xb1000000 },	// EOR.b    1011 xxx1 00xx xxxx
	{ Cmd_CMPM,		0xf1f80000, 0xb1480000 },	// CMPM.w	1011 xxx1 0100 1xxx
	{ Cmd_EOR,		0xf1c00000, 0xb1400000 },	// EOR.w    1011 xxx1 01xx xxxx
	{ Cmd_CMPM,		0xf1f80000, 0xb1880000 },	// CMPM.l	1011 xxx1 1000 1xxx
	{ Cmd_EOR,		0xf1c00000, 0xb1800000 },	// EOR.l    1011 xxx1 10xx xxxx
	{ Cmd_CMPA,		0xf1c00000, 0xb1c00000 },	// CMPA.l   1011 xxx1 11xx xxxx

	{ NULL,			0x00000000, 0x00000000 },
};

static const struct CmdStruct m68kCmds_1100[] =
{
	{ Cmd_AND,		0xf1c00000, 0xc0000000 },	// AND.b	1100 xxx0 00xx xxxx
	{ Cmd_AND,		0xf1c00000, 0xc0400000 },	// AND.w	1100 xxx0 01xx xxxx
	{ Cmd_AND,		0xf1c00000, 0xc0800000 },	// AND.l	1100 xxx0 10xx xxxx
	{ Cmd_MUL,		0xf1c00000, 0xc0c00000 },	// MULU.w	1100 xxx0 11xx xxxx
	{ Cmd_ABCD,		0xf1f00000, 0xc1000000 },	// ABCD.b	1100 xxx1 0000 xxxx
	{ Cmd_AND,		0xf1c00000, 0xc1000000 },	// AND.b	1100 xxx1 00xx xxxx
	{ Cmd_EXG,		0xf1f80000, 0xc1400000 },   // EXG.l	1100 xxx1 0100 0xxx
	{ Cmd_EXG,		0xf1f80000, 0xc1480000 },   // EXG.l	1100 xxx1 0100 1xxx
	{ Cmd_AND,		0xf1c00000, 0xc1400000 },	// AND.w	1100 xxx1 01xx xxxx
	{ Cmd_EXG,		0xf1f80000, 0xc1880000 },   // EXG.l	1100 xxx1 1000 1xxx
	{ Cmd_AND,		0xf1c00000, 0xc1800000 },	// AND.l	1100 xxx1 10xx xxxx
	{ Cmd_MUL,		0xf1c00000, 0xc1c00000 },	// MULS.w	1100 xxx1 11xx xxxx
	{ NULL,			0x00000000, 0x00000000 },
};

static const struct CmdStruct m68kCmds_1101[] =
{
	{ Cmd_ADD,		0xf1c00000, 0xd0000000 },	// ADD.b    1101 xxx0 00xx xxxx
	{ Cmd_ADD,		0xf1c00000, 0xd0400000 },	// ADD.w    1101 xxx0 01xx xxxx
	{ Cmd_ADD,		0xf1c00000, 0xd0800000 },	// ADD.l    1101 xxx0 10xx xxxx
	{ Cmd_ADDA,		0xf1c00000, 0xd0c00000 },	// ADDA.w   1101 xxx0 11xx xxxx
	{ Cmd_ADDX,		0xf1f00000, 0xd1000000 },	// ADDX.b	1101 xxx1 0000 xxxx
	{ Cmd_ADD,		0xf1c00000, 0xd1000000 },	// ADD.b    1101 xxx1 00xx xxxx
	{ Cmd_ADDX,		0xf1f00000, 0xd1400000 },	// ADDX.w	1101 xxx1 0100 xxxx
	{ Cmd_ADD,		0xf1c00000, 0xd1400000 },	// ADD.w    1101 xxx1 01xx xxxx
	{ Cmd_ADDX,		0xf1f00000, 0xd1800000 },	// ADDX.l	1101 xxx1 1000 xxxx
	{ Cmd_ADD,		0xf1c00000, 0xd1800000 },	// ADD.l    1101 xxx1 10xx xxxx
	{ Cmd_ADDA,		0xf1c00000, 0xd1c00000 },	// ADDA.l   1101 xxx1 11xx xxxx
	{ NULL,			0x00000000, 0x00000000 },
};

static const struct CmdStruct m68kCmds_1110[] =
{
//  { Cmd_BFTST     wwwwwwwwww, wwwwwwwwww },   // BFTST    1110 1000 11xx xxxx 0000 xxxx xxxxx xxxx
//  { Cmd_BFEXTU    wwwwwwwwww, wwwwwwwwww },   // BFEXTU   1110 1001 11xx xxxx 0xxx xxxx xxxxx xxxx
//  { Cmd_BFCHG     wwwwwwwwww, wwwwwwwwww },   // BFCHG    1110 1010 11xx xxxx 0000 xxxx xxxxx xxxx
//  { Cmd_BFEXTS    wwwwwwwwww, wwwwwwwwww },   // BFEXTS   1110 1011 11xx xxxx 0xxx xxxx xxxxx xxxx
//  { Cmd_BFCLR     wwwwwwwwww, wwwwwwwwww },   // BFCLT    1110 1100 11xx xxxx 0000 xxxx xxxxx xxxx
//  { Cmd_BFFFO     wwwwwwwwww, wwwwwwwwww },   // BFFFO    1110 1101 11xx xxxx 0xxx xxxx xxxxx xxxx
//  { Cmd_BFSET     wwwwwwwwww, wwwwwwwwww },   // BFSET    1110 1110 11xx xxxx 0000 xxxx xxxxx xxxx
//  { Cmd_BFINS     wwwwwwwwww, wwwwwwwwww },   // BFINS    1110 1111 11xx xxxx 0xxx xxxx xxxxx xxxx

	{ Cmd_ASR,		0xf1d80000, 0xe0000000 },	// ASR.b	1110 xxx0 00x0 0xxx
	{ Cmd_LSR,		0xf1d80000, 0xe0080000 },	// LSR.b	1110 xxx0 00x0 1xxx
	{ Cmd_ROXR,		0xf1d80000, 0xe0100000 },	// ROXR.b	1110 xxx0 00x1 0xxx
	{ Cmd_ROR,		0xf1d80000, 0xe0180000 },	// ROR.b	1110 xxx0 00x1 1xxx
	{ Cmd_ASR,		0xf1d80000, 0xe0400000 },	// ASR.w	1110 xxx0 01x0 0xxx
	{ Cmd_LSR,		0xf1d80000, 0xe0480000 },	// LSR.w	1110 xxx0 01x0 1xxx
	{ Cmd_ROXR,		0xf1d80000, 0xe0500000 },	// ROXR.w	1110 xxx0 01x1 0xxx
	{ Cmd_ROR,		0xf1d80000, 0xe0580000 },	// ROR.w	1110 xxx0 01x1 1xxx
	{ Cmd_ASR,		0xf1d80000, 0xe0800000 },	// ASR.l	1110 xxx0 10x0 0xxx
	{ Cmd_LSR,		0xf1d80000, 0xe0880000 },	// LSR.l	1110 xxx0 10x0 1xxx
	{ Cmd_ROXR,		0xf1d80000, 0xe0900000 },	// ROXR.l	1110 xxx0 10x1 0xxx
	{ Cmd_ROR,		0xf1d80000, 0xe0980000 },	// ROR.l	1110 xxx0 10x1 1xxx
	{ Cmd_ASR2,		0xffc00000, 0xe0c00000 },	// ASR		1110 0000 11xx xxxx
	{ Cmd_LSR2,		0xffc00000, 0xe2c00000 },	// LSR		1110 0010 11xx xxxx
	{ Cmd_ROXR2,	0xffc00000, 0xe4c00000 },	// ROXR		1110 0100 11xx xxxx
	{ Cmd_ROR2,		0xffc00000, 0xe6c00000 },	// ROR		1110 0110 11xx xxxx
	{ Cmd_ASL,		0xf1d80000, 0xe1000000 },	// ASL.b	1110 xxx1 00x0 0xxx
	{ Cmd_LSL,		0xf1d80000, 0xe1080000 },	// LSL.b	1110 xxx1 00x0 1xxx
	{ Cmd_ROXL,		0xf1d80000, 0xe1100000 },	// ROXL.b	1110 xxx1 00x1 0xxx
	{ Cmd_ROL,		0xf1d80000, 0xe1180000 },	// ROL.b	1110 xxx1 00x1 1xxx
	{ Cmd_ASL,		0xf1d80000, 0xe1400000 },	// ASL.w	1110 xxx1 01x0 0xxx
	{ Cmd_LSL,		0xf1d80000, 0xe1480000 },	// LSL.w	1110 xxx1 01x0 1xxx
	{ Cmd_ROXL,		0xf1d80000, 0xe1500000 },	// ROXL.w	1110 xxx1 01x1 0xxx
	{ Cmd_ROL,		0xf1d80000, 0xe1580000 },	// ROL.w	1110 xxx1 01x1 1xxx
	{ Cmd_ASL,		0xf1d80000, 0xe1800000 },	// ASL.l	1110 xxx1 10x0 0xxx
	{ Cmd_LSL,		0xf1d80000, 0xe1880000 },	// LSL.l	1110 xxx1 10x0 1xxx
	{ Cmd_ROXL,		0xf1d80000, 0xe1900000 },	// ROXL.l	1110 xxx1 10x1 0xxx
	{ Cmd_ROL,		0xf1d80000, 0xe1980000 },	// ROL.l	1110 xxx1 10x1 1xxx
	{ Cmd_ASL2,		0xffc00000, 0xe1c00000 },	// ASL		1110 0001 11xx xxxx
	{ Cmd_LSL2,		0xffc00000, 0xe3c00000 },	// LSL		1110 0011 11xx xxxx
	{ Cmd_ROXL2,	0xffc00000, 0xe5c00000 },	// ROXL		1110 0101 11xx xxxx
	{ Cmd_ROL2,		0xffc00000, 0xe7c00000 },	// ROL		1110 0111 11xx xxxx
	{ NULL,			0x00000000, 0x00000000 },
};

static const struct CmdStruct m68kCmds_1111[] =
{

	// F-Reg to F-Reg
	{ Cmd_FMOVE,	0xffffe07f, 0xf2000000 },	// FMove	1111 0010 0000 0000 000x xxxx x000 0000
	{ Cmd_FINT,		0xffffe07f, 0xf2000001 },	// FInt		1111 0010 0000 0000 000x xxxx x000 0001
	{ Cmd_FSINH,	0xffffe07f, 0xf2000002 },	// FSinh	1111 0010 0000 0000 000x xxxx x000 0010
	{ Cmd_FINTRZ,	0xffffe07f, 0xf2000003 },	// FIntrz	1111 0010 0000 0000 000x xxxx x000 0011
	{ Cmd_FSQRT,	0xffffe07f, 0xf2000004 },	// FSqrt	1111 0010 0000 0000 000x xxxx x000 0100
	// 5
	{ Cmd_FLOGNP1,	0xffffe07f, 0xf2000006 },	// FLognp1	1111 0010 0000 0000 000x xxxx x000 0110
	// 7
	{ Cmd_FETOXM1,	0xffffe07f, 0xf2000008 },	// FEtoxm1	1111 0010 0000 0000 000x xxxx x000 1000
	{ Cmd_FTANh,	0xffffe07f, 0xf2000009 },	// FTanh	1111 0010 0000 0000 000x xxxx x000 1001
	{ Cmd_FATAN,	0xffffe07f, 0xf200000a },	// FATan	1111 0010 0000 0000 000x xxxx x000 1010
	{ Cmd_FASIN,	0xffffe07f, 0xf200000c },	// FASin	1111 0010 0000 0000 000x xxxx x000 1100
	{ Cmd_FATANH,	0xffffe07f, 0xf200000d },	// FATanh	1111 0010 0000 0000 000x xxxx x000 1101
	{ Cmd_FSIN,		0xffffe07f, 0xf200000e },	// FSin		1111 0010 0000 0000 000x xxxx x000 1101
	{ Cmd_FTAN,		0xffffe07f, 0xf200000f },	// FTan		1111 0010 0000 0000 000x xxxx x000 1111
	{ Cmd_FETOX,	0xffffe07f, 0xf2000010 },	// FEtox	1111 0010 0000 0000 000x xxxx x001 0000
	{ Cmd_FTWOTOX,	0xffffe07f, 0xf2000011 },	// FTwotox	1111 0010 0000 0000 000x xxxx x001 0001
	{ Cmd_FTENTOX,	0xffffe07f, 0xf2000012 },	// FTentox	1111 0010 0000 0000 000x xxxx x001 0010
	{ Cmd_FLOGN,	0xffffe07f, 0xf2000014 },	// FLogn	1111 0010 0000 0000 000x xxxx x001 0100
	{ Cmd_FLOG10,	0xffffe07f, 0xf2000015 },	// FLog10	1111 0010 0000 0000 000x xxxx x001 0101
	{ Cmd_FLOG2,	0xffffe07f, 0xf2000016 },	// FLog2	1111 0010 0000 0000 000x xxxx x001 0110
	// 17
	{ Cmd_FABS,		0xffffe07f, 0xf2000018 },	// FAbs		1111 0010 0000 0000 000x xxxx x001 1000
	{ Cmd_FCOSH,	0xffffe07f, 0xf2000019 },	// FCosh	1111 0010 0000 0000 000x xxxx x001 1001
	{ Cmd_FNEG,		0xffffe07f, 0xf200001a },	// FNeg		1111 0010 0000 0000 000x xxxx x001 1010
	// 1b
	{ Cmd_FACOS,	0xffffe07f, 0xf200001c },	// FACos	1111 0010 0000 0000 000x xxxx x001 1100
	{ Cmd_FCOS,		0xffffe07f, 0xf200001d },	// FCos		1111 0010 0000 0000 000x xxxx x001 1101
	{ Cmd_FGETEXP,	0xffffe07f, 0xf200001e },	// FGetexp	1111 0010 0000 0000 000x xxxx x001 1110
	{ Cmd_FGETMAN,	0xffffe07f, 0xf200001f },	// FGetman	1111 0010 0000 0000 000x xxxx x001 1111
	{ Cmd_FDIV,		0xffffe07f, 0xf2000020 },	// FDiv		1111 0010 0000 0000 000x xxxx x010 0000
	{ Cmd_FMOD,		0xffffe07f, 0xf2000021 },	// FMod		1111 0010 0000 0000 000x xxxx x010 0001
	{ Cmd_FADD,		0xffffe07f, 0xf2000022 },	// FAdd		1111 0010 0000 0000 000x xxxx x010 0010
	{ Cmd_FMUL,		0xffffe07f, 0xf2000023 },	// FMul		1111 0010 0000 0000 000x xxxx x010 0011
	{ Cmd_FSGLDIV,	0xffffe07f, 0xf2000024 },	// FSgldiv	1111 0010 0000 0000 000x xxxx x010 0100
	{ Cmd_FREM,		0xffffe07f, 0xf2000025 },	// FRem		1111 0010 0000 0000 000x xxxx x010 0101
	{ Cmd_FSCALE,	0xffffe07f, 0xf2000026 },	// FScale	1111 0010 0000 0000 000x xxxx x010 0110
	{ Cmd_FSGLMUL,	0xffffe07f, 0xf2000027 },	// FSglmul	1111 0010 0000 0000 000x xxxx x010 0111
	{ Cmd_FSUB,		0xffffe07f, 0xf2000028 },	// FSub		1111 0010 0000 0000 000x xxxx x010 1000
	{ Cmd_FSINCOS,	0xffffe078, 0xf2000030 },	// FSincos	1111 0010 0000 0000 000x xxxx x011 0000
//	{ Cmd_FSINCOS,	0xffffe07f, 0xf2000031 },	// FSincos	1111 0010 0000 0000 000x xxxx x011 0001
//	{ Cmd_FSINCOS,	0xffffe07f, 0xf2000032 },	// FSincos	1111 0010 0000 0000 000x xxxx x011 0010
//	{ Cmd_FSINCOS,	0xffffe07f, 0xf2000033 },	// FSincos	1111 0010 0000 0000 000x xxxx x011 0011
//	{ Cmd_FSINCOS,	0xffffe07f, 0xf2000034 },	// FSincos	1111 0010 0000 0000 000x xxxx x011 0100
//	{ Cmd_FSINCOS,	0xffffe07f, 0xf2000035 },	// FSincos	1111 0010 0000 0000 000x xxxx x011 0101
//	{ Cmd_FSINCOS,	0xffffe07f, 0xf2000036 },	// FSincos	1111 0010 0000 0000 000x xxxx x011 0110
//	{ Cmd_FSINCOS,	0xffffe07f, 0xf2000037 },	// FSincos	1111 0010 0000 0000 000x xxxx x011 0111
	{ Cmd_FCMP,		0xffffe07f, 0xf2000038 },	// FCmp		1111 0010 0000 0000 000x xxxx x011 1000
	// 39
	{ Cmd_FTST,		0xffffe07f, 0xf200003a },	// FTst		1111 0010 0000 0000 000x xxxx x011 1010
	// 3b
	// 3c
	// 3d
	// 3e
	// 3f
	{ Cmd_FSMOVE,	0xffffe07f, 0xf2000040 },	// FSMove	1111 0010 0000 0000 000x xxxx x100 0000
	{ Cmd_FSSQRT,	0xffffe07f, 0xf2000041 },	// FSSqrt	1111 0010 0000 0000 000x xxxx x100 0001
	// 42
	// 43
	{ Cmd_FDMOVE,	0xffffe07f, 0xf2000044 },	// FDMove	1111 0010 0000 0000 000x xxxx x100 0100
	{ Cmd_FDSQRT,	0xffffe07f, 0xf2000045 },	// FDSqrt	1111 0010 0000 0000 000x xxxx x100 0101
	{ Cmd_FSABS,	0xffffe07f, 0xf2000058 },	// FSAbs	1111 0010 0000 0000 000x xxxx x101 1000
	{ Cmd_FSNEG,	0xffffe07f, 0xf200005a },	// FSNeg	1111 0010 0000 0000 000x xxxx x101 1010
	{ Cmd_FDABS,	0xffffe07f, 0xf200005c },	// FDAbs	1111 0010 0000 0000 000x xxxx x101 1100
	{ Cmd_FDNEG,	0xffffe07f, 0xf200005e },	// FDNeg	1111 0010 0000 0000 000x xxxx x101 1110
	{ Cmd_FSDIV,	0xffffe07f, 0xf2000060 },	// FSDiv	1111 0010 0000 0000 000x xxxx x110 0000
	{ Cmd_FSADD,	0xffffe07f, 0xf2000062 },	// FSAdd	1111 0010 0000 0000 000x xxxx x110 0010
	{ Cmd_FSMUL,	0xffffe07f, 0xf2000063 },	// FSMul	1111 0010 0000 0000 000x xxxx x110 0011
	{ Cmd_FDDIV,	0xffffe07f, 0xf2000064 },	// FDDiv	1111 0010 0000 0000 000x xxxx x110 0100
	{ Cmd_FDMUL,	0xffffe07f, 0xf2000067 },	// FDMul	1111 0010 0000 0000 000x xxxx x110 0111
	{ Cmd_FSSUB,	0xffffe07f, 0xf2000068 },	// FSSub	1111 0010 0000 0000 000x xxxx x110 1000
	{ Cmd_FDADD,	0xffffe07f, 0xf2000066 },	// FDAdd	1111 0010 0000 0000 000x xxxx x110 0110
	{ Cmd_FDSUB,	0xffffe07f, 0xf200006c },	// FDSub	1111 0010 0000 0000 000x xxxx x110 1100


	// EA to F-Reg
	{ Cmd_FMOVE2,	0xffc0fc7f, 0xf2004000 },	// FMove	1111 0010 00xx xxxx 0100 00xx x000 0000
	{ Cmd_FMOVE2,	0xffc0fc7f, 0xf2004400 },	// FMove	1111 0010 00xx xxxx 0100 01xx x000 0000
	{ Cmd_FMOVE2,	0xffc0fc7f, 0xf2004800 },	// FMove	1111 0010 00xx xxxx 0100 10xx x000 0000
	{ Cmd_FMOVE2,	0xffc0fc7f, 0xf2004c00 },	// FMove	1111 0010 00xx xxxx 0100 11xx x000 0000
	{ Cmd_FMOVE2,	0xffc0fc7f, 0xf2005000 },	// FMove	1111 0010 00xx xxxx 0101 00xx x000 0000
	{ Cmd_FMOVE2,	0xffc0fc7f, 0xf2005400 },	// FMove	1111 0010 00xx xxxx 0101 01xx x000 0000
	{ Cmd_FMOVE2,	0xffc0fc7f, 0xf2005800 },	// FMove	1111 0010 00xx xxxx 0101 10xx x000 0000

	{ Cmd_FINT2,	0xffc0fc7f, 0xf2004001 },	// FInt		1111 0010 00xx xxxx 0100 xxxx x000 0001
	{ Cmd_FINT2,	0xffc0fc7f, 0xf2004401 },	// FInt		1111 0010 00xx xxxx 0100 xxxx x000 0001
	{ Cmd_FINT2,	0xffc0fc7f, 0xf2004801 },	// FInt		1111 0010 00xx xxxx 0100 xxxx x000 0001
	{ Cmd_FINT2,	0xffc0fc7f, 0xf2004c01 },	// FInt		1111 0010 00xx xxxx 0100 xxxx x000 0001
	{ Cmd_FINT2,	0xffc0fc7f, 0xf2005001 },	// FInt		1111 0010 00xx xxxx 0101 xxxx x000 0001
	{ Cmd_FINT2,	0xffc0fc7f, 0xf2005401 },	// FInt		1111 0010 00xx xxxx 0101 xxxx x000 0001
	{ Cmd_FINT2,	0xffc0fc7f, 0xf2005801 },	// FInt		1111 0010 00xx xxxx 0101 xxxx x000 0001

	{ Cmd_FSINH2,	0xffc0fc7f, 0xf2004002 },	// FSinh	1111 0010 00xx xxxx 0100 xxxx x000 0010
	{ Cmd_FSINH2,	0xffc0fc7f, 0xf2004402 },	// FSinh	1111 0010 00xx xxxx 0100 xxxx x000 0010
	{ Cmd_FSINH2,	0xffc0fc7f, 0xf2004802 },	// FSinh	1111 0010 00xx xxxx 0100 xxxx x000 0010
	{ Cmd_FSINH2,	0xffc0fc7f, 0xf2004c02 },	// FSinh	1111 0010 00xx xxxx 0100 xxxx x000 0010
	{ Cmd_FSINH2,	0xffc0fc7f, 0xf2005002 },	// FSinh	1111 0010 00xx xxxx 0101 xxxx x000 0010
	{ Cmd_FSINH2,	0xffc0fc7f, 0xf2005402 },	// FSinh	1111 0010 00xx xxxx 0101 xxxx x000 0010
	{ Cmd_FSINH2,	0xffc0fc7f, 0xf2005802 },	// FSinh	1111 0010 00xx xxxx 0101 xxxx x000 0010

	{ Cmd_FINTRZ2,	0xffc0fc7f, 0xf2004003 },	// FIntrz	1111 0010 00xx xxxx 0100 xxxx x000 0011
	{ Cmd_FINTRZ2,	0xffc0fc7f, 0xf2004403 },	// FIntrz	1111 0010 00xx xxxx 0100 xxxx x000 0011
	{ Cmd_FINTRZ2,	0xffc0fc7f, 0xf2004803 },	// FIntrz	1111 0010 00xx xxxx 0100 xxxx x000 0011
	{ Cmd_FINTRZ2,	0xffc0fc7f, 0xf2004c03 },	// FIntrz	1111 0010 00xx xxxx 0100 xxxx x000 0011
	{ Cmd_FINTRZ2,	0xffc0fc7f, 0xf2005003 },	// FIntrz	1111 0010 00xx xxxx 0101 xxxx x000 0011
	{ Cmd_FINTRZ2,	0xffc0fc7f, 0xf2005403 },	// FIntrz	1111 0010 00xx xxxx 0101 xxxx x000 0011
	{ Cmd_FINTRZ2,	0xffc0fc7f, 0xf2005803 },	// FIntrz	1111 0010 00xx xxxx 0101 xxxx x000 0011

	{ Cmd_FSQRT2,	0xffc0fc7f, 0xf2004004 },	// FSqrt	1111 0010 00xx xxxx 0100 xxxx x000 0100
	{ Cmd_FSQRT2,	0xffc0fc7f, 0xf2004404 },	// FSqrt	1111 0010 00xx xxxx 0100 xxxx x000 0100
	{ Cmd_FSQRT2,	0xffc0fc7f, 0xf2004804 },	// FSqrt	1111 0010 00xx xxxx 0100 xxxx x000 0100
	{ Cmd_FSQRT2,	0xffc0fc7f, 0xf2004c04 },	// FSqrt	1111 0010 00xx xxxx 0100 xxxx x000 0100
	{ Cmd_FSQRT2,	0xffc0fc7f, 0xf2005004 },	// FSqrt	1111 0010 00xx xxxx 0101 xxxx x000 0100
	{ Cmd_FSQRT2,	0xffc0fc7f, 0xf2005404 },	// FSqrt	1111 0010 00xx xxxx 0101 xxxx x000 0100
	{ Cmd_FSQRT2,	0xffc0fc7f, 0xf2005804 },	// FSqrt	1111 0010 00xx xxxx 0101 xxxx x000 0100

	{ Cmd_FLOGNP12,	0xffc0fc7f, 0xf2004006 },	// FLognp1	1111 0010 00xx xxxx 0100 xxxx x000 0110
	{ Cmd_FLOGNP12,	0xffc0fc7f, 0xf2004406 },	// FLognp1	1111 0010 00xx xxxx 0100 xxxx x000 0110
	{ Cmd_FLOGNP12,	0xffc0fc7f, 0xf2004806 },	// FLognp1	1111 0010 00xx xxxx 0100 xxxx x000 0110
	{ Cmd_FLOGNP12,	0xffc0fc7f, 0xf2004c06 },	// FLognp1	1111 0010 00xx xxxx 0100 xxxx x000 0110
	{ Cmd_FLOGNP12,	0xffc0fc7f, 0xf2005006 },	// FLognp1	1111 0010 00xx xxxx 0101 xxxx x000 0110
	{ Cmd_FLOGNP12,	0xffc0fc7f, 0xf2005406 },	// FLognp1	1111 0010 00xx xxxx 0101 xxxx x000 0110
	{ Cmd_FLOGNP12,	0xffc0fc7f, 0xf2005806 },	// FLognp1	1111 0010 00xx xxxx 0101 xxxx x000 0110

	{ Cmd_FETOXM12,	0xffc0fc7f, 0xf2004008 },	// FEtoxm1	1111 0010 00xx xxxx 0100 xxxx x000 1000
	{ Cmd_FETOXM12,	0xffc0fc7f, 0xf2004408 },	// FEtoxm1	1111 0010 00xx xxxx 0100 xxxx x000 1000
	{ Cmd_FETOXM12,	0xffc0fc7f, 0xf2004808 },	// FEtoxm1	1111 0010 00xx xxxx 0100 xxxx x000 1000
	{ Cmd_FETOXM12,	0xffc0fc7f, 0xf2004c08 },	// FEtoxm1	1111 0010 00xx xxxx 0100 xxxx x000 1000
	{ Cmd_FETOXM12,	0xffc0fc7f, 0xf2005008 },	// FEtoxm1	1111 0010 00xx xxxx 0101 xxxx x000 1000
	{ Cmd_FETOXM12,	0xffc0fc7f, 0xf2005408 },	// FEtoxm1	1111 0010 00xx xxxx 0101 xxxx x000 1000
	{ Cmd_FETOXM12,	0xffc0fc7f, 0xf2005808 },	// FEtoxm1	1111 0010 00xx xxxx 0101 xxxx x000 1000

	{ Cmd_FTANh2,	0xffc0fc7f, 0xf2004009 },	// FTanh	1111 0010 00xx xxxx 0100 xxxx x000 1001
	{ Cmd_FTANh2,	0xffc0fc7f, 0xf2004409 },	// FTanh	1111 0010 00xx xxxx 0100 xxxx x000 1001
	{ Cmd_FTANh2,	0xffc0fc7f, 0xf2004809 },	// FTanh	1111 0010 00xx xxxx 0100 xxxx x000 1001
	{ Cmd_FTANh2,	0xffc0fc7f, 0xf2004c09 },	// FTanh	1111 0010 00xx xxxx 0100 xxxx x000 1001
	{ Cmd_FTANh2,	0xffc0fc7f, 0xf2005009 },	// FTanh	1111 0010 00xx xxxx 0101 xxxx x000 1001
	{ Cmd_FTANh2,	0xffc0fc7f, 0xf2005409 },	// FTanh	1111 0010 00xx xxxx 0101 xxxx x000 1001
	{ Cmd_FTANh2,	0xffc0fc7f, 0xf2005809 },	// FTanh	1111 0010 00xx xxxx 0101 xxxx x000 1001

	{ Cmd_FATAN2,	0xffc0fc7f, 0xf200400a },	// FATan	1111 0010 00xx xxxx 0100 xxxx x000 1010
	{ Cmd_FATAN2,	0xffc0fc7f, 0xf200440a },	// FATan	1111 0010 00xx xxxx 0100 xxxx x000 1010
	{ Cmd_FATAN2,	0xffc0fc7f, 0xf200480a },	// FATan	1111 0010 00xx xxxx 0100 xxxx x000 1010
	{ Cmd_FATAN2,	0xffc0fc7f, 0xf2004c0a },	// FATan	1111 0010 00xx xxxx 0100 xxxx x000 1010
	{ Cmd_FATAN2,	0xffc0fc7f, 0xf200500a },	// FATan	1111 0010 00xx xxxx 0101 xxxx x000 1010
	{ Cmd_FATAN2,	0xffc0fc7f, 0xf200540a },	// FATan	1111 0010 00xx xxxx 0101 xxxx x000 1010
	{ Cmd_FATAN2,	0xffc0fc7f, 0xf200580a },	// FATan	1111 0010 00xx xxxx 0101 xxxx x000 1010

	{ Cmd_FASIN2,	0xffc0fc7f, 0xf200400c },	// FASin	1111 0010 00xx xxxx 010x xxxx x000 1100
	{ Cmd_FASIN2,	0xffc0fc7f, 0xf200440c },	// FASin	1111 0010 00xx xxxx 010x xxxx x000 1100
	{ Cmd_FASIN2,	0xffc0fc7f, 0xf200480c },	// FASin	1111 0010 00xx xxxx 010x xxxx x000 1100
	{ Cmd_FASIN2,	0xffc0fc7f, 0xf2004c0c },	// FASin	1111 0010 00xx xxxx 010x xxxx x000 1100
	{ Cmd_FASIN2,	0xffc0fc7f, 0xf200500c },	// FASin	1111 0010 00xx xxxx 010x xxxx x000 1100
	{ Cmd_FASIN2,	0xffc0fc7f, 0xf200540c },	// FASin	1111 0010 00xx xxxx 010x xxxx x000 1100
	{ Cmd_FASIN2,	0xffc0fc7f, 0xf200580c },	// FASin	1111 0010 00xx xxxx 010x xxxx x000 1100

	{ Cmd_FATANH2,	0xffc0fc7f, 0xf200400d },	// FATanh	1111 0010 00xx xxxx 010x xxxx x000 1101
	{ Cmd_FATANH2,	0xffc0fc7f, 0xf200440d },	// FATanh	1111 0010 00xx xxxx 010x xxxx x000 1101
	{ Cmd_FATANH2,	0xffc0fc7f, 0xf200480d },	// FATanh	1111 0010 00xx xxxx 010x xxxx x000 1101
	{ Cmd_FATANH2,	0xffc0fc7f, 0xf2004c0d },	// FATanh	1111 0010 00xx xxxx 010x xxxx x000 1101
	{ Cmd_FATANH2,	0xffc0fc7f, 0xf200500d },	// FATanh	1111 0010 00xx xxxx 010x xxxx x000 1101
	{ Cmd_FATANH2,	0xffc0fc7f, 0xf200540d },	// FATanh	1111 0010 00xx xxxx 010x xxxx x000 1101
	{ Cmd_FATANH2,	0xffc0fc7f, 0xf200580d },	// FATanh	1111 0010 00xx xxxx 010x xxxx x000 1101

	{ Cmd_FSIN2,	0xffc0fc7f, 0xf200400e },	// FSin		1111 0010 00xx xxxx 010x xxxx x000 1101
	{ Cmd_FSIN2,	0xffc0fc7f, 0xf200440e },	// FSin		1111 0010 00xx xxxx 010x xxxx x000 1101
	{ Cmd_FSIN2,	0xffc0fc7f, 0xf200480e },	// FSin		1111 0010 00xx xxxx 010x xxxx x000 1101
	{ Cmd_FSIN2,	0xffc0fc7f, 0xf2004c0e },	// FSin		1111 0010 00xx xxxx 010x xxxx x000 1101
	{ Cmd_FSIN2,	0xffc0fc7f, 0xf200500e },	// FSin		1111 0010 00xx xxxx 010x xxxx x000 1101
	{ Cmd_FSIN2,	0xffc0fc7f, 0xf200540e },	// FSin		1111 0010 00xx xxxx 010x xxxx x000 1101
	{ Cmd_FSIN2,	0xffc0fc7f, 0xf200580e },	// FSin		1111 0010 00xx xxxx 010x xxxx x000 1101

	{ Cmd_FTAN2,	0xffc0fc7f, 0xf200400f },	// FTan		1111 0010 00xx xxxx 010x xxxx x000 1111
	{ Cmd_FTAN2,	0xffc0fc7f, 0xf200440f },	// FTan		1111 0010 00xx xxxx 010x xxxx x000 1111
	{ Cmd_FTAN2,	0xffc0fc7f, 0xf200480f },	// FTan		1111 0010 00xx xxxx 010x xxxx x000 1111
	{ Cmd_FTAN2,	0xffc0fc7f, 0xf2004c0f },	// FTan		1111 0010 00xx xxxx 010x xxxx x000 1111
	{ Cmd_FTAN2,	0xffc0fc7f, 0xf200500f },	// FTan		1111 0010 00xx xxxx 010x xxxx x000 1111
	{ Cmd_FTAN2,	0xffc0fc7f, 0xf200540f },	// FTan		1111 0010 00xx xxxx 010x xxxx x000 1111
	{ Cmd_FTAN2,	0xffc0fc7f, 0xf200580f },	// FTan		1111 0010 00xx xxxx 010x xxxx x000 1111

	{ Cmd_FETOX2,	0xffc0fc7f, 0xf2004010 },	// FEtox	1111 0010 00xx xxxx 010x xxxx x001 0000
	{ Cmd_FETOX2,	0xffc0fc7f, 0xf2004410 },	// FEtox	1111 0010 00xx xxxx 010x xxxx x001 0000
	{ Cmd_FETOX2,	0xffc0fc7f, 0xf2004810 },	// FEtox	1111 0010 00xx xxxx 010x xxxx x001 0000
	{ Cmd_FETOX2,	0xffc0fc7f, 0xf2004c10 },	// FEtox	1111 0010 00xx xxxx 010x xxxx x001 0000
	{ Cmd_FETOX2,	0xffc0fc7f, 0xf2005010 },	// FEtox	1111 0010 00xx xxxx 010x xxxx x001 0000
	{ Cmd_FETOX2,	0xffc0fc7f, 0xf2005410 },	// FEtox	1111 0010 00xx xxxx 010x xxxx x001 0000
	{ Cmd_FETOX2,	0xffc0fc7f, 0xf2005810 },	// FEtox	1111 0010 00xx xxxx 010x xxxx x001 0000

	{ Cmd_FTWOTOX2,	0xffc0fc7f, 0xf2004011 },	// FTwotox	1111 0010 00xx xxxx 010x xxxx x001 0001
	{ Cmd_FTWOTOX2,	0xffc0fc7f, 0xf2004411 },	// FTwotox	1111 0010 00xx xxxx 010x xxxx x001 0001
	{ Cmd_FTWOTOX2,	0xffc0fc7f, 0xf2004811 },	// FTwotox	1111 0010 00xx xxxx 010x xxxx x001 0001
	{ Cmd_FTWOTOX2,	0xffc0fc7f, 0xf2004c11 },	// FTwotox	1111 0010 00xx xxxx 010x xxxx x001 0001
	{ Cmd_FTWOTOX2,	0xffc0fc7f, 0xf2005011 },	// FTwotox	1111 0010 00xx xxxx 010x xxxx x001 0001
	{ Cmd_FTWOTOX2,	0xffc0fc7f, 0xf2005411 },	// FTwotox	1111 0010 00xx xxxx 010x xxxx x001 0001
	{ Cmd_FTWOTOX2,	0xffc0fc7f, 0xf2005811 },	// FTwotox	1111 0010 00xx xxxx 010x xxxx x001 0001

	{ Cmd_FTENTOX2,	0xffc0fc7f, 0xf2004012 },	// FTentox	1111 0010 00xx xxxx 010x xxxx x001 0010
	{ Cmd_FTENTOX2,	0xffc0fc7f, 0xf2004412 },	// FTentox	1111 0010 00xx xxxx 010x xxxx x001 0010
	{ Cmd_FTENTOX2,	0xffc0fc7f, 0xf2004812 },	// FTentox	1111 0010 00xx xxxx 010x xxxx x001 0010
	{ Cmd_FTENTOX2,	0xffc0fc7f, 0xf2004c12 },	// FTentox	1111 0010 00xx xxxx 010x xxxx x001 0010
	{ Cmd_FTENTOX2,	0xffc0fc7f, 0xf2005012 },	// FTentox	1111 0010 00xx xxxx 010x xxxx x001 0010
	{ Cmd_FTENTOX2,	0xffc0fc7f, 0xf2005412 },	// FTentox	1111 0010 00xx xxxx 010x xxxx x001 0010
	{ Cmd_FTENTOX2,	0xffc0fc7f, 0xf2005812 },	// FTentox	1111 0010 00xx xxxx 010x xxxx x001 0010

	{ Cmd_FLOGN2,	0xffc0fc7f, 0xf2004014 },	// FLogn	1111 0010 00xx xxxx 010x xxxx x001 0100
	{ Cmd_FLOGN2,	0xffc0fc7f, 0xf2004414 },	// FLogn	1111 0010 00xx xxxx 010x xxxx x001 0100
	{ Cmd_FLOGN2,	0xffc0fc7f, 0xf2004814 },	// FLogn	1111 0010 00xx xxxx 010x xxxx x001 0100
	{ Cmd_FLOGN2,	0xffc0fc7f, 0xf2004c14 },	// FLogn	1111 0010 00xx xxxx 010x xxxx x001 0100
	{ Cmd_FLOGN2,	0xffc0fc7f, 0xf2005014 },	// FLogn	1111 0010 00xx xxxx 010x xxxx x001 0100
	{ Cmd_FLOGN2,	0xffc0fc7f, 0xf2005414 },	// FLogn	1111 0010 00xx xxxx 010x xxxx x001 0100
	{ Cmd_FLOGN2,	0xffc0fc7f, 0xf2005814 },	// FLogn	1111 0010 00xx xxxx 010x xxxx x001 0100

	{ Cmd_FLOGN2,	0xffc0fc7f, 0xf2004014 },	// FLogn	1111 0010 00xx xxxx 010x xxxx x001 0100
	{ Cmd_FLOGN2,	0xffc0fc7f, 0xf2004414 },	// FLogn	1111 0010 00xx xxxx 010x xxxx x001 0100
	{ Cmd_FLOGN2,	0xffc0fc7f, 0xf2004814 },	// FLogn	1111 0010 00xx xxxx 010x xxxx x001 0100
	{ Cmd_FLOGN2,	0xffc0fc7f, 0xf2004c14 },	// FLogn	1111 0010 00xx xxxx 010x xxxx x001 0100
	{ Cmd_FLOGN2,	0xffc0fc7f, 0xf2005014 },	// FLogn	1111 0010 00xx xxxx 010x xxxx x001 0100
	{ Cmd_FLOGN2,	0xffc0fc7f, 0xf2005414 },	// FLogn	1111 0010 00xx xxxx 010x xxxx x001 0100
	{ Cmd_FLOGN2,	0xffc0fc7f, 0xf2005814 },	// FLogn	1111 0010 00xx xxxx 010x xxxx x001 0100

	{ Cmd_FLOG102,	0xffc0fc7f, 0xf2004015 },	// FLog10	1111 0010 00xx xxxx 010x xxxx x001 0101
	{ Cmd_FLOG102,	0xffc0fc7f, 0xf2004415 },	// FLog10	1111 0010 00xx xxxx 010x xxxx x001 0101
	{ Cmd_FLOG102,	0xffc0fc7f, 0xf2004815 },	// FLog10	1111 0010 00xx xxxx 010x xxxx x001 0101
	{ Cmd_FLOG102,	0xffc0fc7f, 0xf2004c15 },	// FLog10	1111 0010 00xx xxxx 010x xxxx x001 0101
	{ Cmd_FLOG102,	0xffc0fc7f, 0xf2005015 },	// FLog10	1111 0010 00xx xxxx 010x xxxx x001 0101
	{ Cmd_FLOG102,	0xffc0fc7f, 0xf2005415 },	// FLog10	1111 0010 00xx xxxx 010x xxxx x001 0101
	{ Cmd_FLOG102,	0xffc0fc7f, 0xf2005815 },	// FLog10	1111 0010 00xx xxxx 010x xxxx x001 0101

	{ Cmd_FLOG22,	0xffc0fc7f, 0xf2004016 },	// FLog2	1111 0010 00xx xxxx 010x xxxx x001 0110
	{ Cmd_FLOG22,	0xffc0fc7f, 0xf2004416 },	// FLog2	1111 0010 00xx xxxx 010x xxxx x001 0110
	{ Cmd_FLOG22,	0xffc0fc7f, 0xf2004816 },	// FLog2	1111 0010 00xx xxxx 010x xxxx x001 0110
	{ Cmd_FLOG22,	0xffc0fc7f, 0xf2004c16 },	// FLog2	1111 0010 00xx xxxx 010x xxxx x001 0110
	{ Cmd_FLOG22,	0xffc0fc7f, 0xf2005016 },	// FLog2	1111 0010 00xx xxxx 010x xxxx x001 0110
	{ Cmd_FLOG22,	0xffc0fc7f, 0xf2005416 },	// FLog2	1111 0010 00xx xxxx 010x xxxx x001 0110
	{ Cmd_FLOG22,	0xffc0fc7f, 0xf2005816 },	// FLog2	1111 0010 00xx xxxx 010x xxxx x001 0110

	{ Cmd_FABS2,	0xffc0fc7f, 0xf2004018 },	// FAbs		1111 0010 00xx xxxx 010x xxxx x001 1000
	{ Cmd_FABS2,	0xffc0fc7f, 0xf2004418 },	// FAbs		1111 0010 00xx xxxx 010x xxxx x001 1000
	{ Cmd_FABS2,	0xffc0fc7f, 0xf2004818 },	// FAbs		1111 0010 00xx xxxx 010x xxxx x001 1000
	{ Cmd_FABS2,	0xffc0fc7f, 0xf2004c18 },	// FAbs		1111 0010 00xx xxxx 010x xxxx x001 1000
	{ Cmd_FABS2,	0xffc0fc7f, 0xf2005018 },	// FAbs		1111 0010 00xx xxxx 010x xxxx x001 1000
	{ Cmd_FABS2,	0xffc0fc7f, 0xf2005418 },	// FAbs		1111 0010 00xx xxxx 010x xxxx x001 1000
	{ Cmd_FABS2,	0xffc0fc7f, 0xf2005818 },	// FAbs		1111 0010 00xx xxxx 010x xxxx x001 1000

	{ Cmd_FCOSH2,	0xffc0fc7f, 0xf2004019 },	// FCosh	1111 0010 00xx xxxx 010x xxxx x001 1001
	{ Cmd_FCOSH2,	0xffc0fc7f, 0xf2004419 },	// FCosh	1111 0010 00xx xxxx 010x xxxx x001 1001
	{ Cmd_FCOSH2,	0xffc0fc7f, 0xf2004819 },	// FCosh	1111 0010 00xx xxxx 010x xxxx x001 1001
	{ Cmd_FCOSH2,	0xffc0fc7f, 0xf2004c19 },	// FCosh	1111 0010 00xx xxxx 010x xxxx x001 1001
	{ Cmd_FCOSH2,	0xffc0fc7f, 0xf2005019 },	// FCosh	1111 0010 00xx xxxx 010x xxxx x001 1001
	{ Cmd_FCOSH2,	0xffc0fc7f, 0xf2005419 },	// FCosh	1111 0010 00xx xxxx 010x xxxx x001 1001
	{ Cmd_FCOSH2,	0xffc0fc7f, 0xf2005819 },	// FCosh	1111 0010 00xx xxxx 010x xxxx x001 1001

	{ Cmd_FNEG2,	0xffc0fc7f, 0xf200401a },	// FNeg		1111 0010 00xx xxxx 010x xxxx x001 1010
	{ Cmd_FNEG2,	0xffc0fc7f, 0xf200441a },	// FNeg		1111 0010 00xx xxxx 010x xxxx x001 1010
	{ Cmd_FNEG2,	0xffc0fc7f, 0xf200481a },	// FNeg		1111 0010 00xx xxxx 010x xxxx x001 1010
	{ Cmd_FNEG2,	0xffc0fc7f, 0xf2004c1a },	// FNeg		1111 0010 00xx xxxx 010x xxxx x001 1010
	{ Cmd_FNEG2,	0xffc0fc7f, 0xf200501a },	// FNeg		1111 0010 00xx xxxx 010x xxxx x001 1010
	{ Cmd_FNEG2,	0xffc0fc7f, 0xf200541a },	// FNeg		1111 0010 00xx xxxx 010x xxxx x001 1010
	{ Cmd_FNEG2,	0xffc0fc7f, 0xf200581a },	// FNeg		1111 0010 00xx xxxx 010x xxxx x001 1010

	{ Cmd_FACOS2,	0xffc0fc7f, 0xf200401c },	// FACos	1111 0010 00xx xxxx 010x xxxx x001 1100
	{ Cmd_FACOS2,	0xffc0fc7f, 0xf200441c },	// FACos	1111 0010 00xx xxxx 010x xxxx x001 1100
	{ Cmd_FACOS2,	0xffc0fc7f, 0xf200481c },	// FACos	1111 0010 00xx xxxx 010x xxxx x001 1100
	{ Cmd_FACOS2,	0xffc0fc7f, 0xf2004c1c },	// FACos	1111 0010 00xx xxxx 010x xxxx x001 1100
	{ Cmd_FACOS2,	0xffc0fc7f, 0xf200501c },	// FACos	1111 0010 00xx xxxx 010x xxxx x001 1100
	{ Cmd_FACOS2,	0xffc0fc7f, 0xf200541c },	// FACos	1111 0010 00xx xxxx 010x xxxx x001 1100
	{ Cmd_FACOS2,	0xffc0fc7f, 0xf200581c },	// FACos	1111 0010 00xx xxxx 010x xxxx x001 1100

	{ Cmd_FCOS2,	0xffc0fc7f, 0xf200401d },	// FCos		1111 0010 00xx xxxx 010x xxxx x001 1101
	{ Cmd_FCOS2,	0xffc0fc7f, 0xf200441d },	// FCos		1111 0010 00xx xxxx 010x xxxx x001 1101
	{ Cmd_FCOS2,	0xffc0fc7f, 0xf200481d },	// FCos		1111 0010 00xx xxxx 010x xxxx x001 1101
	{ Cmd_FCOS2,	0xffc0fc7f, 0xf2004c1d },	// FCos		1111 0010 00xx xxxx 010x xxxx x001 1101
	{ Cmd_FCOS2,	0xffc0fc7f, 0xf200501d },	// FCos		1111 0010 00xx xxxx 010x xxxx x001 1101
	{ Cmd_FCOS2,	0xffc0fc7f, 0xf200541d },	// FCos		1111 0010 00xx xxxx 010x xxxx x001 1101
	{ Cmd_FCOS2,	0xffc0fc7f, 0xf200581d },	// FCos		1111 0010 00xx xxxx 010x xxxx x001 1101

	{ Cmd_FGETEXP2,	0xffc0fc7f, 0xf200401e },	// FGetexp	1111 0010 00xx xxxx 010x xxxx x001 1110
	{ Cmd_FGETEXP2,	0xffc0fc7f, 0xf200441e },	// FGetexp	1111 0010 00xx xxxx 010x xxxx x001 1110
	{ Cmd_FGETEXP2,	0xffc0fc7f, 0xf200481e },	// FGetexp	1111 0010 00xx xxxx 010x xxxx x001 1110
	{ Cmd_FGETEXP2,	0xffc0fc7f, 0xf2004c1e },	// FGetexp	1111 0010 00xx xxxx 010x xxxx x001 1110
	{ Cmd_FGETEXP2,	0xffc0fc7f, 0xf200501e },	// FGetexp	1111 0010 00xx xxxx 010x xxxx x001 1110
	{ Cmd_FGETEXP2,	0xffc0fc7f, 0xf200541e },	// FGetexp	1111 0010 00xx xxxx 010x xxxx x001 1110
	{ Cmd_FGETEXP2,	0xffc0fc7f, 0xf200581e },	// FGetexp	1111 0010 00xx xxxx 010x xxxx x001 1110

	{ Cmd_FGETMAN2,	0xffc0fc7f, 0xf200401f },	// FGetman	1111 0010 00xx xxxx 010x xxxx x001 1111
	{ Cmd_FGETMAN2,	0xffc0fc7f, 0xf200441f },	// FGetman	1111 0010 00xx xxxx 010x xxxx x001 1111
	{ Cmd_FGETMAN2,	0xffc0fc7f, 0xf200481f },	// FGetman	1111 0010 00xx xxxx 010x xxxx x001 1111
	{ Cmd_FGETMAN2,	0xffc0fc7f, 0xf2004c1f },	// FGetman	1111 0010 00xx xxxx 010x xxxx x001 1111
	{ Cmd_FGETMAN2,	0xffc0fc7f, 0xf200501f },	// FGetman	1111 0010 00xx xxxx 010x xxxx x001 1111
	{ Cmd_FGETMAN2,	0xffc0fc7f, 0xf200541f },	// FGetman	1111 0010 00xx xxxx 010x xxxx x001 1111
	{ Cmd_FGETMAN2,	0xffc0fc7f, 0xf200581f },	// FGetman	1111 0010 00xx xxxx 010x xxxx x001 1111

	{ Cmd_FDIV2,	0xffc0fc7f, 0xf2004020 },	// FDiv		1111 0010 00xx xxxx 010x xxxx x010 0000
	{ Cmd_FDIV2,	0xffc0fc7f, 0xf2004420 },	// FDiv		1111 0010 00xx xxxx 010x xxxx x010 0000
	{ Cmd_FDIV2,	0xffc0fc7f, 0xf2004820 },	// FDiv		1111 0010 00xx xxxx 010x xxxx x010 0000
	{ Cmd_FDIV2,	0xffc0fc7f, 0xf2004c20 },	// FDiv		1111 0010 00xx xxxx 010x xxxx x010 0000
	{ Cmd_FDIV2,	0xffc0fc7f, 0xf2005020 },	// FDiv		1111 0010 00xx xxxx 010x xxxx x010 0000
	{ Cmd_FDIV2,	0xffc0fc7f, 0xf2005420 },	// FDiv		1111 0010 00xx xxxx 010x xxxx x010 0000
	{ Cmd_FDIV2,	0xffc0fc7f, 0xf2005820 },	// FDiv		1111 0010 00xx xxxx 010x xxxx x010 0000

	{ Cmd_FMOD2,	0xffc0fc7f, 0xf2004021 },	// FMod		1111 0010 00xx xxxx 010x xxxx x010 0001
	{ Cmd_FMOD2,	0xffc0fc7f, 0xf2004421 },	// FMod		1111 0010 00xx xxxx 010x xxxx x010 0001
	{ Cmd_FMOD2,	0xffc0fc7f, 0xf2004821 },	// FMod		1111 0010 00xx xxxx 010x xxxx x010 0001
	{ Cmd_FMOD2,	0xffc0fc7f, 0xf2004c21 },	// FMod		1111 0010 00xx xxxx 010x xxxx x010 0001
	{ Cmd_FMOD2,	0xffc0fc7f, 0xf2005021 },	// FMod		1111 0010 00xx xxxx 010x xxxx x010 0001
	{ Cmd_FMOD2,	0xffc0fc7f, 0xf2005421 },	// FMod		1111 0010 00xx xxxx 010x xxxx x010 0001
	{ Cmd_FMOD2,	0xffc0fc7f, 0xf2005821 },	// FMod		1111 0010 00xx xxxx 010x xxxx x010 0001

	{ Cmd_FADD2,	0xffc0fc7f, 0xf2004022 },	// FAdd		1111 0010 00xx xxxx 010x xxxx x010 0010
	{ Cmd_FADD2,	0xffc0fc7f, 0xf2004422 },	// FAdd		1111 0010 00xx xxxx 010x xxxx x010 0010
	{ Cmd_FADD2,	0xffc0fc7f, 0xf2004822 },	// FAdd		1111 0010 00xx xxxx 010x xxxx x010 0010
	{ Cmd_FADD2,	0xffc0fc7f, 0xf2004c22 },	// FAdd		1111 0010 00xx xxxx 010x xxxx x010 0010
	{ Cmd_FADD2,	0xffc0fc7f, 0xf2005022 },	// FAdd		1111 0010 00xx xxxx 010x xxxx x010 0010
	{ Cmd_FADD2,	0xffc0fc7f, 0xf2005422 },	// FAdd		1111 0010 00xx xxxx 010x xxxx x010 0010
	{ Cmd_FADD2,	0xffc0fc7f, 0xf2005822 },	// FAdd		1111 0010 00xx xxxx 010x xxxx x010 0010

	{ Cmd_FMUL2,	0xffc0fc7f, 0xf2004023 },	// FMul		1111 0010 00xx xxxx 010x xxxx x010 0011
	{ Cmd_FMUL2,	0xffc0fc7f, 0xf2004423 },	// FMul		1111 0010 00xx xxxx 010x xxxx x010 0011
	{ Cmd_FMUL2,	0xffc0fc7f, 0xf2004823 },	// FMul		1111 0010 00xx xxxx 010x xxxx x010 0011
	{ Cmd_FMUL2,	0xffc0fc7f, 0xf2004c23 },	// FMul		1111 0010 00xx xxxx 010x xxxx x010 0011
	{ Cmd_FMUL2,	0xffc0fc7f, 0xf2005023 },	// FMul		1111 0010 00xx xxxx 010x xxxx x010 0011
	{ Cmd_FMUL2,	0xffc0fc7f, 0xf2005423 },	// FMul		1111 0010 00xx xxxx 010x xxxx x010 0011
	{ Cmd_FMUL2,	0xffc0fc7f, 0xf2005823 },	// FMul		1111 0010 00xx xxxx 010x xxxx x010 0011

	{ Cmd_FSGLDIV2,	0xffc0fc7f, 0xf2004024 },	// FSgldiv	1111 0010 00xx xxxx 010x xxxx x010 0100
	{ Cmd_FSGLDIV2,	0xffc0fc7f, 0xf2004424 },	// FSgldiv	1111 0010 00xx xxxx 010x xxxx x010 0100
	{ Cmd_FSGLDIV2,	0xffc0fc7f, 0xf2004824 },	// FSgldiv	1111 0010 00xx xxxx 010x xxxx x010 0100
	{ Cmd_FSGLDIV2,	0xffc0fc7f, 0xf2004c24 },	// FSgldiv	1111 0010 00xx xxxx 010x xxxx x010 0100
	{ Cmd_FSGLDIV2,	0xffc0fc7f, 0xf2005024 },	// FSgldiv	1111 0010 00xx xxxx 010x xxxx x010 0100
	{ Cmd_FSGLDIV2,	0xffc0fc7f, 0xf2005424 },	// FSgldiv	1111 0010 00xx xxxx 010x xxxx x010 0100
	{ Cmd_FSGLDIV2,	0xffc0fc7f, 0xf2005824 },	// FSgldiv	1111 0010 00xx xxxx 010x xxxx x010 0100

	{ Cmd_FREM2,	0xffc0fc7f, 0xf2004025 },	// FRem		1111 0010 00xx xxxx 010x xxxx x010 0101
	{ Cmd_FREM2,	0xffc0fc7f, 0xf2004425 },	// FRem		1111 0010 00xx xxxx 010x xxxx x010 0101
	{ Cmd_FREM2,	0xffc0fc7f, 0xf2004825 },	// FRem		1111 0010 00xx xxxx 010x xxxx x010 0101
	{ Cmd_FREM2,	0xffc0fc7f, 0xf2004c25 },	// FRem		1111 0010 00xx xxxx 010x xxxx x010 0101
	{ Cmd_FREM2,	0xffc0fc7f, 0xf2005025 },	// FRem		1111 0010 00xx xxxx 010x xxxx x010 0101
	{ Cmd_FREM2,	0xffc0fc7f, 0xf2005425 },	// FRem		1111 0010 00xx xxxx 010x xxxx x010 0101
	{ Cmd_FREM2,	0xffc0fc7f, 0xf2005825 },	// FRem		1111 0010 00xx xxxx 010x xxxx x010 0101

	{ Cmd_FSCALE2,	0xffc0fc7f, 0xf2004026 },	// FScale	1111 0010 00xx xxxx 010x xxxx x010 0110
	{ Cmd_FSCALE2,	0xffc0fc7f, 0xf2004426 },	// FScale	1111 0010 00xx xxxx 010x xxxx x010 0110
	{ Cmd_FSCALE2,	0xffc0fc7f, 0xf2004826 },	// FScale	1111 0010 00xx xxxx 010x xxxx x010 0110
	{ Cmd_FSCALE2,	0xffc0fc7f, 0xf2004c26 },	// FScale	1111 0010 00xx xxxx 010x xxxx x010 0110
	{ Cmd_FSCALE2,	0xffc0fc7f, 0xf2005026 },	// FScale	1111 0010 00xx xxxx 010x xxxx x010 0110
	{ Cmd_FSCALE2,	0xffc0fc7f, 0xf2005426 },	// FScale	1111 0010 00xx xxxx 010x xxxx x010 0110
	{ Cmd_FSCALE2,	0xffc0fc7f, 0xf2005826 },	// FScale	1111 0010 00xx xxxx 010x xxxx x010 0110

	{ Cmd_FSGLMUL2,	0xffc0fc7f, 0xf2004027 },	// FSglmul	1111 0010 00xx xxxx 010x xxxx x010 0111
	{ Cmd_FSGLMUL2,	0xffc0fc7f, 0xf2004427 },	// FSglmul	1111 0010 00xx xxxx 010x xxxx x010 0111
	{ Cmd_FSGLMUL2,	0xffc0fc7f, 0xf2004827 },	// FSglmul	1111 0010 00xx xxxx 010x xxxx x010 0111
	{ Cmd_FSGLMUL2,	0xffc0fc7f, 0xf2004c27 },	// FSglmul	1111 0010 00xx xxxx 010x xxxx x010 0111
	{ Cmd_FSGLMUL2,	0xffc0fc7f, 0xf2005027 },	// FSglmul	1111 0010 00xx xxxx 010x xxxx x010 0111
	{ Cmd_FSGLMUL2,	0xffc0fc7f, 0xf2005427 },	// FSglmul	1111 0010 00xx xxxx 010x xxxx x010 0111
	{ Cmd_FSGLMUL2,	0xffc0fc7f, 0xf2005827 },	// FSglmul	1111 0010 00xx xxxx 010x xxxx x010 0111

	{ Cmd_FSUB2,	0xffc0fc7f, 0xf2004028 },	// FSub		1111 0010 00xx xxxx 010x xxxx x010 0000
	{ Cmd_FSUB2,	0xffc0fc7f, 0xf2004428 },	// FSub		1111 0010 00xx xxxx 010x xxxx x010 0000
	{ Cmd_FSUB2,	0xffc0fc7f, 0xf2004828 },	// FSub		1111 0010 00xx xxxx 010x xxxx x010 0000
	{ Cmd_FSUB2,	0xffc0fc7f, 0xf2004c28 },	// FSub		1111 0010 00xx xxxx 010x xxxx x010 0000
	{ Cmd_FSUB2,	0xffc0fc7f, 0xf2005028 },	// FSub		1111 0010 00xx xxxx 010x xxxx x010 0000
	{ Cmd_FSUB2,	0xffc0fc7f, 0xf2005428 },	// FSub		1111 0010 00xx xxxx 010x xxxx x010 0000
	{ Cmd_FSUB2,	0xffc0fc7f, 0xf2005828 },	// FSub		1111 0010 00xx xxxx 010x xxxx x010 0000

	{ Cmd_FSINCOS2,	0xffc0fc78, 0xf2004030 },	// FSincos	1111 0010 00xx xxxx 010x xxxx x011 0000
	{ Cmd_FSINCOS2,	0xffc0fc78, 0xf2004430 },	// FSincos	1111 0010 00xx xxxx 010x xxxx x011 0000
	{ Cmd_FSINCOS2,	0xffc0fc78, 0xf2004830 },	// FSincos	1111 0010 00xx xxxx 010x xxxx x011 0000
	{ Cmd_FSINCOS2,	0xffc0fc78, 0xf2004c30 },	// FSincos	1111 0010 00xx xxxx 010x xxxx x011 0000
	{ Cmd_FSINCOS2,	0xffc0fc78, 0xf2005030 },	// FSincos	1111 0010 00xx xxxx 010x xxxx x011 0000
	{ Cmd_FSINCOS2,	0xffc0fc78, 0xf2005430 },	// FSincos	1111 0010 00xx xxxx 010x xxxx x011 0000
	{ Cmd_FSINCOS2,	0xffc0fc78, 0xf2005830 },	// FSincos	1111 0010 00xx xxxx 010x xxxx x011 0000

	{ Cmd_FCMP2,	0xffc0fc7f, 0xf2004038 },	// FCmp		1111 0010 00xx xxxx 010x xxxx x011 1000
	{ Cmd_FCMP2,	0xffc0fc7f, 0xf2004438 },	// FCmp		1111 0010 00xx xxxx 010x xxxx x011 1000
	{ Cmd_FCMP2,	0xffc0fc7f, 0xf2004838 },	// FCmp		1111 0010 00xx xxxx 010x xxxx x011 1000
	{ Cmd_FCMP2,	0xffc0fc7f, 0xf2004c38 },	// FCmp		1111 0010 00xx xxxx 010x xxxx x011 1000
	{ Cmd_FCMP2,	0xffc0fc7f, 0xf2005038 },	// FCmp		1111 0010 00xx xxxx 010x xxxx x011 1000
	{ Cmd_FCMP2,	0xffc0fc7f, 0xf2005438 },	// FCmp		1111 0010 00xx xxxx 010x xxxx x011 1000
	{ Cmd_FCMP2,	0xffc0fc7f, 0xf2005838 },	// FCmp		1111 0010 00xx xxxx 010x xxxx x011 1000

	{ Cmd_FTST2,	0xffc0fc7f, 0xf200403a },	// FTst		1111 0010 00xx xxxx 010x xxxx x011 1010
	{ Cmd_FTST2,	0xffc0fc7f, 0xf200443a },	// FTst		1111 0010 00xx xxxx 010x xxxx x011 1010
	{ Cmd_FTST2,	0xffc0fc7f, 0xf200483a },	// FTst		1111 0010 00xx xxxx 010x xxxx x011 1010
	{ Cmd_FTST2,	0xffc0fc7f, 0xf2004c3a },	// FTst		1111 0010 00xx xxxx 010x xxxx x011 1010
	{ Cmd_FTST2,	0xffc0fc7f, 0xf200503a },	// FTst		1111 0010 00xx xxxx 010x xxxx x011 1010
	{ Cmd_FTST2,	0xffc0fc7f, 0xf200543a },	// FTst		1111 0010 00xx xxxx 010x xxxx x011 1010
	{ Cmd_FTST2,	0xffc0fc7f, 0xf200583a },	// FTst		1111 0010 00xx xxxx 010x xxxx x011 1010

	{ Cmd_FSMOVE2,	0xffc0fc7f, 0xf2004040 },	// FSMove	1111 0010 00xx xxxx 010x xxxx x100 0000
	{ Cmd_FSMOVE2,	0xffc0fc7f, 0xf2004440 },	// FSMove	1111 0010 00xx xxxx 010x xxxx x100 0000
	{ Cmd_FSMOVE2,	0xffc0fc7f, 0xf2004840 },	// FSMove	1111 0010 00xx xxxx 010x xxxx x100 0000
	{ Cmd_FSMOVE2,	0xffc0fc7f, 0xf2004c40 },	// FSMove	1111 0010 00xx xxxx 010x xxxx x100 0000
	{ Cmd_FSMOVE2,	0xffc0fc7f, 0xf2005040 },	// FSMove	1111 0010 00xx xxxx 010x xxxx x100 0000
	{ Cmd_FSMOVE2,	0xffc0fc7f, 0xf2005440 },	// FSMove	1111 0010 00xx xxxx 010x xxxx x100 0000
	{ Cmd_FSMOVE2,	0xffc0fc7f, 0xf2005840 },	// FSMove	1111 0010 00xx xxxx 010x xxxx x100 0000

	{ Cmd_FSSQRT2,	0xffc0fc7f, 0xf2004041 },	// FSSqrt	1111 0010 00xx xxxx 010x xxxx x100 0001
	{ Cmd_FSSQRT2,	0xffc0fc7f, 0xf2004441 },	// FSSqrt	1111 0010 00xx xxxx 010x xxxx x100 0001
	{ Cmd_FSSQRT2,	0xffc0fc7f, 0xf2004841 },	// FSSqrt	1111 0010 00xx xxxx 010x xxxx x100 0001
	{ Cmd_FSSQRT2,	0xffc0fc7f, 0xf2004c41 },	// FSSqrt	1111 0010 00xx xxxx 010x xxxx x100 0001
	{ Cmd_FSSQRT2,	0xffc0fc7f, 0xf2005041 },	// FSSqrt	1111 0010 00xx xxxx 010x xxxx x100 0001
	{ Cmd_FSSQRT2,	0xffc0fc7f, 0xf2005441 },	// FSSqrt	1111 0010 00xx xxxx 010x xxxx x100 0001
	{ Cmd_FSSQRT2,	0xffc0fc7f, 0xf2005841 },	// FSSqrt	1111 0010 00xx xxxx 010x xxxx x100 0001

	{ Cmd_FDMOVE2,	0xffc0fc7f, 0xf2004044 },	// FDMove	1111 0010 00xx xxxx 010x xxxx x100 0100
	{ Cmd_FDMOVE2,	0xffc0fc7f, 0xf2004444 },	// FDMove	1111 0010 00xx xxxx 010x xxxx x100 0100
	{ Cmd_FDMOVE2,	0xffc0fc7f, 0xf2004844 },	// FDMove	1111 0010 00xx xxxx 010x xxxx x100 0100
	{ Cmd_FDMOVE2,	0xffc0fc7f, 0xf2004c44 },	// FDMove	1111 0010 00xx xxxx 010x xxxx x100 0100
	{ Cmd_FDMOVE2,	0xffc0fc7f, 0xf2005044 },	// FDMove	1111 0010 00xx xxxx 010x xxxx x100 0100
	{ Cmd_FDMOVE2,	0xffc0fc7f, 0xf2005444 },	// FDMove	1111 0010 00xx xxxx 010x xxxx x100 0100
	{ Cmd_FDMOVE2,	0xffc0fc7f, 0xf2005844 },	// FDMove	1111 0010 00xx xxxx 010x xxxx x100 0100

	{ Cmd_FDSQRT2,	0xffc0fc7f, 0xf2004045 },	// FDSqrt	1111 0010 00xx xxxx 010x xxxx x100 0101
	{ Cmd_FDSQRT2,	0xffc0fc7f, 0xf2004445 },	// FDSqrt	1111 0010 00xx xxxx 010x xxxx x100 0101
	{ Cmd_FDSQRT2,	0xffc0fc7f, 0xf2004845 },	// FDSqrt	1111 0010 00xx xxxx 010x xxxx x100 0101
	{ Cmd_FDSQRT2,	0xffc0fc7f, 0xf2004c45 },	// FDSqrt	1111 0010 00xx xxxx 010x xxxx x100 0101
	{ Cmd_FDSQRT2,	0xffc0fc7f, 0xf2005045 },	// FDSqrt	1111 0010 00xx xxxx 010x xxxx x100 0101
	{ Cmd_FDSQRT2,	0xffc0fc7f, 0xf2005445 },	// FDSqrt	1111 0010 00xx xxxx 010x xxxx x100 0101
	{ Cmd_FDSQRT2,	0xffc0fc7f, 0xf2005845 },	// FDSqrt	1111 0010 00xx xxxx 010x xxxx x100 0101

	{ Cmd_FSABS2,	0xffc0fc7f, 0xf2004058 },	// FSAbs	1111 0010 00xx xxxx 010x xxxx x101 1000
	{ Cmd_FSABS2,	0xffc0fc7f, 0xf2004458 },	// FSAbs	1111 0010 00xx xxxx 010x xxxx x101 1000
	{ Cmd_FSABS2,	0xffc0fc7f, 0xf2004858 },	// FSAbs	1111 0010 00xx xxxx 010x xxxx x101 1000
	{ Cmd_FSABS2,	0xffc0fc7f, 0xf2004c58 },	// FSAbs	1111 0010 00xx xxxx 010x xxxx x101 1000
	{ Cmd_FSABS2,	0xffc0fc7f, 0xf2005058 },	// FSAbs	1111 0010 00xx xxxx 010x xxxx x101 1000
	{ Cmd_FSABS2,	0xffc0fc7f, 0xf2005458 },	// FSAbs	1111 0010 00xx xxxx 010x xxxx x101 1000
	{ Cmd_FSABS2,	0xffc0fc7f, 0xf2005858 },	// FSAbs	1111 0010 00xx xxxx 010x xxxx x101 1000

	{ Cmd_FSNEG2,	0xffc0fc7f, 0xf200405a },	// FSNeg	1111 0010 00xx xxxx 010x xxxx x101 1010
	{ Cmd_FSNEG2,	0xffc0fc7f, 0xf200445a },	// FSNeg	1111 0010 00xx xxxx 010x xxxx x101 1010
	{ Cmd_FSNEG2,	0xffc0fc7f, 0xf200485a },	// FSNeg	1111 0010 00xx xxxx 010x xxxx x101 1010
	{ Cmd_FSNEG2,	0xffc0fc7f, 0xf2004c5a },	// FSNeg	1111 0010 00xx xxxx 010x xxxx x101 1010
	{ Cmd_FSNEG2,	0xffc0fc7f, 0xf200505a },	// FSNeg	1111 0010 00xx xxxx 010x xxxx x101 1010
	{ Cmd_FSNEG2,	0xffc0fc7f, 0xf200545a },	// FSNeg	1111 0010 00xx xxxx 010x xxxx x101 1010
	{ Cmd_FSNEG2,	0xffc0fc7f, 0xf200585a },	// FSNeg	1111 0010 00xx xxxx 010x xxxx x101 1010

	{ Cmd_FDABS2,	0xffc0fc7f, 0xf200405c },	// FDAbs	1111 0010 00xx xxxx 010x xxxx x101 1100
	{ Cmd_FDABS2,	0xffc0fc7f, 0xf200445c },	// FDAbs	1111 0010 00xx xxxx 010x xxxx x101 1100
	{ Cmd_FDABS2,	0xffc0fc7f, 0xf200485c },	// FDAbs	1111 0010 00xx xxxx 010x xxxx x101 1100
	{ Cmd_FDABS2,	0xffc0fc7f, 0xf2004c5c },	// FDAbs	1111 0010 00xx xxxx 010x xxxx x101 1100
	{ Cmd_FDABS2,	0xffc0fc7f, 0xf200505c },	// FDAbs	1111 0010 00xx xxxx 010x xxxx x101 1100
	{ Cmd_FDABS2,	0xffc0fc7f, 0xf200545c },	// FDAbs	1111 0010 00xx xxxx 010x xxxx x101 1100
	{ Cmd_FDABS2,	0xffc0fc7f, 0xf200585c },	// FDAbs	1111 0010 00xx xxxx 010x xxxx x101 1100

	{ Cmd_FDNEG2,	0xffc0fc7f, 0xf200405e },	// FDNeg	1111 0010 00xx xxxx 010x xxxx x101 1110
	{ Cmd_FDNEG2,	0xffc0fc7f, 0xf200445e },	// FDNeg	1111 0010 00xx xxxx 010x xxxx x101 1110
	{ Cmd_FDNEG2,	0xffc0fc7f, 0xf200485e },	// FDNeg	1111 0010 00xx xxxx 010x xxxx x101 1110
	{ Cmd_FDNEG2,	0xffc0fc7f, 0xf2004c5e },	// FDNeg	1111 0010 00xx xxxx 010x xxxx x101 1110
	{ Cmd_FDNEG2,	0xffc0fc7f, 0xf200505e },	// FDNeg	1111 0010 00xx xxxx 010x xxxx x101 1110
	{ Cmd_FDNEG2,	0xffc0fc7f, 0xf200545e },	// FDNeg	1111 0010 00xx xxxx 010x xxxx x101 1110
	{ Cmd_FDNEG2,	0xffc0fc7f, 0xf200585e },	// FDNeg	1111 0010 00xx xxxx 010x xxxx x101 1110

	{ Cmd_FSDIV2,	0xffc0fc7f, 0xf2004060 },	// FSDiv	1111 0010 00xx xxxx 010x xxxx x110 0000
	{ Cmd_FSDIV2,	0xffc0fc7f, 0xf2004460 },	// FSDiv	1111 0010 00xx xxxx 010x xxxx x110 0000
	{ Cmd_FSDIV2,	0xffc0fc7f, 0xf2004860 },	// FSDiv	1111 0010 00xx xxxx 010x xxxx x110 0000
	{ Cmd_FSDIV2,	0xffc0fc7f, 0xf2004c60 },	// FSDiv	1111 0010 00xx xxxx 010x xxxx x110 0000
	{ Cmd_FSDIV2,	0xffc0fc7f, 0xf2005060 },	// FSDiv	1111 0010 00xx xxxx 010x xxxx x110 0000
	{ Cmd_FSDIV2,	0xffc0fc7f, 0xf2005460 },	// FSDiv	1111 0010 00xx xxxx 010x xxxx x110 0000
	{ Cmd_FSDIV2,	0xffc0fc7f, 0xf2005860 },	// FSDiv	1111 0010 00xx xxxx 010x xxxx x110 0000

	{ Cmd_FSADD2,	0xffc0fc7f, 0xf2004062 },	// FSAdd	1111 0010 00xx xxxx 010x xxxx x110 0010
	{ Cmd_FSADD2,	0xffc0fc7f, 0xf2004462 },	// FSAdd	1111 0010 00xx xxxx 010x xxxx x110 0010
	{ Cmd_FSADD2,	0xffc0fc7f, 0xf2004862 },	// FSAdd	1111 0010 00xx xxxx 010x xxxx x110 0010
	{ Cmd_FSADD2,	0xffc0fc7f, 0xf2004c62 },	// FSAdd	1111 0010 00xx xxxx 010x xxxx x110 0010
	{ Cmd_FSADD2,	0xffc0fc7f, 0xf2005062 },	// FSAdd	1111 0010 00xx xxxx 010x xxxx x110 0010
	{ Cmd_FSADD2,	0xffc0fc7f, 0xf2005462 },	// FSAdd	1111 0010 00xx xxxx 010x xxxx x110 0010
	{ Cmd_FSADD2,	0xffc0fc7f, 0xf2005862 },	// FSAdd	1111 0010 00xx xxxx 010x xxxx x110 0010

	{ Cmd_FSMUL2,	0xffc0fc7f, 0xf2004063 },	// FSMul	1111 0010 00xx xxxx 010x xxxx x110 0011
	{ Cmd_FSMUL2,	0xffc0fc7f, 0xf2004463 },	// FSMul	1111 0010 00xx xxxx 010x xxxx x110 0011
	{ Cmd_FSMUL2,	0xffc0fc7f, 0xf2004863 },	// FSMul	1111 0010 00xx xxxx 010x xxxx x110 0011
	{ Cmd_FSMUL2,	0xffc0fc7f, 0xf2004c63 },	// FSMul	1111 0010 00xx xxxx 010x xxxx x110 0011
	{ Cmd_FSMUL2,	0xffc0fc7f, 0xf2005063 },	// FSMul	1111 0010 00xx xxxx 010x xxxx x110 0011
	{ Cmd_FSMUL2,	0xffc0fc7f, 0xf2005463 },	// FSMul	1111 0010 00xx xxxx 010x xxxx x110 0011
	{ Cmd_FSMUL2,	0xffc0fc7f, 0xf2005863 },	// FSMul	1111 0010 00xx xxxx 010x xxxx x110 0011

	{ Cmd_FDDIV2,	0xffc0fc7f, 0xf2004064 },	// FDDiv	1111 0010 00xx xxxx 010x xxxx x110 0100
	{ Cmd_FDDIV2,	0xffc0fc7f, 0xf2004464 },	// FDDiv	1111 0010 00xx xxxx 010x xxxx x110 0100
	{ Cmd_FDDIV2,	0xffc0fc7f, 0xf2004864 },	// FDDiv	1111 0010 00xx xxxx 010x xxxx x110 0100
	{ Cmd_FDDIV2,	0xffc0fc7f, 0xf2004c64 },	// FDDiv	1111 0010 00xx xxxx 010x xxxx x110 0100
	{ Cmd_FDDIV2,	0xffc0fc7f, 0xf2005064 },	// FDDiv	1111 0010 00xx xxxx 010x xxxx x110 0100
	{ Cmd_FDDIV2,	0xffc0fc7f, 0xf2005464 },	// FDDiv	1111 0010 00xx xxxx 010x xxxx x110 0100
	{ Cmd_FDDIV2,	0xffc0fc7f, 0xf2005864 },	// FDDiv	1111 0010 00xx xxxx 010x xxxx x110 0100

	{ Cmd_FDMUL2,	0xffc0fc7f, 0xf2004067 },	// FDMul	1111 0010 00xx xxxx 010x xxxx x110 0111
	{ Cmd_FDMUL2,	0xffc0fc7f, 0xf2004467 },	// FDMul	1111 0010 00xx xxxx 010x xxxx x110 0111
	{ Cmd_FDMUL2,	0xffc0fc7f, 0xf2004867 },	// FDMul	1111 0010 00xx xxxx 010x xxxx x110 0111
	{ Cmd_FDMUL2,	0xffc0fc7f, 0xf2004c67 },	// FDMul	1111 0010 00xx xxxx 010x xxxx x110 0111
	{ Cmd_FDMUL2,	0xffc0fc7f, 0xf2005067 },	// FDMul	1111 0010 00xx xxxx 010x xxxx x110 0111
	{ Cmd_FDMUL2,	0xffc0fc7f, 0xf2005467 },	// FDMul	1111 0010 00xx xxxx 010x xxxx x110 0111
	{ Cmd_FDMUL2,	0xffc0fc7f, 0xf2005867 },	// FDMul	1111 0010 00xx xxxx 010x xxxx x110 0111

	{ Cmd_FSSUB2,	0xffc0fc7f, 0xf2004068 },	// FSSub	1111 0010 00xx xxxx 010x xxxx x110 1000
	{ Cmd_FSSUB2,	0xffc0fc7f, 0xf2004468 },	// FSSub	1111 0010 00xx xxxx 010x xxxx x110 1000
	{ Cmd_FSSUB2,	0xffc0fc7f, 0xf2004868 },	// FSSub	1111 0010 00xx xxxx 010x xxxx x110 1000
	{ Cmd_FSSUB2,	0xffc0fc7f, 0xf2004c68 },	// FSSub	1111 0010 00xx xxxx 010x xxxx x110 1000
	{ Cmd_FSSUB2,	0xffc0fc7f, 0xf2005068 },	// FSSub	1111 0010 00xx xxxx 010x xxxx x110 1000
	{ Cmd_FSSUB2,	0xffc0fc7f, 0xf2005468 },	// FSSub	1111 0010 00xx xxxx 010x xxxx x110 1000
	{ Cmd_FSSUB2,	0xffc0fc7f, 0xf2005868 },	// FSSub	1111 0010 00xx xxxx 010x xxxx x110 1000

	{ Cmd_FDADD2,	0xffc0fc7f, 0xf2004066 },	// FDAdd	1111 0010 00xx xxxx 010x xxxx x110 0110
	{ Cmd_FDADD2,	0xffc0fc7f, 0xf2004466 },	// FDAdd	1111 0010 00xx xxxx 010x xxxx x110 0110
	{ Cmd_FDADD2,	0xffc0fc7f, 0xf2004866 },	// FDAdd	1111 0010 00xx xxxx 010x xxxx x110 0110
	{ Cmd_FDADD2,	0xffc0fc7f, 0xf2004c66 },	// FDAdd	1111 0010 00xx xxxx 010x xxxx x110 0110
	{ Cmd_FDADD2,	0xffc0fc7f, 0xf2005066 },	// FDAdd	1111 0010 00xx xxxx 010x xxxx x110 0110
	{ Cmd_FDADD2,	0xffc0fc7f, 0xf2005466 },	// FDAdd	1111 0010 00xx xxxx 010x xxxx x110 0110
	{ Cmd_FDADD2,	0xffc0fc7f, 0xf2005866 },	// FDAdd	1111 0010 00xx xxxx 010x xxxx x110 0110

	{ Cmd_FDSUB2,	0xffc0fc7f, 0xf200406c },	// FDSub	1111 0010 00xx xxxx 010x xxxx x110 1100
	{ Cmd_FDSUB2,	0xffc0fc7f, 0xf200446c },	// FDSub	1111 0010 00xx xxxx 010x xxxx x110 1100
	{ Cmd_FDSUB2,	0xffc0fc7f, 0xf200486c },	// FDSub	1111 0010 00xx xxxx 010x xxxx x110 1100
	{ Cmd_FDSUB2,	0xffc0fc7f, 0xf2004c6c },	// FDSub	1111 0010 00xx xxxx 010x xxxx x110 1100
	{ Cmd_FDSUB2,	0xffc0fc7f, 0xf200506c },	// FDSub	1111 0010 00xx xxxx 010x xxxx x110 1100
	{ Cmd_FDSUB2,	0xffc0fc7f, 0xf200546c },	// FDSub	1111 0010 00xx xxxx 010x xxxx x110 1100
	{ Cmd_FDSUB2,	0xffc0fc7f, 0xf200586c },	// FDSub	1111 0010 00xx xxxx 010x xxxx x110 1100

	{ Cmd_FMOVECR,	0xfffffc00, 0xf2005c00 },	// FMovecr	1111 0010 0000 0000 0101 11xx xxxx xxxx
	{ Cmd_FMOVE3,	0xffc0e000, 0xf2006000 },	// FMove	1111 0010 00xx xxxx 011x xxxx xxxx xxxx

	{ Cmd_FMOVE4,	0xffc0ffff, 0xf2008400 },	// FMove	1111 0010 00xx xxxx 1000 0100 0000 0000 > ---- ---- FPIAR
	{ Cmd_FMOVE4,	0xffc0ffff, 0xf2008800 },	// FMove	1111 0010 00xx xxxx 1000 1000 0000 0000 > ---- FPSR -----
	{ Cmd_FMOVEM,	0xffc0ffff, 0xf2008c00 },	// FMovem	1111 0010 00xx xxxx 1000 1100 0000 0000 > ---- FPSR FPIAR
	{ Cmd_FMOVE4,	0xffc0ffff, 0xf2009000 },	// FMove	1111 0010 00xx xxxx 1001 0000 0000 0000 > FPCR ---- -----
	{ Cmd_FMOVEM,	0xffc0ffff, 0xf2009400 },	// FMovem	1111 0010 00xx xxxx 1001 0100 0000 0000 > FPCR ---- FPIAR
	{ Cmd_FMOVEM,	0xffc0ffff, 0xf2009800 },	// FMovem	1111 0010 00xx xxxx 1001 1000 0000 0000 > FPCR FPSR -----
	{ Cmd_FMOVEM,	0xffc0ffff, 0xf2009c00 },	// FMovem	1111 0010 00xx xxxx 1001 1100 0000 0000 > FPCR FPSR FPIAR

	{ Cmd_FMOVE4,	0xffc0ffff, 0xf200a400 },	// FMove	1111 0010 00xx xxxx 1010 0100 0000 0000 < ---- ---- FPIAR
	{ Cmd_FMOVE4,	0xffc0ffff, 0xf200a800 },	// FMove	1111 0010 00xx xxxx 1010 1000 0000 0000 < ---- FPSR -----
	{ Cmd_FMOVEM,	0xffc0ffff, 0xf200ac00 },	// FMovem	1111 0010 00xx xxxx 1010 1100 0000 0000 < ---- FPSR FPIAR
	{ Cmd_FMOVEM,	0xffc0ffff, 0xf200b400 },	// FMovem	1111 0010 00xx xxxx 1011 0100 0000 0000 < FPCR ---- -----
	{ Cmd_FMOVE4,	0xffc0ffff, 0xf200b000 },	// FMove	1111 0010 00xx xxxx 1011 0000 0000 0000 < FPCR ---- FPIAR
	{ Cmd_FMOVEM,	0xffc0ffff, 0xf200b800 },	// FMovem	1111 0010 00xx xxxx 1011 1000 0000 0000 < FPCR FPSR -----
	{ Cmd_FMOVEM,	0xffc0ffff, 0xf200bc00 },	// FMovem	1111 0010 00xx xxxx 1011 1100 0000 0000 < FPCR FPSR FPIAR
	{ Cmd_FMOVEM2,	0xffc0ff00, 0xf200c000 },	// FMovem	1111 0010 00xx xxxx 1100 0000 xxxx xxxx
	{ Cmd_FMOVEM2,	0xffc0ff00, 0xf200c800 },	// FMovem	1111 0010 00xx xxxx 1100 1000 xxxx xxxx
	{ Cmd_FMOVEM2,	0xffc0ff00, 0xf200d000 },	// FMovem	1111 0010 00xx xxxx 1101 0000 xxxx xxxx
	{ Cmd_FMOVEM2,	0xffc0ff00, 0xf200d800 },	// FMovem	1111 0010 00xx xxxx 1101 1000 xxxx xxxx
	{ Cmd_FMOVEM2,	0xffc0ff00, 0xf200e000 },	// FMovem	1111 0010 00xx xxxx 1110 0000 xxxx xxxx
	{ Cmd_FMOVEM2,	0xffc0ff00, 0xf200e800 },	// FMovem	1111 0010 00xx xxxx 1110 1000 xxxx xxxx
	{ Cmd_FMOVEM2,	0xffc0ff00, 0xf200f000 },	// FMovem	1111 0010 00xx xxxx 1111 0000 xxxx xxxx
	{ Cmd_FMOVEM2,	0xffc0ff00, 0xf200f800 },	// FMovem	1111 0010 00xx xxxx 1111 1000 xxxx xxxx

 	{ Cmd_FScc,		0xffc0ffc0, 0xf2400000 },	// FScc		1111 0010 001x xxxx 0000 0000 00xx xxxx
 	{ Cmd_FBcc,		0xffe00000, 0xf2800000 },	// FBcc		1111 0010 100x xxxx
 	{ Cmd_FBcc,		0xffe00000, 0xf2c00000 },	// FBcc		1111 0010 110x xxxx

	{ NULL,			0x00000000, 0x00000000 },
};

// --

static const struct CmdStruct *Find_Opcode( struct M68kStruct *ms )
{
const struct CmdStruct *retval;
const struct CmdStruct *list;
uint32_t opcode;
uint32_t cnt;

	retval = NULL;
	opcode = ms->ms_Opcode;

	switch(( opcode & 0xf0000000 ) >> 28 )
	{
		case 0:		list = m68kCmds_0000;	break;
		case 1:		list = m68kCmds_0001;	break;
		case 2:		list = m68kCmds_0010;	break;
		case 3:		list = m68kCmds_0011;	break;
		case 4:		list = m68kCmds_0100;	break;
		case 5:		list = m68kCmds_0101;	break;
		case 6:		list = m68kCmds_0110;	break;
		case 7:		list = m68kCmds_0111;	break;
		case 8:		list = m68kCmds_1000;	break;
		case 9:		list = m68kCmds_1001;	break;
		case 10:	list = m68kCmds_1010;	break;
		case 11:	list = m68kCmds_1011;	break;
		case 12:	list = m68kCmds_1100;	break;
		case 13:	list = m68kCmds_1101;	break;
		case 14:	list = m68kCmds_1110;	break;
		case 15:	list = m68kCmds_1111;	break;

		default:
		{
			goto bailout;
		}
	}

	cnt = 0;

	while( list[cnt].cs_Function != NULL )
	{
		if (( opcode & list[cnt].cs_Mask ) == list[cnt].cs_Opcode )
		{
			break;
		}
		else
		{
			cnt++;
		}
	}

	retval = & list[cnt];

bailout:

	return( retval );
}

// --

void M68k_Decode( struct M68kStruct *ms )
{
const struct CmdStruct *cmd;
uint8_t *mem;

	if (( ms->ms_Buf_Argument == NULL )
	||	( ms->ms_MemoryType == NULL )
	||	( ms->ms_MemoryBuf == NULL )
	||	( ms->ms_HunkStruct == NULL )
	||	( ms->ms_HunkNode == NULL ))
	{
		printf( "%s:%04d: NULL Pointer %p %p %p %p %p\n",
			__FILE__, 
			__LINE__,
			ms->ms_Buf_Argument,
			ms->ms_MemoryType,
			ms->ms_MemoryBuf,
			ms->ms_HunkStruct,
			ms->ms_HunkNode
		);

		ms->ms_DecodeStatus = MSTAT_Error;
		return;
	}

	// --
	// Setup Default Values

	mem = ms->ms_MemoryBuf;

	ms->ms_Opcode = (( mem[0] << 24 ) | ( mem[1] << 16 ) | ( mem[2] << 8 ) | ( mem[3] << 0 ));

	ms->ms_Buf_Argument[0]	= 0;
	ms->ms_Str_Opcode		= NULL;
	ms->ms_JumpTable		= false;
	ms->ms_LastOpcode		= false;
	ms->ms_OpcodeSize		= 2;
	ms->ms_ArgType			= OS_Unsized;
	ms->ms_ArgSize			= 2;
	ms->ms_DecMode			= 0;
	ms->ms_DoExternal		= 1;
	ms->ms_ClearRegMask		= 0;
	ms->ms_LibCall			= 0;
	ms->ms_IsPea			= 0;

	ms->ms_CurRegister		= NULL;
	ms->ms_SrcRegister.mr_Type = RT_Unknown;
	ms->ms_DstRegister.mr_Type = RT_Unknown;
	ms->ms_JmpRegister.mr_Type = RT_Unknown;

	// --

	cmd = Find_Opcode( ms );

	if (( cmd ) && ( cmd->cs_Function ))
	{
		ms->ms_DecodeStatus = MSTAT_Opcode;

		cmd->cs_Function( ms );
	}
	else
	{
		ms->ms_DecodeStatus = MSTAT_UnknownCmd;
	}

	// --
}

// --

enum
{
	_Unknown = 0,
	_Reg,
	_Exec,
	_Label,
};

void Misc_Move_Get( struct M68kStruct *ms, struct M68kRegister *cur, uint8_t *mem, struct Misc_Move_GetSetStruct *gss )
{
struct HunkLabel *hl;
int32_t val;

	memset( gss, 0, sizeof( struct Misc_Move_GetSetStruct ));

	// --

	/**/ if (( ms->ms_ArgEMode == 7 ) && ( ms->ms_ArgEReg == 0 ))	// Move.l $0004.w
	{
		val = (( mem[0] << 8 ) | ( mem[1] << 0 ));

		if ( val == 4 )
		{
			gss->SrcType = _Exec;
		}
	}
	else if (( ms->ms_ArgEMode == 7 ) && ( ms->ms_ArgEReg == 1 ))	// Move.l $00000004
	{
		val = (( mem[0] << 24 ) | ( mem[1] << 16 ) | ( mem[2] << 8 ) | ( mem[3] << 0 ));

		if ( val == 4 )
		{
			gss->SrcType = _Exec;
		}
		else
		{
			hl = Hunk_FindLabel( ms->ms_HunkStruct, val );

			if ( hl )
			{
				gss->SrcType = _Label;
				gss->Label = hl;
			}
		}
	}
	else if ( ms->ms_ArgEMode == 0 )	// Dn Reg
	{
		gss->SrcType = _Reg;
		gss->Reg = *cur;
	}
	else if ( ms->ms_ArgEMode == 1 )	// An Reg
	{
		gss->SrcType = _Reg;
		gss->Reg = *cur;
	}

	// --
}

void Misc_Move_Set( struct M68kStruct *ms, struct M68kRegister *cur, uint8_t *mem, struct Misc_Move_GetSetStruct *gss )
{
struct HunkLabel *src;
struct HunkLabel *hl;
int32_t val;

	// --

	/**/ if (( ms->ms_ArgEMode == 7 ) && ( ms->ms_ArgEReg == 0 ))	// Move.l $0004.w
	{
		// We don't move to External Address
	}
	else if (( ms->ms_ArgEMode == 7 ) && ( ms->ms_ArgEReg == 1 ))	// Move.l $00000004
	{
		val = (( mem[0] << 24 ) | ( mem[1] << 16 ) | ( mem[2] << 8 ) | ( mem[3] << 0 ));

		if ( val != 4 )
		{
			hl = Hunk_FindLabel( ms->ms_HunkStruct, val );
		}
		else
		{
			hl = NULL;
		}

		if (( hl ) && ( ! hl->hl_Label_FixedType ))
		{
			// A Label can only be set once

			switch( gss->SrcType ) // Set xx -> Label
			{
				case _Exec:
				{
					int st = LPT_Library + ( LIBT_ExecBase << 16 );

					if ( hl->hl_Label_Type == LT_Unset )
					{
						hl->hl_Label_Type = LT_Pointer;
						hl->hl_Label_SubType = st;
					}
					else if ( hl->hl_Label_Type == LT_Pointer )
					{
						if ( hl->hl_Label_SubType != st )
						{
							hl->hl_Label_Type = LT_Unknown;
						}
					}
					else
					{
						hl->hl_Label_Type = LT_Unknown;
					}
					break;
				}

				case _Reg:
				{
					// Reg Type
					int rt = ( gss->Reg.mr_Type & 0x0000ffff );
					// Lib SubType
					int st = ( LPT_Library + ( gss->Reg.mr_Type & 0xffff0000 )); 

					if ( hl->hl_Label_Type == LT_Unset )
					{
						if ( rt == RT_Library )
						{
							hl->hl_Label_Type = LT_Pointer;
							hl->hl_Label_SubType = st;
						}
						else
						{

							hl->hl_Label_Type = LT_Unknown;
						}
					}
					else if ( hl->hl_Label_Type == LT_Pointer )
					{
						if ( hl->hl_Label_SubType != st )
						{
							hl->hl_Label_Type = LT_Unknown;
						}
					}
					else
					{
						if ( hl->hl_Label_Type != LT_Unknown )
						{
							hl->hl_Label_Type = LT_Unknown;
						}

					}
					break;
				}

				case _Label:
				{
					src = gss->Label;

					if ( hl->hl_Label_Type == LT_Unset )
					{
						hl->hl_Label_Type = src->hl_Label_Type;
						hl->hl_Label_SubType = src->hl_Label_SubType;
					}
					else if ( hl->hl_Label_Type == LT_Pointer )
					{
						if ( hl->hl_Label_SubType != src->hl_Label_SubType )
						{
							hl->hl_Label_SubType = LPT_Error;
						}
					}
					else
					{
						if ( hl->hl_Label_Type != LT_Unknown )
						{
							hl->hl_Label_Type = LT_Unknown;

						}
					}
					break;
				}

				default:
				{
					if ( hl->hl_Label_Type != LT_Unknown )
					{
						hl->hl_Label_Type = LT_Unknown;

					}
					break;
				}
			}
		}
	}
	else if (( ms->ms_ArgEMode == 0 ) || ( ms->ms_ArgEMode == 1 ))
	{
		switch( gss->SrcType ) // Set xx -> Reg
		{
			case _Reg:
			{
				*cur = gss->Reg;
				break;
			}

			case _Exec:
			{
				cur->mr_Type = RT_Library + ( LIBT_ExecBase << 16 );
				cur->mr_Address = 0;
				cur->mr_LabelNode = NULL;
				break;
			}

			case _Label:
			{
				cur->mr_Type = RT_Unknown;

				hl = gss->Label;

				if ( hl->hl_Label_Type == LT_Pointer )
				{
					cur->mr_Type = RT_Library + ( hl->hl_Label_SubType & 0xffff0000 );
					cur->mr_Address = 0;
					cur->mr_LabelNode = NULL;
				}
				else
				{
					cur->mr_Type = RT_Unknown;
					cur->mr_Address = 0;
					cur->mr_LabelNode = NULL;
				}
				break;
			}

			default:
			{
				cur->mr_Type = RT_Unknown;
				break;
			}
		}
	}
}

// --
