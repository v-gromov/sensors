##
## This file is part of the libopenstm32 project.
##
## Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
##
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.
##

HWTEST		= hwtest

PREFIX		?= arm-none-eabi
CC		= $(PREFIX)-gcc
LD		= $(PREFIX)-ld
OBJCOPY		= $(PREFIX)-objcopy
OBJDUMP		= $(PREFIX)-objdump

# Uncomment this line if you want to use the installed (not local) library.
# TOOLCHAIN_DIR	= `dirname \`which $(CC)\``/../$(PREFIX)
TOOLCHAIN_DIR    = ../
COMMON_DIR       = $(TOOLCHAIN_DIR)/common
CONIO_DIR        = $(TOOLCHAIN_DIR)/libconio
USB_DIR          = $(TOOLCHAIN_DIR)/libstm32usb
USBSERIAL_DIR    = $(TOOLCHAIN_DIR)/libusbserial

CFLAGS                = -O0 -g -Wall -Wextra -I$(TOOLCHAIN_DIR) \
                  -I$(TOOLCHAIN_DIR)/libopenstm32 -fno-common \
		  -I$(CONIO_DIR) -I$(USBSERIAL_DIR) -I$(USB_DIR) \
                  -mcpu=cortex-m3 -mthumb 

ifeq "$(RAM)" "1"
CFLAGS		+= -D_LINK_RAM_
LDSCRIPT	= $(TOOLCHAIN_DIR)/libopenstm32/libopenstm32.ram.ld
else
LDSCRIPT	= $(TOOLCHAIN_DIR)/libopenstm32/libopenstm32.ld
endif

LDFLAGS         = -L$(TOOLCHAIN_DIR)/libopenstm32 \
                  -L/usr/arm-none-eabi/lib/thumb \
                  -L../\
		  -L$(CONIO_DIR) -L$(USBSERIAL_DIR) -L$(USB_DIR) \
                  -T$(LDSCRIPT) -nostdlib -nostartfiles

ADDLIBS         = -lc -lopenstm32 -lstm32usb -lusbserial -lconio

OBJS            = $(COMMON_DIR)/*.o gpio.o io.o

OBJS_HWTEST     = $(HWTEST).o $(OBJS)

Q=

all: $(HWTEST) 

$(HWTEST): cmdlist.h $(OBJS_HWTEST) $(LDSCRIPT)
	@printf "  LD      $(subst $(shell pwd)/,,$(@))\r\n"
	$(Q)$(LD) $(LDFLAGS) $(OBJS_HWTEST) --start-group $(ADDLIBS) \
	          --end-group -o $@
	$(OBJCOPY) -Obinary $@ $@.bin
	$(OBJDUMP) -S $@ > $@.list

%.d: %.o

%.o: %.c Makefile
	@printf "  CC      $(subst $(shell pwd)/,,$(@))\n"
	$(Q)$(CC) $(CFLAGS) -o $@ -c $<
	@$(CC) $(CFLAGS) -MD -c $< -o $@

cmdlist.h: cmdlist.txt
	echo "char* cmdlist = "   > $@
	sed 's/^/"/;s/$$/\\r\\n"/' $< >> $@
	echo ";"                  >> $@

doc: Doxyfile Doxyfile.tmpl
	doxygen

clean:
	$(Q)rm -f *.o *.d
	$(Q)rm -f $(HWTEST) 
	$(Q)rm -f *.bin *.list *.*~
	$(Q)rm -f cmdlist.h
	$(Q)rm -rf doc

.PHONY: clean doc

-include $(OBJS_HWTEST:.o=.d)
