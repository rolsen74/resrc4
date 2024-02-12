
###########################################################################

.PHONY: all clean install

###########################################################################

# Flags used for compiling
CFLAGS		+= -I.
CFLAGS		+= -O2
CFLAGS		+= -gstabs
CFLAGS		+= -Wall
CFLAGS		+= -Wextra

# Flags passed to gcc during linking
LDFLAGS		:= 

# Additional linker libraries
LIBS		:=

###########################################################################

SRCS		:= ReSrc4.c
SRCS		+= BuildSource.c
SRCS		+= Config.c
SRCS		+= M68k.c
SRCS		+= Hunk.c
SRCS		+= LabelMagic.c
SRCS		+= MD5.c
SRCS		+= Misc.c
SRCS		+= Nodes.c
SRCS		+= SaveSource.c
SRCS		+= Trace.c
SRCS		+= Trace_JumpTables.c

SRCS		+= M68k__EffectiveAddress.c
SRCS		+= M68k__Library.c
SRCS		+= M68k__Lib_Amigaguide.c
SRCS		+= M68k__Lib_Asl.c
SRCS		+= M68k__Lib_Battclock.c
SRCS		+= M68k__Lib_Battmem.c
SRCS		+= M68k__Lib_Bullet.c
SRCS		+= M68k__Lib_Cardres.c
SRCS		+= M68k__Lib_Cia.c
SRCS		+= M68k__Lib_Colorwheel.c
SRCS		+= M68k__Lib_Commodities.c
#SRCS		+= M68k__Lib_Console.c
SRCS		+= M68k__Lib_Datatypes.c
SRCS		+= M68k__Lib_Disk.c
SRCS		+= M68k__Lib_Diskfont.c
SRCS		+= M68k__Lib_Dos.c
SRCS		+= M68k__Lib_DTClass.c
SRCS		+= M68k__Lib_Exec.c
SRCS		+= M68k__Lib_Expansion.c
SRCS		+= M68k__Lib_Gadtools.c
SRCS		+= M68k__Lib_Graphics.c
SRCS		+= M68k__Lib_Icon.c
SRCS		+= M68k__Lib_IFFParse.c
#SRCS		+= M68k__Lib_Input.c
SRCS		+= M68k__Lib_Intuition.c
SRCS		+= M68k__Lib_Keymap.c
SRCS		+= M68k__Lib_Layers.c
SRCS		+= M68k__Lib_Locale.c
SRCS		+= M68k__Lib_Lowlevel.c
SRCS		+= M68k__Lib_Mathffp.c
SRCS		+= M68k__Lib_Mathieeedoubbas.c
SRCS		+= M68k__Lib_Mathieeedoubtrans.c
SRCS		+= M68k__Lib_Mathieeesingbas.c
SRCS		+= M68k__Lib_Mathieeesingtrans.c
SRCS		+= M68k__Lib_Mathtrans.c
SRCS		+= M68k__Lib_Misc.c
SRCS		+= M68k__Lib_Nonvolatile.c
SRCS		+= M68k__Lib_Potgo.c
#SRCS		+= M68k__Lib_Ramdrive.c
SRCS		+= M68k__Lib_Rexxsyslib.c
#SRCS		+= M68k__Lib_Timer.c
SRCS		+= M68k__Lib_Translator.c
SRCS		+= M68k__Lib_Utility.c
SRCS		+= M68k__Lib_Workbench.c

SRCS		+= M68k_Abcd.c
SRCS		+= M68k_Add.c
SRCS		+= M68k_Adda.c
SRCS		+= M68k_Addi.c
SRCS		+= M68k_Addq.c
SRCS		+= M68k_Addx.c
SRCS		+= M68k_And.c
SRCS		+= M68k_Andi.c
SRCS		+= M68k_Asl.c
SRCS		+= M68k_Asr.c
SRCS		+= M68k_Bcc.c
SRCS		+= M68k_BChg.c
SRCS		+= M68k_BClr.c
SRCS		+= M68k_Bgnd.c
SRCS		+= M68k_Bkpt.c
SRCS		+= M68k_BSet.c
SRCS		+= M68k_BTst.c
SRCS		+= M68k_Callm.c
SRCS		+= M68k_Chk.c
SRCS		+= M68k_Chk2.c
SRCS		+= M68k_Clr.c
SRCS		+= M68k_Cmp.c
SRCS		+= M68k_Cmp2.c
SRCS		+= M68k_Cmpa.c
SRCS		+= M68k_Cmpi.c
SRCS		+= M68k_Cmpm.c
SRCS		+= M68k_DBcc.c
SRCS		+= M68k_Div.c
SRCS		+= M68k_Eor.c
SRCS		+= M68k_Eori.c
SRCS		+= M68k_Exg.c
SRCS		+= M68k_Ext.c
SRCS		+= M68k_Illegal.c
SRCS		+= M68k_Jmp.c
SRCS		+= M68k_Jsr.c
SRCS		+= M68k_Lea.c
SRCS		+= M68k_Link.c
SRCS		+= M68k_Lsl.c
SRCS		+= M68k_Lsr.c
SRCS		+= M68k_Move.c
SRCS		+= M68k_Movea.c
SRCS		+= M68k_Movem.c
SRCS		+= M68k_Movep.c
SRCS		+= M68k_Moveq.c
SRCS		+= M68k_Mul.c
SRCS		+= M68k_Nbcd.c
SRCS		+= M68k_Neg.c
SRCS		+= M68k_Nop.c
SRCS		+= M68k_Not.c
SRCS		+= M68k_Or.c
SRCS		+= M68k_Ori.c
SRCS		+= M68k_Pack.c
SRCS		+= M68k_Pea.c
SRCS		+= M68k_Reset.c
SRCS		+= M68k_Rol.c
SRCS		+= M68k_Ror.c
SRCS		+= M68k_Roxl.c
SRCS		+= M68k_Roxr.c
SRCS		+= M68k_Rtd.c
SRCS		+= M68k_Rte.c
SRCS		+= M68k_Rtm.c
SRCS		+= M68k_Rtr.c
SRCS		+= M68k_Rts.c
SRCS		+= M68k_Sbcd.c
SRCS		+= M68k_Scc.c
SRCS		+= M68k_Sub.c
SRCS		+= M68k_Suba.c
SRCS		+= M68k_Subi.c
SRCS		+= M68k_Subq.c
SRCS		+= M68k_Subx.c
SRCS		+= M68k_Swap.c
SRCS		+= M68k_Trap.c
SRCS		+= M68k_Trapv.c
SRCS		+= M68k_Tst.c
SRCS		+= M68k_Unlk.c
SRCS		+= M68k_Unpk.c

SRCS		+= M68k__FAbs.c
SRCS		+= M68k__FACos.c
SRCS		+= M68k__FAdd.c
SRCS		+= M68k__FASin.c
SRCS		+= M68k__FATan.c
SRCS		+= M68k__FATanh.c
SRCS		+= M68k__FBcc.c
SRCS		+= M68k__FCmp.c
SRCS		+= M68k__FCos.c
SRCS		+= M68k__FCosh.c
SRCS		+= M68k__FDAbs.c
SRCS		+= M68k__FDAdd.c
SRCS		+= M68k__FDDiv.c
SRCS		+= M68k__FDiv.c
SRCS		+= M68k__FDMove.c
SRCS		+= M68k__FDMul.c
SRCS		+= M68k__FDNeg.c
SRCS		+= M68k__FDSqrt.c
SRCS		+= M68k__FDSub.c
SRCS		+= M68k__FEtox.c
SRCS		+= M68k__FEtoxm1.c
SRCS		+= M68k__FGetexp.c
SRCS		+= M68k__FGetman.c
SRCS		+= M68k__FInt.c
SRCS		+= M68k__FIntrz.c
SRCS		+= M68k__FLog2.c
SRCS		+= M68k__FLog10.c
SRCS		+= M68k__FLogn.c
SRCS		+= M68k__FLognp1.c
SRCS		+= M68k__FMod.c
SRCS		+= M68k__FMove.c
SRCS		+= M68k__FMovecr.c
SRCS		+= M68k__FMovem.c
SRCS		+= M68k__FMul.c
SRCS		+= M68k__FNeg.c
SRCS		+= M68k__FRem.c
SRCS		+= M68k__FSAbs.c
SRCS		+= M68k__FSAdd.c
SRCS		+= M68k__FScale.c
SRCS		+= M68k__FScc.c
SRCS		+= M68k__FSDiv.c
SRCS		+= M68k__FSgldiv.c
SRCS		+= M68k__FSglmul.c
SRCS		+= M68k__FSin.c
SRCS		+= M68k__FSincos.c
SRCS		+= M68k__FSinh.c
SRCS		+= M68k__FSMove.c
SRCS		+= M68k__FSMul.c
SRCS		+= M68k__FSNeg.c
SRCS		+= M68k__FSqrt.c
SRCS		+= M68k__FSSqrt.c
SRCS		+= M68k__FSSub.c
SRCS		+= M68k__FSub.c
SRCS		+= M68k__FTan.c
SRCS		+= M68k__FTanh.c
SRCS		+= M68k__FTentox.c
SRCS		+= M68k__FTst.c
SRCS		+= M68k__FTwotox.c

###########################################################################

ifeq ($(OS),Windows_NT)
	detected_os := Windows
else
	detected_os := $(shell uname)
endif

# --

ifeq ($(detected_os),Windows)

AR			:= ar
RM			:= del
CC			:= gcc
STRIP		:= strip
MKDIR		:= mkdir
COPY		:= copy
SDK			:=
CRT			:=
TARGET		:= ReSrc4.exe

# --

else ifeq ($(detected_os),AmigaOS)

# Native AmigaOS4 Build
LS			:= list
AR			:= ar
RM			:= delete
CC			:= gcc
STRIP		:= strip
MKDIR		:= makedir FORCE
COPY		:= copy CLONE
SDK			:= sdk:
CRT			:= newlib
TARGET		:= ReSrc4
CFLAGS		+= -DBIGENDIAN

CFLAGS		+= -mcrt=$(CRT)
LDFLAGS		+= -mcrt=$(CRT)

# --

else # ifeq ($(detected_os),Linux)

CrossCompile := 0

ifeq ($(CrossCompile),1)

# Cross Compile for AmigaOS4
LS			:= ls -lort
AR			:= ppc-amigaos-ar
RM			:= rm
CC			:= ppc-amigaos-gcc
STRIP		:= ppc-amigaos-strip
MKDIR		:= mkdir -p
COPY		:= cp
SDK			:= /SDK/
CRT			:= newlib
TARGET		:= ReSrc4

CFLAGS		+= -mcrt=$(CRT)
LDFLAGS		+= -mcrt=$(CRT)

else

# Native Linux Build
LS			:= ls -lort
AR			:= ar
RM			:= rm
CC			:= gcc
STRIP		:= strip
MKDIR		:= mkdir -p
COPY		:= cp
SDK			:= 
CRT			:= 
TARGET		:= ReSrc4

endif
endif

# --

###############################################################################

all: $(TARGET)

###########################################################################

OBJS	:= $(SRCS:.c=.o)

###########################################################################

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LIBS) -Wl,--cref,-M,-Map=$@.map

%.o: %.c
	$(CC) -c $(CFLAGS) -o$@ $<

#%.o: %.c
#	@echo "Compiling $<"
#	@$(CC) -c $(CFLAGS) -o$@ $<

###########################################################################

clean:
	$(RM) $(TARGET) $(TARGET).map $(OBJS)

strip:
	@$(LS) $(TARGET)
	@$(STRIP) $(TARGET)
	@$(LS) $(TARGET)

###########################################################################

ReSrc4.o:					Makefile ReSrc4.h
BuildSource.o:				Makefile ReSrc4.h
Config.o:					Makefile ReSrc4.h
Hunk.o:						Makefile ReSrc4.h
LabelMagic.o:				Makefile ReSrc4.h
M68k.o:						Makefile ReSrc4.h
MD5.o:						Makefile ReSrc4.h
Misc.o:						Makefile ReSrc4.h
Nodes.o:					Makefile ReSrc4.h
SaveSource.o:				Makefile ReSrc4.h
Trace.o:					Makefile ReSrc4.h
Trace_JumpTables.o:			Makefile ReSrc4.h

M68k__EffectiveAddress.o:	Makefile ReSrc4.h
M68k__Library.o:			Makefile ReSrc4.h
M68k__Lib_Amigaguide.o:		Makefile ReSrc4.h
M68k__Lib_Asl.o:			Makefile ReSrc4.h
M68k__Lib_Battclock.o:		Makefile ReSrc4.h
M68k__Lib_Battmem.o:		Makefile ReSrc4.h
M68k__Lib_Bullet.o:			Makefile ReSrc4.h
M68k__Lib_Cardres.o:		Makefile ReSrc4.h
M68k__Lib_Cia.o:			Makefile ReSrc4.h
M68k__Lib_Colorwheel.o:		Makefile ReSrc4.h
M68k__Lib_Commodities.o:	Makefile ReSrc4.h
M68k__Lib_Console.o:		Makefile ReSrc4.h
M68k__Lib_Datatypes.o:		Makefile ReSrc4.h
M68k__Lib_Disk.o:			Makefile ReSrc4.h
M68k__Lib_Diskfont.o:		Makefile ReSrc4.h
M68k__Lib_Dos.o:			Makefile ReSrc4.h
M68k__Lib_DTClass.o:		Makefile ReSrc4.h
M68k__Lib_Exec.o:			Makefile ReSrc4.h
M68k__Lib_Expansion.o:		Makefile ReSrc4.h
M68k__Lib_Gadtools.o:		Makefile ReSrc4.h
M68k__Lib_Graphics.o:		Makefile ReSrc4.h
M68k__Lib_Icon.o:			Makefile ReSrc4.h
M68k__Lib_IFFParse.o:		Makefile ReSrc4.h
M68k__Lib_Input.o:			Makefile ReSrc4.h
M68k__Lib_Intuition.o:		Makefile ReSrc4.h
M68k__Lib_Keymap.o:			Makefile ReSrc4.h
M68k__Lib_Layers.o:			Makefile ReSrc4.h
M68k__Lib_Locale.o:			Makefile ReSrc4.h
M68k__Lib_Lowlevel.o:		Makefile ReSrc4.h
M68k__Lib_Mathffp.o:		Makefile ReSrc4.h
M68k__Lib_Mathieeedoubbas.o:	Makefile ReSrc4.h
M68k__Lib_Mathieeedoubtrans.o:	Makefile ReSrc4.h
M68k__Lib_Mathieeesingbas.o:	Makefile ReSrc4.h
M68k__Lib_Mathieeesingtrans.o:	Makefile ReSrc4.h
M68k__Lib_Mathtrans.o:		Makefile ReSrc4.h
M68k__Lib_Misc.o:			Makefile ReSrc4.h
M68k__Lib_Nonvolatile.o:	Makefile ReSrc4.h
M68k__Lib_Potgo.o:			Makefile ReSrc4.h
M68k__Lib_Ramdrive.o:		Makefile ReSrc4.h
M68k__Lib_Rexxsyslib.o:		Makefile ReSrc4.h
M68k__Lib_Timer.o:			Makefile ReSrc4.h
M68k__Lib_Translator.o:		Makefile ReSrc4.h
M68k__Lib_Utility.o:		Makefile ReSrc4.h
M68k__Lib_Workbench.o:		Makefile ReSrc4.h

M68k_Abcd.o:				Makefile ReSrc4.h
M68k_Add.o:					Makefile ReSrc4.h
M68k_Adda.o:				Makefile ReSrc4.h
M68k_Addi.o:				Makefile ReSrc4.h
M68k_Addq.o:				Makefile ReSrc4.h
M68k_Addx.o:				Makefile ReSrc4.h
M68k_And.o:					Makefile ReSrc4.h
M68k_Andi.o:				Makefile ReSrc4.h
M68k_Asl.o:					Makefile ReSrc4.h
M68k_Asr.o:					Makefile ReSrc4.h
M68k_Bcc.o:					Makefile ReSrc4.h
M68k_BChg.o:				Makefile ReSrc4.h
M68k_BClr.o:				Makefile ReSrc4.h
M68k_Bgnd.o:				Makefile ReSrc4.h
M68k_Bkpt.o:				Makefile ReSrc4.h
M68k_BSet.o:				Makefile ReSrc4.h
M68k_BTst.o:				Makefile ReSrc4.h
M68k_Callm.o:				Makefile ReSrc4.h
M68k_Chk.o:					Makefile ReSrc4.h
M68k_Chk2.o:				Makefile ReSrc4.h
M68k_Clr.o:					Makefile ReSrc4.h
M68k_Cmp.o:					Makefile ReSrc4.h
M68k_Cmp2.o:				Makefile ReSrc4.h
M68k_Cmpa.o:				Makefile ReSrc4.h
M68k_Cmpi.o:				Makefile ReSrc4.h
M68k_Cmpm.o:				Makefile ReSrc4.h
M68k_DBcc.o:				Makefile ReSrc4.h
M68k_Div.o:					Makefile ReSrc4.h
M68k_Eor.o:					Makefile ReSrc4.h
M68k_Eori.o:				Makefile ReSrc4.h
M68k_Exg.o:					Makefile ReSrc4.h
M68k_Ext.o:					Makefile ReSrc4.h
M68k_Illegal.o:				Makefile ReSrc4.h
M68k_Jmp.o:					Makefile ReSrc4.h
M68k_Jsr.o:					Makefile ReSrc4.h
M68k_Lea.o:					Makefile ReSrc4.h
M68k_Link.o:				Makefile ReSrc4.h
M68k_Lsl.o:					Makefile ReSrc4.h
M68k_Lsr.o:					Makefile ReSrc4.h
M68k_Move.o:				Makefile ReSrc4.h
M68k_Movea.o:				Makefile ReSrc4.h
M68k_Movem.o:				Makefile ReSrc4.h
M68k_Movep.o:				Makefile ReSrc4.h
M68k_Moveq.o:				Makefile ReSrc4.h
M68k_Mul.o:					Makefile ReSrc4.h
M68k_Nbcd.o:				Makefile ReSrc4.h
M68k_Neg.o:					Makefile ReSrc4.h
M68k_Nop.o:					Makefile ReSrc4.h
M68k_Not.o:					Makefile ReSrc4.h
M68k_Or.o:					Makefile ReSrc4.h
M68k_Ori.o:					Makefile ReSrc4.h
M68k_Pack.o:				Makefile ReSrc4.h
M68k_Pea.o:					Makefile ReSrc4.h
M68k_Reset.o:				Makefile ReSrc4.h
M68k_Rol.o:					Makefile ReSrc4.h
M68k_Ror.o:					Makefile ReSrc4.h
M68k_Roxl.o:				Makefile ReSrc4.h
M68k_Roxr.o:				Makefile ReSrc4.h
M68k_Rtd.o:					Makefile ReSrc4.h
M68k_Rte.o:					Makefile ReSrc4.h
M68k_Rtm.o:					Makefile ReSrc4.h
M68k_Rtr.o:					Makefile ReSrc4.h
M68k_Rts.o:					Makefile ReSrc4.h
M68k_Sbcd.o:				Makefile ReSrc4.h
M68k_Scc.o:					Makefile ReSrc4.h
M68k_Sub.o:					Makefile ReSrc4.h
M68k_Suba.o:				Makefile ReSrc4.h
M68k_Subi.o:				Makefile ReSrc4.h
M68k_Subq.o:				Makefile ReSrc4.h
M68k_Subx.o:				Makefile ReSrc4.h
M68k_Swap.o:				Makefile ReSrc4.h
M68k_Trap.o:				Makefile ReSrc4.h
M68k_Trapv.o:				Makefile ReSrc4.h
M68k_Tst.o:					Makefile ReSrc4.h
M68k_Unlk.o:				Makefile ReSrc4.h
M68k_Unpk.o:				Makefile ReSrc4.h

M68k__FAbs.o:				Makefile ReSrc4.h
M68k__FACos.o:				Makefile ReSrc4.h
M68k__FAdd.o:				Makefile ReSrc4.h
M68k__FASin.o:				Makefile ReSrc4.h
M68k__FATan.o:				Makefile ReSrc4.h
M68k__FATanh.o:				Makefile ReSrc4.h
M68k__FBcc.o:				Makefile ReSrc4.h
M68k__FCmp.o:				Makefile ReSrc4.h
M68k__FCos.o:				Makefile ReSrc4.h
M68k__FCosh.o:				Makefile ReSrc4.h
M68k__FDAbs.o:				Makefile ReSrc4.h
M68k__FDAdd.o:				Makefile ReSrc4.h
M68k__FDDiv.o:				Makefile ReSrc4.h
M68k__FDiv.o:				Makefile ReSrc4.h
M68k__FDMove.o:				Makefile ReSrc4.h
M68k__FDMul.o:				Makefile ReSrc4.h
M68k__FDNeg.o:				Makefile ReSrc4.h
M68k__FDSqrt.o:				Makefile ReSrc4.h
M68k__FDSub.o:				Makefile ReSrc4.h
M68k__FEtox.o:				Makefile ReSrc4.h
M68k__FEtoxm1.o:			Makefile ReSrc4.h
M68k__FGetexp.o:			Makefile ReSrc4.h
M68k__FGetman.o:			Makefile ReSrc4.h
M68k__FInt.o:				Makefile ReSrc4.h
M68k__FIntrz.o:				Makefile ReSrc4.h
M68k__FLog2.o:				Makefile ReSrc4.h
M68k__FLog10.o:				Makefile ReSrc4.h
M68k__FLogn.o:				Makefile ReSrc4.h
M68k__FLognp1.o:			Makefile ReSrc4.h
M68k__FMod.o:				Makefile ReSrc4.h
M68k__FMove.o:				Makefile ReSrc4.h
M68k__FMovecr.o:			Makefile ReSrc4.h
M68k__FMovem.o:				Makefile ReSrc4.h
M68k__FMul.o:				Makefile ReSrc4.h
M68k__FNeg.o:				Makefile ReSrc4.h
M68k__FRem.o:				Makefile ReSrc4.h
M68k__FSAbs.o:				Makefile ReSrc4.h
M68k__FSAdd.o:				Makefile ReSrc4.h
M68k__FScale.o:				Makefile ReSrc4.h
M68k__FScc.o:				Makefile ReSrc4.h
M68k__FSDiv.o:				Makefile ReSrc4.h
M68k__FSgldiv.o:			Makefile ReSrc4.h
M68k__FSglmul.o:			Makefile ReSrc4.h
M68k__FSin.o:				Makefile ReSrc4.h
M68k__FSincos.o:			Makefile ReSrc4.h
M68k__FSinh.o:				Makefile ReSrc4.h
M68k__FSMove.o:				Makefile ReSrc4.h
M68k__FSMul.o:				Makefile ReSrc4.h
M68k__FSNeg.o:				Makefile ReSrc4.h
M68k__FSqrt.o:				Makefile ReSrc4.h
M68k__FSSqrt.o:				Makefile ReSrc4.h
M68k__FSSub.o:				Makefile ReSrc4.h
M68k__FSub.o:				Makefile ReSrc4.h
M68k__FTan.o:				Makefile ReSrc4.h
M68k__FTanh.o:				Makefile ReSrc4.h
M68k__FTentox.o:			Makefile ReSrc4.h
M68k__FTst.o:				Makefile ReSrc4.h
M68k__FTwotox.o:			Makefile ReSrc4.h

###########################################################################
