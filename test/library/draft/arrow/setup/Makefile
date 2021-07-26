# This makefile will be used to make arrow programs in Chapel
#  $@ and $^, which are the left and right sides of the :, respectively.
# $< is the first item after :

CHPL = chpl
CHPLFLAGS = -g
CC = clang
CFLAGS = -Wall -g

GLIB_INCLUDES = -I /usr/local/include/glib-2.0 -I /usr/local/lib/glib-2.0/include
GLIB_LINKS = -larrow-glib -lglib-2.0 -lgobject-2.0
ASAN = -fsanitize=address

default : all

.PHONY: all
all: cbuild build

%: %.chpl
	+$(CHPL) $(GLIB_INCLUDES) $(GLIB_LINKS) -o $@ $<

CBUILD_SRCS = cbuild.c
cbuild: $(CBUILD_SRCS)
	$(CC) $(CFLAGS) $(GLIB_INCLUDES) $(GLIB_LINKS) $(CBUILD_SRCS) -o $@

.PHONY: clean
clean:
	rm -rf *.o cbuild build