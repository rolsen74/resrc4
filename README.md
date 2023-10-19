# ReSrc4 - MC680x0 Disassembler

Portable Amiga 68k Hunk File Disassembler.

This disassembler was tested against the Amiga game Civilization (ECS).

The ReSrc4 program does not handle most of the newer 020+ opcode argument modes. It mainly implements only opcodes that were needed for Civ.

## Features

### Library Call Recognition

It attempts to convert library calls into a more readable format. For example:

- `jsr (-552,a6)` will be converted to `jsr (_LVOOpenLibrary,a6)` when it can identify that a6 holds the ExecBase.
- It can also detect calls to DiskFont, Dos, Graphics, and Intuition library calls.

### Jump Table Recognition

There is also some JumpTable detection. Please note that this code is a little rough and may need a good rewrite, which is on the to-do list. You will likely need to create a config file and use the Relative16 option.

## Opcodes (Currently Supported)

Please be aware of the following:

- Moving to/from CCR or SR registers has not been implemented.
- The movec opcode is missing.
- There are no MMU and FPU opcode handling.

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
 - Clr
 - Cmp
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

# Hunk file support

Again, only what is needed has been implemented, so it's a little limited. The original Civ game did not include Symbol, DRel32, and Relloc32Short, but when I assembled the disassembled source with vasm, those hunks were used.

**Hunks Supported:**
- HUNK_CODE (3E9)
- HUNK_DATA (3EA)
- HUNK_BSS (3EB)
- HUNK_RELOC32 (3EC)
- HUNK_SYMBOL (3F0)
- HUNK_END (3F2)
- HUNK_HEADER (3F3)
- HUNK_DREL32 (3F7)
- HUNK_RELOC32SHORT (3FC)