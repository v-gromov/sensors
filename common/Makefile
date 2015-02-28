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

PREFIX		?= arm-none-eabi
CC		= $(PREFIX)-gcc
LD		= $(PREFIX)-ld
OBJCOPY		= $(PREFIX)-objcopy
OBJDUMP		= $(PREFIX)-objdump
CONIODIR        = $(TOOLCHAIN_DIR)/libconio
USBSERIALDIR        = $(TOOLCHAIN_DIR)/libusbserial

# Uncomment this line if you want to use the installed (not local) library.
# TOOLCHAIN_DIR	= `dirname \`which $(CC)\``/../$(PREFIX)
TOOLCHAIN_DIR	= ../
CFLAGS		= -O0 -g -Wall -Wextra -I$(TOOLCHAIN_DIR) \
		  -I$(TOOLCHAIN_DIR)/libopenstm32 -fno-common \
                  -I$(CONIODIR) -I$(USBSERIALDIR) \
		  -mcpu=cortex-m3 -mthumb 

OBJS            = dataflash.o spi.o usart2.o service.o \
                  i2c.o fpga.o io.o power.o autotests.o fpgaregs.o

Q=

all: common 

common: $(OBJS) 
###	@printf "  LD      $(subst $(shell pwd)/,,$(@))\n"
#	$(Q)$(LD) $(LDFLAGS) $(OBJS) $(ADDLIBS) 

%.o: %.c Makefile
	@printf "  CC      $(subst $(shell pwd)/,,$(@))\n"
	$(Q)$(CC) $(CFLAGS) -o $@ -c $<

doc: Doxyfile Doxyfile.tmpl
	doxygen

clean:
	$(Q)rm -f *.o 
	$(Q)rm -rf doc

.PHONY: clean doc common libusb

