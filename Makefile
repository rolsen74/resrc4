
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
SRCS		+= M68k__EffectiveAddress.c
SRCS		+= M68k__Library.c
SRCS		+= M68k__Lib_DiskFont.c
SRCS		+= M68k__Lib_Dos.c
SRCS		+= M68k__Lib_Exec.c
SRCS		+= M68k__Lib_Graphics.c
SRCS		+= M68k__Lib_Intuition.c
SRCS		+= Hunk.c
SRCS		+= LabelMagic.c
SRCS		+= MD5.c
SRCS		+= Nodes.c
SRCS		+= SaveSource.c
SRCS		+= Trace.c
SRCS		+= Trace_JumpTables.c

SRCS		+= M68k_Abcd.c
SRCS		+= M68k_Add.c
SRCS		+= M68k_Adda.c
SRCS		+= M68k_Addi.c
SRCS		+= M68k_Addq.c
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
SRCS		+= M68k_Clr.c
SRCS		+= M68k_Cmp.c
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
SRCS		+= M68k_Swap.c
SRCS		+= M68k_Trap.c
SRCS		+= M68k_Trapv.c
SRCS		+= M68k_Tst.c
SRCS		+= M68k_Unlk.c
SRCS		+= M68k_Unpk.c

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

else ifeq ($(detected_os),Linux)

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
M68k__EffectiveAddress.o:	Makefile ReSrc4.h
M68k__Library.o:			Makefile ReSrc4.h
M68k__Lib_DiskFont.o:		Makefile ReSrc4.h
M68k__Lib_Dos.o:			Makefile ReSrc4.h
M68k__Lib_Exec.o:			Makefile ReSrc4.h
M68k__Lib_Graphics.o:		Makefile ReSrc4.h
M68k__Lib_Intuition.o:		Makefile ReSrc4.h
MD5.o:						Makefile ReSrc4.h
Nodes.o:					Makefile ReSrc4.h
SaveSource.o:				Makefile ReSrc4.h
Trace.o:					Makefile ReSrc4.h
Trace_JumpTables.o:			Makefile ReSrc4.h

M68k_Abcd.o:				Makefile ReSrc4.h
M68k_Add.o:					Makefile ReSrc4.h
M68k_Adda.o:				Makefile ReSrc4.h
M68k_Addi.o:				Makefile ReSrc4.h
M68k_Addq.o:				Makefile ReSrc4.h
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
M68k_Clr.o:					Makefile ReSrc4.h
M68k_Cmp.o:					Makefile ReSrc4.h
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
M68k_Swap.o:				Makefile ReSrc4.h
M68k_Trap.o:				Makefile ReSrc4.h
M68k_Trapv.o:				Makefile ReSrc4.h
M68k_Tst.o:					Makefile ReSrc4.h
M68k_Unlk.o:				Makefile ReSrc4.h
M68k_Unpk.o:				Makefile ReSrc4.h

###########################################################################
