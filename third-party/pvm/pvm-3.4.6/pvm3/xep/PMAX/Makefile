#
# $Id: Makefile,v 1.3 1997/05/08 16:15:46 pvmsrc Exp $
#

#
# Generic Makefile body to be concatenated to config header.
#
# Imports:
#  PVM_ARCH   = the official pvm-name of your processor
#  ARCHCFLAGS = special cc flags
#  ARCHLIB    = special libs needed for daemon
#
PVMDIR	=	../..
PVMIDIR	=	$(PVMDIR)/include
PVMLDIR	=	$(PVMDIR)/lib/$(PVM_ARCH)
PVMLIB	=	-lpvm3
SDIR	=	$(PVMDIR)/xep
BDIR	=	$(PVMDIR)/bin
XDIR	=	$(BDIR)/$(PVM_ARCH)

CFLOPTS	=	-O
CFLAGS	=	$(CFLOPTS) -I$(PVMIDIR) -L$(PVMLDIR) $(ARCHCFLAGS)
LIBS	=	$(PVMLIB) $(ARCHLIB)
XLIBS	=	-lXaw -lXmu-mit -lXext-mit -lXt-mit -lX11-mit
XCFLAGS	=	-I/usr/include/mit

default:	mtile xep

all:	mmain mtile xep

$(XDIR):
	- mkdir $(BDIR)
	- mkdir $(XDIR)

mmain:	$(SDIR)/mmain.c $(XDIR)
	$(CC) $(CFLAGS) -o mmain $(SDIR)/mmain.c $(LIBS)
	mv mmain $(XDIR)

mtile:	$(SDIR)/mtile.c $(XDIR)
	$(CC) $(CFLAGS) -o mtile $(SDIR)/mtile.c $(LIBS) -lm
	mv mtile $(XDIR)

xep:	$(SDIR)/xep.c $(XDIR)
	$(CC) $(XCFLAGS) $(CFLAGS) -o xep $(SDIR)/xep.c $(XLIBS) $(LIBS) -lm
	mv xep $(XDIR)

clean:
	rm -f mmain mtile xep *.o

