# ReSrc4 - MC680x0 Disassembler

Portable Amiga 68k Hunk File Disassembler.

This disassembler was tested against the Amiga game Civilization (ECS) and Lightwave for FPU support.

The ReSrc4 program does not handle most of the newer 020+ opcode argument modes. It mainly implements only opcodes that were needed for Civ.

## Features

### Library Call Recognition

It attempts to convert library calls into a more readable format. For example:

- `jsr (-552,a6)` will be converted to `jsr (_LVOOpenLibrary,a6)` when it can identify that a6 holds the ExecBase.
- It can also detect calls to DiskFont, Dos, Graphics, and Intuition library calls.

### Jump Table Recognition

There is also some JumpTable detection but its far from perfect. 
You will likely need to create a config file and use the Relative16 option.

## Effective Address Modes

### Modes

PC = Program Counter<br>
d8 = Signed 8bit value<br>
d16 = Signed 16bit value<br>
d32 = Signed 32bit value<br>
An = Ax Register<br>
Dn = Dx Register<br>
Xn = Ax/Dx Register.[W/L] with Scale<br>
xxx = Address<br>
data = 32bit Data value<br>

### Currently Supported

- Dn
- An
- ( An )
- ( An )+
- -( An )
- ( d16 , An )
- ( d16 , PC )
- ( d8 , An , Xn )
- ( d8 , PC , Xn )
- ( xxx ).w
- ( xxx ).l
- `# <data>`

### Missing

- ( bd , An , Xn )
- ( bd , PC , Xn )
- ( [ bd , An ] , Xn , od )
- ( [ bd , PC ] , Xn , od )
- ( [ bd , An , Xn ] , od )
- ( [ bd , PC , Xn ] , od )

## Opcodes

Please be aware of the following:

- Moving to/from CCR or SR registers has not been implemented.
- The movec opcode is missing.
- There are no MMU opcode handling.

**Integer Opcodes (Currently Supported)**

- Abcd
- Add
- Adda
- Addi
- Addq
- And
- Andi
- Asl
- Asr
- Bcc
- BChg
- BClr
- Bcs
- Beq
- Bge
- BGnd
- Bgt
- Bhi
- Bkpt
- Ble
- Bls
- Blt
- Bmi
- Bne
- Bpl
- Bra
- BSet
- Bsr
- BTst
- Bvc
- Bvs
- Callm
- Chk
- Chk2
- Clr
- Cmp
- Cmp2
- Cmpa
- Cmpi
- Cmpm
- DBcc
- DBcs
- DBeq
- DBf
- DBge
- DBgt
- DBhi
- DBle
- DBls
- DBlt
- DBmi
- DBne
- DBpl
- DBt
- DBvc
- DBvs
- Divs
- Divsl
- Divu
- Eor
- Eori
- Exg
- Ext
- Extb
- Illegal
- Jmp
- Jsr
- Lea
- Link
- Lsl
- Lsr
- Move
- Movea
- Movem
- Movep
- Moveq
- Muls
- Mulu
- Nbcd
- Neg
- Nop
- Not
- Or
- Ori
- Pack
- Pea
- Reset
- Rol
- Ror
- Roxl
- Roxr
- Rtd
- Rte
- Rtm
- Rtr
- Rts
- Sbcd
- Scc
- Scs
- Seq
- Sf
- Sge
- Sgt
- Shi
- Sle
- Sls
- Slt
- Smi
- Sne
- Spl
- St
- Sub
- Suba
- Subi
- Subq
- Svc
- Svs
- Swap
- Trap
- Trapv
- Tst
- Unlk
- Unpk

**FPU Opcodes (Currently Supported)**

- FAbs
- FACos
- FAdd
- FASin
- FATan
- FATanh
- FBeq
- FBf
- FBge
- FBgl
- FBgle
- FBgt
- FBle
- FBlt
- FBne
- FBnge
- FBngl
- FBngle
- FBngt
- FBoge
- FBogl
- FBogt
- FBole
- FBolt
- FBor
- FBseq
- FBsne
- FBst
- FBt
- FBueq
- FBuge
- FBugt
- FBule
- FBult
- FBun
- FCmp
- FCos
- FCosh
- FDAbs
- FDAdd
- FDDiv
- FDiv
- FDMove
- FDMul
- FDNeg
- FDSqrt
- FDSub
- FEtox
- FEtoxm1
- FGetexp
- FGetman
- FInt
- FIntrz
- FLog2
- FLog10
- FLogn
- FLognp1
- FMod
- FMove
- FMovecr
- FMovem (Partially implemented)
- FMul
- FNeg
- FRem
- FSAbs
- FSAdd
- FScale
- FSDiv
- FSeq
- FSf
- FSge
- FSgl
- FSgldiv
- FSgle
- FSglmul
- FSgt
- FSin
- FSincos
- FSinh
- FSle
- FSlt
- FSMove
- FSMul
- FSne
- FSNeg
- FSnge
- FSngl
- FSngle
- FSngt
- FSnle
- FSnlt
- FSoge
- FSogl
- FSogt
- FSole
- FSolt
- FSor
- FSqrt
- FSseq
- FSsne
- FSSqrt
- FSst
- FSSub
- FSt
- FSub
- FSueq
- FSuge
- FSugt
- FSule
- FSult
- FSun
- FTan
- FTanh
- FTentox
- FTst
- FTwotox


# Hunk file support

Again, only what is needed has been implemented, so it's a little limited. The original Civ game did not include Symbol, DRel32, and Relloc32Short, but when I assembled the disassembled source with vasm, those hunks were used.

**Hunks Supported:**
- HUNK_CODE (3E9)
- HUNK_DATA (3EA)
- HUNK_BSS (3EB)
- HUNK_RELOC32 (3EC)
- HUNK_SYMBOL (3F0)
- HUNK_DEBUG (3F1) [Skipping]
- HUNK_END (3F2)
- HUNK_HEADER (3F3)
- HUNK_DREL32 (3F7)
- HUNK_RELOC32SHORT (3FC)