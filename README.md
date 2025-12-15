# ReSrc4 - MC680x0 Disassembler

Portable Amiga 68k Hunk File Disassembler.

This disassembler was tested against the Amiga game Civilization (ECS) and Lightwave for FPU support.

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

- **PC** – Program Counter  
- **d8** – Signed 8-bit value  
- **d16** – Signed 16-bit value  
- **d32** – Signed 32-bit value  
- **An** – Ax Register  
- **Dn** – Dx Register  
- **Xn** – Ax/Dx Register \[W/L\] with scale  
- **xxx** – Address  
- **data** – 32-bit data value  

### Supported

- `Dn`
- `An`
- `( An )`
- `( An )+`
- `-( An )`
- `( d16 , An )`
- `( d16 , PC )`
- `( d8 , An , Xn )`
- `( d8 , PC , Xn )`
- `( bd , An , Xn )`
- `( bd , PC , Xn )`
- `( [ bd , An ] , Xn , od )`
- `( [ bd , PC ] , Xn , od )`
- `( [ bd , An , Xn ] , od )`
- `( [ bd , PC , Xn ] , od )`
- `( xxx ).w`
- `( xxx ).l`
- `# <data>`

## ***CPU Opcodes list
<details>
<summary>Show CPU opcodes</summary>

- Abcd
- Add
- Adda
- Addi
- Addq
- Addx
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
- LPStop
- Lsl
- Lsr
- Move
- Move16
- Movea
- Movec
- Movem
- Movep
- Moveq
- Moves
- Muls
- Mulu
- Nbcd
- Neg
- Negx
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
- Subx
- Svc
- Svs
- Swap
- Tas
- Trap
- Trapcc
- Trapcs
- Trapeq
- Trapf
- Trapge
- Trapgt
- Traphi
- Traple
- Trapls
- Traplt
- Trapmi
- Trapne
- Trappl
- Trapt
- Trapv
- Trapvc
- Trapvs
- Tst
- Unlk
- Unpk

</details>

## ***FPU Opcodes list
<details>
<summary>Show FPU opcodes</summary>

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
- FNop
- FRem
- FRestore
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

</details>

# Hunk / FHR Containers

Again, only what is needed has been implemented, so it's a little limited. The original Civ game did not include Symbol, DRel32, and Relloc32Short, but when I assembled the disassembled source with vasm, those hunks were used.

Added my own custom FHR Container, for testing.

## Hunk Container
<details>
<summary>Hunks Supported</summary>

| Label               |  Hex ID  |  Dec ID  | Description / Note        |
|---------------------|----------|----------|---------------------------|
| `HUNK_CODE`         | `0x03E9` | `1001`   | Code Section              |
| `HUNK_DATA`         | `0x03EA` | `1002`   | Data Section              |
| `HUNK_BSS`          | `0x03EB` | `1003`   | Bss Section               |
| `HUNK_RELOC32`      | `0x03EC` | `1004`   |                           |
| `HUNK_SYMBOL`       | `0x03F0` | `1008`   |                           |
| `HUNK_DEBUG`        | `0x03F1` | `1009`   | *skipping only*           |
| `HUNK_END`          | `0x03F2` | `1010`   |                           |
| `HUNK_HEADER`       | `0x03F3` | `1011`   | File Header               |
| `HUNK_DREL32`       | `0x03F7` | `1015`   | *Same as HUNK_RELRELOC32* |
| `HUNK_RELOC32SHORT` | `0x03FC` | `1020`   | *Same as HUNK_DREL32*     |
| `HUNK_RELRELOC32`   | `0x03FD` | `1021`   |                           |

</details>

## FHR Container
<details>
<summary>FHR Chunks Supported</summary>

| Label        | ID        | Description               |
|--------------|-----------|---------------------------|
| `FHR_HEADER` | `FHR\0`   | File Header               |
| `FHR_COD0`   | `COD\0`   | Executable Code Section   |
| `FHR_DAT0`   | `DAT\0`   | Executable Data Section   |
| `FHR_BSS0`   | `BSS\0`   | Executable BSS Section    |
| `FHR_END0`   | `END\0`   | End of Section            |

</details>
