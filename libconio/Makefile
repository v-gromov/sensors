DESTDIR		?= library
PREFIX		?= arm-none-eabi-
CC		= ${PREFIX}gcc
AR		= ${PREFIX}ar
CFLAGS		= -O3 -g -Wall -Wextra -I. -fno-common \
		  -mcpu=cortex-m3 -mthumb -Wstrict-prototypes 
ARFLAGS		= rcs

LIBNAME = libconio

SRCS = conio.c

OBJS = $(patsubst %.c,%.o,$(SRCS))

all: ${LIBNAME}.a

doc_install: doc
	install -d $(DESTDIR)
	cp -r doc $(DESTDIR)/$(LIBNAME)

$(LIBNAME).a: $(OBJS)
	@printf "  AR      $(subst $(shell pwd)/,,$(@))\n"
	$(AR) $(ARFLAGS) $@ $^

%.d: %.o

%.o: %.c Makefile
	@printf "  CC      $(subst $(shell pwd)/,,$(@))\n"
	$(CC) $(CFLAGS) -o $@ -c $<
	@$(CC) $(CFLAGS) -MD -c $< -o $@

doc: README mainpage.dox Doxyfile Doxyfile.tmpl
	doxygen

mainpage.doc: README

README: README.texi
	makeinfo --no-headers --html $< > $@

	
clean:
	rm -f *.o
	rm -f $(LIBNAME).a
	rm -rf doc README
	rm -f *.d

.PHONY: clean doc 

-include $(OBJS:.o=.d)
