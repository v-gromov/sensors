DESTDIR		?= library
CC		= arm-none-eabi-gcc
AR		= arm-none-eabi-ar
TOOLCHAIN_DIR	= ../
CFLAGS		= -Os -g -Wall -Wextra -I. -fno-common \
		  -mcpu=cortex-m3 -mthumb -Wstrict-prototypes \
		  -I$(TOOLCHAIN_DIR) \
		  -I$(TOOLCHAIN_DIR)/libopenstm32
ARFLAGS		= rcs

LIBNAME = libstm32usb

SRCS = stm32_otgd_fs_cal.c stm32_otgd_fs_dev.c stm32_otgd_fs_int.c stm32_otgd_fs_pcd.c \
       stm32_usb_core.c stm32_usb_init.c stm32_usb_istr.c\
       stm32_usb_sil.c stm32_usb_pwr.c udelay.c 

OBJS = $(patsubst %.c,%.o,$(SRCS))

all: ${LIBNAME}.a

${LIBNAME}.a: $(OBJS)
	@printf "  AR      $(subst $(shell pwd)/,,$(@))\n"
	$(AR) $(ARFLAGS) $@ $^

doc_install: doc
	install -d $(DESTDIR)
	cp -r doc $(DESTDIR)/$(LIBNAME)

%.d: %.o

%.o: %.c Makefile
	@printf "  CC      $(subst $(shell pwd)/,,$(@))\n"
	$(CC) $(CFLAGS) -o $@ -c $<
	@$(CC) $(CFLAGS) -MMD -c $< -o $@


doc: README mainpage.dox Doxyfile Doxyfile.tmpl
	doxygen

mainpage.doc: README

README: README.texi
	makeinfo --no-headers --html $< > $@

	
clean:
	rm -f *.o
	rm -f libusb.a
	rm -rf doc README
	rm -f *.d

.PHONY: clean  doc

-include $(OBJS:.o=.d)
