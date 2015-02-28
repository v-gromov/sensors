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

PREFIX	?= arm-none-eabi
# PREFIX		?= arm-elf
DESTDIR		?= /usr
INCDIR		= $(DESTDIR)/$(PREFIX)/include
LIBDIR		= $(DESTDIR)/$(PREFIX)/libopenstm32
COMMONDIR       = ./common 
INSTALL		= install

COMMON          = common

# Be silent per default, but 'make V=1' will show all compiler calls.
ifneq ($(V),1)
Q := @
# Do not print "Entering directory ...".
MAKEFLAGS += --no-print-directory
endif

all: build

CARDS = e1hwtest

build: libopenstm32 libconio $(COMMON) $(CARDS) 

libstm32usb:
	@printf "  BUILD   libstm32usb\n"
	$(Q)$(MAKE) -C libstm32usb all

libusbserial: libstm32usb
	@printf "  BUILD   libusbserial\n"
	$(Q)$(MAKE) -C libusbserial all

libconio: libusbserial 
	@printf "  BUILD   libconio\n"
	$(Q)$(MAKE) -C libconio all

libopenstm32:
	@printf "  BUILD   libopenstm32\n"
ifeq "$(RAM)" "1"
	$(Q)$(MAKE) -C libopenstm32 RAM=1 all
else
	$(Q)$(MAKE) -C libopenstm32 all
endif

$(COMMON): libopenstm32 libconio
	@printf "  BUILD   common files\n"
	$(Q)$(MAKE) -C common all

e1hwtest: $(COMMON)
	@printf "  BUILD   e1hwtest\n"
ifeq "$(RAM)" "1"
	$(Q)$(MAKE) -C e1hwtest RAM=1 hwtest
else
	$(Q)$(MAKE) -C e1hwtest hwtest
endif

e1:
	make CARD=E1

dcom: 
	make CARD=DCOM

otdr:
	make CARD=OTDR

gbe: 
	make CARD=GBE


doc:    README
	$(Q)$(MAKE) -C dcomhwtest doc 
	$(Q)$(MAKE) -C e1hwtest doc
	$(Q)$(MAKE) -C otdrhwtest doc

README: README.texi
	makeinfo --plaintext $< > $@

install: build
	@printf "  INSTALL headers\n"
	$(Q)$(INSTALL) -d $(INCDIR)/libopenstm32
	$(Q)$(INSTALL) -d $(LIBDIR)
	$(Q)$(INSTALL) -m 0644 include/libopenstm32.h $(INCDIR)
	$(Q)$(INSTALL) -m 0644 include/libopenstm32/*.h $(INCDIR)/libopenstm32
	@printf "  INSTALL libopenstm32\n"
	$(Q)$(INSTALL) -m 0644 libopenstm32/*.a $(LIBDIR)
	@printf "  INSTALL ldscript\n"
	$(Q)$(INSTALL) -m 0644 libopenstm32/*.ld $(LIBDIR)

clean:
	$(Q)$(MAKE) -C e1hwtest clean
	$(Q)$(MAKE) -C libopenstm32 clean
	$(Q)$(MAKE) -C libstm32usb clean
	$(Q)$(MAKE) -C common clean
	rm -f README

.PHONY: build libopenstm32 libstm32usb e1hwtest common floader install clean

