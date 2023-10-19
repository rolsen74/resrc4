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

**  Abcd       B      
**  Add        B W L  
**  Adda         W L  
**  Addi       B W L  
**  Addq       B W L  
**  And        B W L  
**  Andi       B W L  
**  Asl        B W L  
**  Asr        B W L  
**  Bcc        B W L  
**  BChg       B   L  
**  BClr       B   L  
**  Bcs        B W L  
**  Beq        B W L  
**  Bge        B W L  
**  BGnd              
**  Bgt        B W L  
**  Bhi        B W L  
**  Bkpt              
**  Ble        B W L  
**  Bls        B W L  
**  Blt        B W L  
**  Bmi        B W L  
**  Bne        B W L  
**  Bpl        B W L  
**  Bra        B W L  
**  BSet       B   L  
**  Bsr        B W L  
**  BTst       B   L  
**  Bvc        B W L  
**  Bvs        B W L  
**  Callm             
**  Clr        B W L  
**  Cmp        B W L  
**  Cmpa         W L  
**  Cmpi       B W L  
**  Cmpm       B W L  
**  DBcc         W    
**  DBcs         W    
**  DBeq         W    
**  DBf          W    
**  DBge         W    
**  DBgt         W    
**  DBhi         W    
**  DBle         W    
**  DBls         W    
**  DBlt         W    
**  DBmi         W    
**  DBne         W    
**  DBpl         W    
**  DBt          W    
**  DBvc         W    
**  DBvs         W    
**  Divs         W L  
**  Divu         W L  
**  Eor        B W L  
**  Eori       B W L  
**  Exg            L  
**  Ext          W L  
**  Extb           L  
**  Illegal           
**  Jmp               
**  Jsr               
**  Lea            L  
**  Link         W L  
**  Lsl        B W L  
**  Lsr        B W L  
**  Move       B W L  
**  Movea        W L  
**  Movem        W L  
**  Movep        W L  
**  Moveq          L  
**  Muls         W L  
**  Mulu         W L  
**  Nbcd       B      
**  Neg        B W L  
**  Nop               
**  Not        B W L  
**  Or         B W L  
**  Ori        B W L  
**  Pack              
**  Pea            L  
**  Reset             
**  Rol        B W L  
**  Ror        B W L  
**  Roxl       B W L  
**  Roxr       B W L  
**  Rtd               
**  Rte               
**  Rtr               
**  Rts               
**  Sbcd       B      
**  Scc        B      
**  Scs        B      
**  Seq        B      
**  Sf         B      
**  Sge        B      
**  Sgt        B      
**  Shi        B      
**  Sle        B      
**  Sls        B      
**  Slt        B      
**  Smi        B      
**  Sne        B      
**  Spl        B      
**  St         B      
**  Sub        B W L  
**  Suba         W L  
**  Subi       B W L  
**  Subq       B W L  
**  Svc        B      
**  Svs        B      
**  Swap         W    
**  Trap              
**  Trapv             
**  Tst        B W L  
**  Unlk              
**  Unpk              

# Hunk file support

Again, only what is needed has been implemented, so it's a little limited. The original Civ game did not include Symbol, DRel32, and Relloc32Short, but when I assembled the disassembled source with vasm, those hunks were used.

** HUNK_CODE (3E9)
** HUNK_DATA (3EA)
** HUNK_BSS (3EB)
** HUNK_RELOC32 (3EC)
** HUNK_SYMBOL (3F0)
** HUNK_END (3F2)
** HUNK_HEADER (3F3)
** HUNK_DREL32 (3F7)
** HUNK_RELOC32SHORT (3FC)
