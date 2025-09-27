
###########################################################################

.PHONY: all clean install strip

###########################################################################

# Flags used for compiling
CFLAGS		+= -I.
CFLAGS		+= -O2
CFLAGS		+= -Wall
CFLAGS		+= -Wextra
CFLAGS		+= -MMD
#CFLAGS		+= -DDEBUG

# Flags passed to gcc during linking
LDFLAGS		:= 

# Additional linker libraries
LIBS		:=

# Source Dirs
SRCDIRS		:= Resourcer

# Object Dir
OBJDIR		:= obj

# Binary Dir
BINDIR		:= bin

###########################################################################

# Loaders
LOADER_HUNK		:= 1

# Systems
SYSTEM_AMIGAOS3	:= 1

# CPUs
CPU_M68K		:= 1

###########################################################################

ifeq ($(LOADER_HUNK),1)
CFLAGS		+= -DSUPPORT_HUNK
SRCDIRS		+= Loader_Hunk
endif

ifeq ($(SYSTEM_AMIGAOS3),1)
CFLAGS		+= -DSUPPORT_AMIGAOS3
SRCDIRS		+= System_AmigaOS3
SRCDIRS		+= System_AmigaOS3/Lib_Amigaguide
SRCDIRS		+= System_AmigaOS3/Lib_Asl
SRCDIRS		+= System_AmigaOS3/Lib_Bullet
SRCDIRS		+= System_AmigaOS3/Lib_Colorwheel
SRCDIRS		+= System_AmigaOS3/Lib_Commodities
SRCDIRS		+= System_AmigaOS3/Lib_Console
SRCDIRS		+= System_AmigaOS3/Lib_Datatypes
SRCDIRS		+= System_AmigaOS3/Lib_Diskfont
SRCDIRS		+= System_AmigaOS3/Lib_Dos
SRCDIRS		+= System_AmigaOS3/Lib_DTClass
SRCDIRS		+= System_AmigaOS3/Lib_Exec
SRCDIRS		+= System_AmigaOS3/Lib_Expansion
SRCDIRS		+= System_AmigaOS3/Lib_Gadtools
SRCDIRS		+= System_AmigaOS3/Lib_Graphics
SRCDIRS		+= System_AmigaOS3/Lib_Icon
SRCDIRS		+= System_AmigaOS3/Lib_IFFParse
SRCDIRS		+= System_AmigaOS3/Lib_Input
SRCDIRS		+= System_AmigaOS3/Lib_Intuition
SRCDIRS		+= System_AmigaOS3/Lib_Keymap
SRCDIRS		+= System_AmigaOS3/Lib_Layers
SRCDIRS		+= System_AmigaOS3/Lib_Locale
SRCDIRS		+= System_AmigaOS3/Lib_Lowlevel
SRCDIRS		+= System_AmigaOS3/Lib_Mathffp
SRCDIRS		+= System_AmigaOS3/Lib_Mathieeedoubbas
SRCDIRS		+= System_AmigaOS3/Lib_Mathieeedoubtrans
SRCDIRS		+= System_AmigaOS3/Lib_Mathieeesingbas
SRCDIRS		+= System_AmigaOS3/Lib_Mathieeesingtrans
SRCDIRS		+= System_AmigaOS3/Lib_Mathtrans
SRCDIRS		+= System_AmigaOS3/Lib_Nonvolatile
SRCDIRS		+= System_AmigaOS3/Lib_Rexxsyslib
SRCDIRS		+= System_AmigaOS3/Lib_Translator
SRCDIRS		+= System_AmigaOS3/Lib_Utility
SRCDIRS		+= System_AmigaOS3/Lib_Workbench
SRCDIRS		+= System_AmigaOS3/Res_Battclock
SRCDIRS		+= System_AmigaOS3/Res_Battmem
SRCDIRS		+= System_AmigaOS3/Res_Cardres
SRCDIRS		+= System_AmigaOS3/Res_Cia
SRCDIRS		+= System_AmigaOS3/Res_Disk
SRCDIRS		+= System_AmigaOS3/Res_Misc
SRCDIRS		+= System_AmigaOS3/Res_Potgo
endif

ifeq ($(CPU_M68K),1)
CFLAGS		+= -DSUPPORT_M68K
SRCDIRS		+= CPU_M68k
SRCDIRS		+= CPU_M68k/_CPU
SRCDIRS		+= CPU_M68k/_FPU
SRCDIRS		+= CPU_M68k/_MMU
endif

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

all: $(BINDIR)/$(TARGET)

###########################################################################

SRCS		+= $(wildcard $(addsuffix /*.c, $(SRCDIRS)))
OBJS		:= $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))
DEPS		:= $(patsubst %.c,$(OBJDIR)/%.d,$(SRCS))

###########################################################################

$(BINDIR)/$(TARGET): $(OBJS) | $(BINDIR)
	$(CC) $(LDFLAGS) -o $(BINDIR)/$(TARGET) $(OBJS) $(LIBS) -Wl,--cref,-M,-Map=$(TARGET).map

$(BINDIR):
	$(MKDIR) $(BINDIR)

###########################################################################

-include $(DEPS)

###########################################################################

clean:
	$(RM) $(OBJS) $(DEPS) $(BINDIR)/$(TARGET)

strip:
	@$(LS)		$(BINDIR)/$(TARGET)
	@$(STRIP)	$(BINDIR)/$(TARGET)
	@$(LS)		$(BINDIR)/$(TARGET)

install:
	install -m 755 $(BINDIR)/$(TARGET) ~/bin

###########################################################################

$(OBJDIR)/%.o: %.c
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

###########################################################################
