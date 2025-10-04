
/*
** Copyright (c) 2014-2025 Rene W. Olsen
**
** SPDX-License-Identifier: GPL-3.0-or-later
**
** This software is released under the GNU General Public License, version 3.
** For the full text of the license, please visit:
** https://www.gnu.org/licenses/gpl-3.0.html
**
** You can also find a copy of the license in the LICENSE file included with this software.
*/

#ifndef CPU_M68K_H
#define CPU_M68K_H 1

#ifndef RESOURCER_RESRC4_H
#error ReSrc4.h must be included first
#endif

// --

enum M68kRegisterType
{
	M68KREGT_Dx = 0,
	M68KREGT_D0 = 0,
	M68KREGT_D1,
	M68KREGT_D2,
	M68KREGT_D3,
	M68KREGT_D4,
	M68KREGT_D5,
	M68KREGT_D6,
	M68KREGT_D7,
	M68KREGT_Ax = 8,
	M68KREGT_A0 = 8,
	M68KREGT_A1,
	M68KREGT_A2,
	M68KREGT_A3,
	M68KREGT_A4,
	M68KREGT_A5,
	M68KREGT_A6,
	M68KREGT_A7,
	M68KREGT_Last,
};

// --

enum M68KOpcodeSize
{
	M68KSIZE_Unsized,
	M68KSIZE_Byte,		// move.b
	M68KSIZE_Word,		// move.w
	M68KSIZE_Long,		// move.l
	M68KSIZE_Single,	// fmove.s
	M68KSIZE_Double,	// fmove.d
	M68KSIZE_Extended,	// fmove.x
	M68KSIZE_Packed,	// fmove.p
};

// --

enum M68KStat
{
	M68KSTAT_UnknownCmd,
	M68KSTAT_Opcode,
	M68KSTAT_Error,
};

// --

struct M68kRegister
{
	enum RS4RegType			mr_Type1;
	S32						mr_Type2;			// Subtype
	#ifdef DEBUG
	S32						mr_Number;			// Temp debug
	#endif
	S32						mr_Address;			// should I do 64bit?
	RS4Label *				mr_Label;
};

// --

struct AmigaOS3_Misc_Move_GetSetStruct
{
	enum GSSType			SrcType;
	S32						LibType;
	RS4Label *				Label;
	struct M68kRegister 	Reg;
};

// --

enum M68kDecMode
{
	M68kDecMode_Signed,
	M68kDecMode_Unsigned,
	M68kDecMode_Hex,
};

struct M68kTrace
{
	U32						mt_Opcode;				// First 4 bytes of Opcode
	S32						mt_OpcodeSize;			// Total size of Opcode
	S32						mt_ArgEMode;			// Effective Addreess Mode
	S32						mt_ArgEReg;				// Effective Addreess Register
	S32						mt_ArgSize;				// Opcode Size in bytes
	enum M68KOpcodeSize		mt_ArgType;				// Opcode Type (b/w/l)
	enum M68kDecMode		mt_DecMode;				// Hex, Sign Dec, Unsign Dec
	S8						mt_LastOpcode;			// Rts or Jmp .. sets this
	S8						mt_IsPea;				// Negative values, will be printed as Decimal
	S8						mt_DoLabelSize;			// 
	S8						mt_LibCall;				// Well Possible LibCall
	S8						mt_DoExternal;
	S8						mt_Pad;
	U16						mt_ClearRegMask;		// Regs to Destroy on Return
	struct M68kRegister *	mt_CurRegister;
	struct M68kRegister		mt_JmpRegister;
	struct M68kRegister		mt_SrcRegister;
	struct M68kRegister		mt_DstRegister;
	struct M68kRegister		mt_Registers[16];
};

// -- Extern

extern CSTR FPx_RegNames[8];
extern CSTR Ax_RegNames[8];
extern CSTR Dx_RegNames[8];
extern CSTR scale_Names[4];


// -- Protos
// -- M68k

enum RS4DecodeStat	M68k_Decoder(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat	M68k_EffectiveAddress(	enum RS4ErrorCode *, RS4Trace * );
void				M68k_Set_Cur_to_Unknown( RS4Trace *rt );

STR 	AmigaOS3_FindLibFunc(			enum RS4ErrorCode *errcode, RS4Trace *rt, S16 val, STR file );

// -- Opcodes
enum RS4DecodeStat M68kCmd_ABCD(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_ADD(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_ADDA(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_ADDI(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_ADDQ(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_ADDX(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_AND(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_ANDI(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_ANDI2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_ANDI3(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_ASL(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_ASL2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_ASR(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_ASR2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_Bcc(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_BCHG(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_BCHG2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_BCLR(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_BCLR2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_BGND(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_BKPT(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_BSET(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_BSET2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_BTST(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_BTST2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_CALLM(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_CHK(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_CHK2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_CLR(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_CMP(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_CMP2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_CMPA(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_CMPI(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_CMPM(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_DBcc(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_DIV(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_DIVS_L(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_DIVU_L(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_EOR(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_EORI(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_EORI2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_EORI3(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_EXG( 		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_EXT(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_ILLEGAL(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_JMP(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_JSR(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_LEA(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_LINK(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_LINK_L(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_LPSTOP(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_LSL(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_LSL2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_LSR(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_LSR2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_MOVE(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_MOVE16(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_MOVE162(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_MOVE2( 		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_MOVE3(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_MOVE4(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_MOVE5(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_MOVE6(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_MOVEA(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_MOVEC(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_MOVEM(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_MOVEP(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_MOVEQ(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_MOVES(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_MUL(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_MULS_L(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_MULU_L(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_NBCD(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_NEG(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_NEGX(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_NOP(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_NOT(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_OR(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_ORI(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_ORI2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_ORI3(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_PACK(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_PEA(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_RESET(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_ROL(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_ROL2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_ROR(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_ROR2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_ROXL(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_ROXL2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_ROXR(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_ROXR2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_RTD(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_RTE(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_RTM(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_RTR(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_RTS(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_SBCD(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_Scc(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_STOP(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_SUB(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_SUBA(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_SUBI(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_SUBQ(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_SUBX(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_SWAP(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_TAS(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_TRAP(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_TRAPcc(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_TRAPV(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_TST(			enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_UNLK(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_UNPK(		enum RS4ErrorCode *, RS4Trace * );

// -- FPU
enum RS4DecodeStat M68kCmd_FABS(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FABS2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FACOS(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FACOS2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FADD(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FADD2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FASIN(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FASIN2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FATAN(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FATAN2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FATANH(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FATANH2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FBcc(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FCMP(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FCMP2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FCOS(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FCOS2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FCOSH(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FCOSH2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FDABS(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FDABS2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FDADD(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FDADD2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FDDIV(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FDDIV2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FDIV(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FDIV2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FDMOVE(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FDMOVE2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FDMUL(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FDMUL2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FDNEG(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FDNEG2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FDSQRT(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FDSQRT2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FDSUB(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FDSUB2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FETOX(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FETOX2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FETOXM1(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FETOXM12(	enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FGETEXP(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FGETEXP2(	enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FGETMAN(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FGETMAN2(	enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FINT(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FINT2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FINTRZ(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FINTRZ2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FLOG2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FLOG22(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FLOG10(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FLOG102(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FLOGN(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FLOGN2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FLOGNP1(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FLOGNP12(	enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FMOD(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FMOD2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FMOVE(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FMOVE2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FMOVE3(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FMOVE4(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FMOVECR(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FMOVEM(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FMOVEM2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FMUL(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FMUL2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FNEG(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FNEG2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FNOP(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FREM(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FREM2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FRESTORE(	enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FSABS(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FSABS2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FSADD(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FSADD2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FSAVE(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FSCALE(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FSCALE2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FScc(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FSDIV(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FSDIV2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FSGLDIV(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FSGLDIV2(	enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FSGLMUL(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FSGLMUL2(	enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FSIN(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FSIN2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FSINCOS(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FSINCOS2(	enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FSINH(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FSINH2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FSMOVE(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FSMOVE2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FSMUL(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FSMUL2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FSNEG(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FSNEG2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FSQRT(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FSQRT2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FSSQRT(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FSSQRT2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FSSUB(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FSSUB2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FSUB(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FSUB2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FTAN(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FTAN2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FTANh(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FTANh2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FTENTOX(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FTENTOX2(	enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FTST(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FTST2(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FTWOTOX(		enum RS4ErrorCode *, RS4Trace * );
enum RS4DecodeStat M68kCmd_FTWOTOX2(	enum RS4ErrorCode *, RS4Trace * );

// --

#endif
