
/*
 * Copyright (c) 2014-2023 Rene W. Olsen <renewolsen@gmail.com>
 * All rights reserved.
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

const char *	Ax_RegNames[8] = { "A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7" };
const char *	Dx_RegNames[8] = { "D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7" };
const char *	scale_Names[4] = { "",   "*2", "*4", "*8" };
//--static const char * FPx_RegNames[]    = { "FP0","FP1","FP2","FP3","FP4","FP5","FP6","FP7" };

// --

struct CmdStruct
{
	void		(*cs_Function)( struct M68kStruct *ms );
	uint32_t	cs_Mask;
	uint32_t	cs_Opcode;
};

// --

/*
** Opcodes (Currently supported)
**
**  Abcd       B      : trace
**  Add        B W L  : trace
**  Adda         W L  : trace
**  Addi       B W L  : trace
**  Addq       B W L  : trace
**  And        B W L  : trace
**  Andi       B W L  : trace
**  Asl        B W L  : trace
**  Asr        B W L  : trace
**  Bcc        B W L  : trace
**  BChg       B   L  : trace
**  BClr       B   L  : trace
**  Bcs        B W L  : trace
**  Beq        B W L  : trace
**  Bge        B W L  : trace
**  BGnd              : trace
**  Bgt        B W L  : trace
**  Bhi        B W L  : trace
**  Bkpt              : trace
**  Ble        B W L  : trace
**  Bls        B W L  : trace
**  Blt        B W L  : trace
**  Bmi        B W L  : trace
**  Bne        B W L  : trace
**  Bpl        B W L  : trace
**  Bra        B W L  : trace
**  BSet       B   L  : trace
**  Bsr        B W L  : trace
**  BTst       B   L  : trace
**  Bvc        B W L  : trace
**  Bvs        B W L  : trace
**  Callm             : trace
**  Clr        B W L  : trace
**  Cmp        B W L  : trace
**  Cmpa         W L  : trace
**  Cmpi       B W L  : trace
**  Cmpm       B W L  : trace
**  DBcc         W    : trace
**  DBcs         W    : trace
**  DBeq         W    : trace
**  DBf          W    : trace
**  DBge         W    : trace
**  DBgt         W    : trace
**  DBhi         W    : trace
**  DBle         W    : trace
**  DBls         W    : trace
**  DBlt         W    : trace
**  DBmi         W    : trace
**  DBne         W    : trace
**  DBpl         W    : trace
**  DBt          W    : trace
**  DBvc         W    : trace
**  DBvs         W    : trace
**  Divs         W L  : trace
**  Divu         W L  : trace
**  Eor        B W L  : trace
**  Eori       B W L  : trace
**  Exg            L  : trace
**  Ext          W L  : trace
**  Extb           L  : trace
**  Illegal           : trace
**  Jmp               : trace
**  Jsr               : trace
**  Lea            L  : trace
**  Link         W L  : trace
**  Lsl        B W L  : trace
**  Lsr        B W L  : trace
**  Move       B W L  :
**  Movea        W L  : trace
**  Movem        W L  : trace
**  Movep        W L  : trace
**  Moveq          L  : trace
**  Muls         W L  : trace
**  Mulu         W L  : trace
**  Nbcd       B      : trace
**  Neg        B W L  : trace
**  Nop               : trace
**  Not        B W L  : trace
**  Or         B W L  : trace
**  Ori        B W L  : trace
**  Pack              : trace
**  Pea            L  : trace
**  Reset             : trace
**  Rol        B W L  : trace
**  Ror        B W L  : trace
**  Roxl       B W L  : trace
**  Roxr       B W L  : trace
**  Rtd               : trace
**  Rte               : trace
**  Rtr               : trace
**  Rts               : trace
**  Sbcd       B      : trace
**  Scc        B      : trace
**  Scs        B      : trace
**  Seq        B      : trace
**  Sf         B      : trace
**  Sge        B      : trace
**  Sgt        B      : trace
**  Shi        B      : trace
**  Sle        B      : trace
**  Sls        B      : trace
**  Slt        B      : trace
**  Smi        B      : trace
**  Sne        B      : trace
**  Spl        B      : trace
**  St         B      : trace
**  Sub        B W L  : trace
**  Suba         W L  : trace
**  Subi       B W L  : trace
**  Subq       B W L  : trace
**  Svc        B      : trace
**  Svs        B      : trace
**  Swap         W    : trace
**  Trap              : trace
**  Trapv             : trace
**  Tst        B W L  : trace
**  Unlk              : trace
**  Unpk              : trace
**
*/

// --

static const struct CmdStruct m68kCmds_0000[] =
{
//  { Cmd_ORI_CCR,  wwwwwwwwww, wwwwwwwwww },   // ORI      0000 0000 0011 1100 0000 0000 xxxx xxxx
//  { Cmd_ORI_SR,   wwwwwwwwww, wwwwwwwwww },   // ORI      0000 0000 0111 1100 xxxx xxxx xxxx xxxx

	{ Cmd_ORI,		0xffc00000, 0x00000000 },	// ORI.b	0000 0000 00xx xxxx
	{ Cmd_ORI,		0xffc00000, 0x00400000 },	// ORI.w	0000 0000 01xx xxxx
	{ Cmd_ORI,		0xffc00000, 0x00800000 },	// ORI.l	0000 0000 10xx xxxx

//  { Cmd_ANDI_CCR, wwwwwwwwww, wwwwwwwwww },   // ANDI     0000 0010 0011 1100 0000 0000 xxxx xxxx
//  { Cmd_ANDI_SR,  wwwwwwwwww, wwwwwwwwww },   // ANDI     0000 0010 0111 1100 xxxx xxxx xxxx xxxx

	{ Cmd_ANDI,		0xffc00000, 0x02000000 },	// ANDI.b	0000 0010 00xx xxxx
	{ Cmd_ANDI,		0xffc00000, 0x02400000 },	// ANDI.w	0000 0010 01xx xxxx
	{ Cmd_ANDI,		0xffc00000, 0x02800000 },	// ANDI.l	0000 0010 10xx xxxx

	{ Cmd_SUBI,		0xffc00000, 0x04000000 },	// SUBI.b	0000 0100 00xx xxxx
	{ Cmd_SUBI,		0xffc00000, 0x04400000 },	// SUBI.w	0000 0100 01xx xxxx
	{ Cmd_SUBI,		0xffc00000, 0x04800000 },	// SUBI.l	0000 0100 10xx xxxx

	{ Cmd_RTM,		0xfff00000, 0x06c00000 },	// RTM		0000 0110 1100 xxxx
	{ Cmd_CALLM,	0xffc0ff00, 0x06c00000 },	// CALLM	0000 0110 11xx xxxx 0000 0000 xxxx xxxx

	{ Cmd_ADDI,		0xffc00000, 0x06000000 },	// ADDI.b	0000 0110 00xx xxxx
	{ Cmd_ADDI,		0xffc00000, 0x06400000 },	// ADDI.w	0000 0110 01xx xxxx
	{ Cmd_ADDI,		0xffc00000, 0x06800000 },	// ADDI.l	0000 0110 10xx xxxx

//  { Cmd_CMP2,     wwwwwwwwww, wwwwwwwwww },   // CMP2     0000 0xx0 11xx xxxx xxxx 0000 0000 0000
//  { Cmd_CHK2,     wwwwwwwwww, wwwwwwwwww },   // CHK2     0000 0xx0 11xx xxxx xxxx 1000 0000 0000

//  { Cmd_EORI_CCR, wwwwwwwwww, wwwwwwwwww },   // EORI     0000 1010 0011 1100 0000 0000 xxxx xxxx
//  { Cmd_EORI_SR,  wwwwwwwwww, wwwwwwwwww },   // EORI     0000 1010 0111 1100 xxxx xxxx xxxx xxxx

	{ Cmd_EORI,		0xffc00000, 0x0a000000 },	// EORI.b	0000 1010 00xx xxxx
	{ Cmd_EORI,		0xffc00000, 0x0a400000 },	// EORI.w	0000 1010 01xx xxxx
	{ Cmd_EORI,		0xffc00000, 0x0a800000 },	// EORI.l	0000 1010 10xx xxxx

	{ Cmd_CMPI,		0xffc00000, 0x0c000000 },	// CMPI.b	0000 1100 00xx xxxx
	{ Cmd_CMPI,		0xffc00000, 0x0c400000 },	// CMPI.w	0000 1100 01xx xxxx
	{ Cmd_CMPI,		0xffc00000, 0x0c800000 },	// CMPI.l	0000 1100 10xx xxxx

	{ Cmd_BTST,		0xffc0ff00, 0x08000000 },	// BTST     0000 1000 00xx xxxx 0000 0000 xxxx xxxx
	{ Cmd_BCHG,		0xffc0ff00, 0x08400000 },	// BCHG     0000 1000 01xx xxxx 0000 0000 xxxx xxxx
	{ Cmd_BCLR,		0xffc0ff00, 0x08800000 },	// BCLR     0000 1000 10xx xxxx 0000 0000 xxxx xxxx
	{ Cmd_BSET,		0xffc0ff00, 0x08c00000 },	// BSET     0000 1000 11xx xxxx 0000 0000 xxxx xxxx

//  { Cmd_MOVES,    wwwwwwwwww, wwwwwwwwww },   // MOVES    0000 1110 xxxx xxxx xxxx x000 0000 0000

//  { Cmd_CAS2,     wwwwwwwwww, wwwwwwwwww },   // CAS2     0000 1xx0 1111 1100 xxxx 000x xx00 0xxx xxxx 000x xx00 0xxxx
//  { Cmd_CAS,      wwwwwwwwww, wwwwwwwwww },   // CAS      0000 1xx0 11xx xxxx 0000 000x xx00 0xxx

	{ Cmd_BTST2,	0xf1c00000, 0x01000000 },	// BTST     0000 xxx1 00xx xxxx
	{ Cmd_BCHG2,	0xf1c00000, 0x01400000 },	// BCHG     0000 xxx1 01xx xxxx
	{ Cmd_BCLR2,	0xf1c00000, 0x01800000 },	// BCLR     0000 xxx1 10xx xxxx
	{ Cmd_BSET2,	0xf1c00000, 0x01c00000 },	// BSET     0000 xxx1 11xx xxxx

	{ Cmd_MOVEP,	0xf1f80000, 0x01080000 },	// MOVEP.w	0000 xxx1 0000 1xxx
	{ Cmd_MOVEP,	0xf1f80000, 0x01480000 },	// MOVEP.l	0000 xxx1 0100 1xxx
	{ Cmd_MOVEP,	0xf1f80000, 0x01880000 },	// MOVEP.w	0000 xxx1 1000 1xxx
	{ Cmd_MOVEP,	0xf1f80000, 0x01c80000 },	// MOVEP.l	0000 xxx1 1100 1xxx
	{ NULL,			0x00000000, 0x00000000 },
};

static const struct CmdStruct m68kCmds_0001[] =
{
	{ Cmd_MOVE,		0xf0000000, 0x10000000 },	// MOVE.b	0001 xxxx xxxx xxxx
	{ NULL,			0x00000000, 0x00000000 },
};

static const struct CmdStruct m68kCmds_0010[] =
{
	{ Cmd_MOVEA,	0xf1c00000, 0x20400000 },	// MOVEA.l	0010 xxx0 01xx xxxx
	{ Cmd_MOVE,		0xf0000000, 0x20000000 },	// MOVE.l	0010 xxxx xxxx xxxx - Must be after MOVEA
	{ NULL,			0x00000000, 0x00000000 },
};

static const struct CmdStruct m68kCmds_0011[] =
{
	{ Cmd_MOVEA,	0xf1c00000, 0x30400000 },	// MOVEA.w	0011 xxx0 01xx xxxx
	{ Cmd_MOVE,		0xf0000000, 0x30000000 },	// MOVE.w	0011 xxxx xxxx xxxx - Must be after MOVEA
	{ NULL,			0x00000000, 0x00000000 },
};

static const struct CmdStruct m68kCmds_0100[] =
{
//    { Cmd_NEGX,     0xffc00000, 0x40000000 },   // NEGX.b   0100 0000 00xx xxxx
//    { Cmd_NEGX,     0xffc00000, 0x40400000 },   // NEGX.w   0100 0000 01xx xxxx
//    { Cmd_NEGX,     0xffc00000, 0x40800000 },   // NEGX.l   0100 0000 10xx xxxx
//  { Cmd_MOVE_SR,  wwwwwwwwww, wwwwwwwwww },   // MOVE     0100 0000 11xx xxxx

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
//	{ Cmd_CHK,      wwwwwwwwww, wwwwwwwwww },   // CHK      0100 xxxx x0xx xxxx

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
	{ Cmd_PACK,		0xf1f00000, 0x81400000 },	// PACK		1000 xxx1 0100 xxxx
	{ Cmd_UNPK,		0xf1f00000, 0x81800000 },	// UNPK		1000 xxx1 1000 xxxx

	{ Cmd_OR,		0xf1c00000, 0x81000000 },	// OR       1000 xxx1 00xx xxxx
	{ Cmd_OR,		0xf1c00000, 0x81400000 },	// OR       1000 xxx1 01xx xxxx
	{ Cmd_OR,		0xf1c00000, 0x81800000 },	// OR       1000 xxx1 10xx xxxx
	{ Cmd_DIV,		0xf1c00000, 0x81c00000 },	// DIVS.w   1000 xxx1 11xx xxxx

	{ NULL,			0x00000000, 0x00000000 },
};

static const struct CmdStruct m68kCmds_1001[] =
{
//  { Cmd_SUBX,     wwwwwwwwww, wwwwwwwwww },   // SUBX     1001 xxx1 xx00 xxxx

	{ Cmd_SUB,		0xf1c00000, 0x90000000 },	// SUB		1001 xxx0 00xx xxxx
	{ Cmd_SUB,		0xf1c00000, 0x90400000 },	// SUB		1001 xxx0 01xx xxxx
	{ Cmd_SUB,		0xf1c00000, 0x90800000 },	// SUB		1001 xxx0 10xx xxxx
	{ Cmd_SUBA,		0xf1c00000, 0x90c00000 },	// SUBA		1001 xxx0 11xx xxxx

	{ Cmd_SUB,		0xf1c00000, 0x91000000 },	// SUB		1001 xxx1 00xx xxxx
	{ Cmd_SUB,		0xf1c00000, 0x91400000 },	// SUB		1001 xxx1 01xx xxxx
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
	{ Cmd_CMPM,		0xf1f80000, 0xb1480000 },	// CMPM.w	1011 xxx1 0100 1xxx
	{ Cmd_CMPM,		0xf1f80000, 0xb1880000 },	// CMPM.l	1011 xxx1 1000 1xxx

	{ Cmd_EOR,		0xf1c00000, 0xb1000000 },	// EOR.b    1011 xxx1 00xx xxxx
	{ Cmd_EOR,		0xf1c00000, 0xb1400000 },	// EOR.w    1011 xxx1 01xx xxxx
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

	{ Cmd_EXG,		0xf1f80000, 0xc1400000 },   // EXG.l	1100 xxx1 0100 0xxx
	{ Cmd_EXG,		0xf1f80000, 0xc1480000 },   // EXG.l	1100 xxx1 0100 1xxx
	{ Cmd_EXG,		0xf1f80000, 0xc1880000 },   // EXG.l	1100 xxx1 1000 1xxx

	{ Cmd_AND,		0xf1c00000, 0xc1000000 },	// AND.b	1100 xxx1 00xx xxxx
	{ Cmd_AND,		0xf1c00000, 0xc1400000 },	// AND.w	1100 xxx1 01xx xxxx
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

//  { Cmd_ADDX,     0xf1f00000, 0xd1000000 },   // ADDX.b   1101 xxx1 0000 xxxx
//  { Cmd_ADDX,     0xf1f00000, 0xd1400000 },   // ADDX.w   1101 xxx1 0100 xxxx
//  { Cmd_ADDX,     0xf1f00000, 0xd1800000 },   // ADDX.l   1101 xxx1 1000 xxxx

	{ Cmd_ADD,		0xf1c00000, 0xd1000000 },	// ADD.b    1101 xxx1 00xx xxxx
	{ Cmd_ADD,		0xf1c00000, 0xd1400000 },	// ADD.w    1101 xxx1 01xx xxxx
	{ Cmd_ADD,		0xf1c00000, 0xd1800000 },	// ADD.l    1101 xxx1 10xx xxxx
	{ Cmd_ADDA,		0xf1c00000, 0xd1c00000 },	// ADDA.l   1101 xxx1 11xx xxxx

	{ NULL,			0x00000000, 0x00000000 },
};

static const struct CmdStruct m68kCmds_1110[] =
{
	{ Cmd_ASR2,		0xffc00000, 0xe0c00000 },	// ASR		1110 0000 11xx xxxx
	{ Cmd_ASL2,		0xffc00000, 0xe1c00000 },	// ASL		1110 0001 11xx xxxx
	{ Cmd_LSR2,		0xffc00000, 0xe2c00000 },	// LSR		1110 0010 11xx xxxx
	{ Cmd_LSL2,		0xffc00000, 0xe3c00000 },	// LSL		1110 0011 11xx xxxx
	{ Cmd_ROXR2,	0xffc00000, 0xe4c00000 },	// ROXR		1110 0100 11xx xxxx
	{ Cmd_ROXL2,	0xffc00000, 0xe5c00000 },	// ROXL		1110 0101 11xx xxxx
	{ Cmd_ROR2,		0xffc00000, 0xe6c00000 },	// ROR		1110 0110 11xx xxxx
	{ Cmd_ROL2,		0xffc00000, 0xe7c00000 },	// ROL		1110 0111 11xx xxxx

//  { Cmd_BFTST     wwwwwwwwww, wwwwwwwwww },   // BFTST    1110 1000 11xx xxxx 0000 xxxx xxxxx xxxx
//  { Cmd_BFEXTU    wwwwwwwwww, wwwwwwwwww },   // BFEXTU   1110 1001 11xx xxxx 0xxx xxxx xxxxx xxxx
//  { Cmd_BFCHG     wwwwwwwwww, wwwwwwwwww },   // BFCHG    1110 1010 11xx xxxx 0000 xxxx xxxxx xxxx
//  { Cmd_BFEXTS    wwwwwwwwww, wwwwwwwwww },   // BFEXTS   1110 1011 11xx xxxx 0xxx xxxx xxxxx xxxx
//  { Cmd_BFCLR     wwwwwwwwww, wwwwwwwwww },   // BFCLT    1110 1100 11xx xxxx 0000 xxxx xxxxx xxxx
//  { Cmd_BFFFO     wwwwwwwwww, wwwwwwwwww },   // BFFFO    1110 1101 11xx xxxx 0xxx xxxx xxxxx xxxx
//  { Cmd_BFSET     wwwwwwwwww, wwwwwwwwww },   // BFSET    1110 1110 11xx xxxx 0000 xxxx xxxxx xxxx
//  { Cmd_BFINS     wwwwwwwwww, wwwwwwwwww },   // BFINS    1110 1111 11xx xxxx 0xxx xxxx xxxxx xxxx

	{ Cmd_ASR,		0xf1d80000, 0xe0000000 },	// ASR.b	1110 xxx0 00x0 0xxx
	{ Cmd_ASR,		0xf1d80000, 0xe0400000 },	// ASR.w	1110 xxx0 01x0 0xxx
	{ Cmd_ASR,		0xf1d80000, 0xe0800000 },	// ASR.l	1110 xxx0 10x0 0xxx

	{ Cmd_ASL,		0xf1d80000, 0xe1000000 },	// ASL.b	1110 xxx1 00x0 0xxx
	{ Cmd_ASL,		0xf1d80000, 0xe1400000 },	// ASL.w	1110 xxx1 01x0 0xxx
	{ Cmd_ASL,		0xf1d80000, 0xe1800000 },	// ASL.l	1110 xxx1 10x0 0xxx

	{ Cmd_LSR,		0xf1d80000, 0xe0080000 },	// LSR.b	1110 xxx0 00x0 1xxx
	{ Cmd_LSR,		0xf1d80000, 0xe0480000 },	// LSR.w	1110 xxx0 01x0 1xxx
	{ Cmd_LSR,		0xf1d80000, 0xe0880000 },	// LSR.l	1110 xxx0 10x0 1xxx

	{ Cmd_LSL,		0xf1d80000, 0xe1080000 },	// LSL.b	1110 xxx1 00x0 1xxx
	{ Cmd_LSL,		0xf1d80000, 0xe1480000 },	// LSL.w	1110 xxx1 01x0 1xxx
	{ Cmd_LSL,		0xf1d80000, 0xe1880000 },	// LSL.l	1110 xxx1 10x0 1xxx

	{ Cmd_ROXR,		0xf1d80000, 0xe0100000 },	// ROXR.b	1110 xxx0 00x1 0xxx
	{ Cmd_ROXR,		0xf1d80000, 0xe0500000 },	// ROXR.w	1110 xxx0 01x1 0xxx
	{ Cmd_ROXR,		0xf1d80000, 0xe0900000 },	// ROXR.l	1110 xxx0 10x1 0xxx

	{ Cmd_ROXL,		0xf1d80000, 0xe1100000 },	// ROXL.b	1110 xxx1 00x1 0xxx
	{ Cmd_ROXL,		0xf1d80000, 0xe1500000 },	// ROXL.w	1110 xxx1 01x1 0xxx
	{ Cmd_ROXL,		0xf1d80000, 0xe1900000 },	// ROXL.l	1110 xxx1 10x1 0xxx

	{ Cmd_ROR,		0xf1d80000, 0xe0180000 },	// ROR.b	1110 xxx0 00x1 1xxx
	{ Cmd_ROR,		0xf1d80000, 0xe0580000 },	// ROR.w	1110 xxx0 01x1 1xxx
	{ Cmd_ROR,		0xf1d80000, 0xe0980000 },	// ROR.l	1110 xxx0 10x1 1xxx

	{ Cmd_ROL,		0xf1d80000, 0xe1180000 },	// ROL.b	1110 xxx1 00x1 1xxx
	{ Cmd_ROL,		0xf1d80000, 0xe1580000 },	// ROL.w	1110 xxx1 01x1 1xxx
	{ Cmd_ROL,		0xf1d80000, 0xe1980000 },	// ROL.l	1110 xxx1 10x1 1xxx

	{ NULL,			0x00000000, 0x00000000 },
};

static const struct CmdStruct m68kCmds_1111[] =
{
//    { Cmd_FMovem,   0xf1c0c300, 0xf000c000 },

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
	ms->ms_ArgSize			= 2;
	ms->ms_DecMode			= 0;
	ms->ms_DoExternal		= 1;
	ms->ms_ClearRegs		= CR_No;
	ms->ms_LibCall			= 0;

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
