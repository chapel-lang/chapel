#
# $Id: Makefile,v 1.4 1997/09/11 20:33:53 pvmsrc Exp $
#

ARCH	=	PGON
#
# Generic Makefile body.
# To update, edit Makefile.body and run lib/UpdateMk.
#
# Imports:
#  ARCH       = the official pvm-name of your processor
#  ARCHCFLAGS = special cc flags
#  ARCHLIB    = special libs needed for daemon
#
PVMDIR	=	../..
PVMIDIR	=	$(PVMDIR)/include
PVMLDIR	=	$(PVMDIR)/lib/$(ARCH)
PVMLIB	=	-lpvm3
ARCHLIB	=	-lrpc -lnx
SDIR	=	$(PVMDIR)/xep
BDIR	=	$(PVMDIR)/bin
# on cross compiler host
# XIDIR	=	/usr/local/paragon/include/X11
# XLDIR	=	/usr/local/paragon/lib-coff
# on paragon set
XIDIR	=	/usr/include/X11
XLDIR	=	/usr/lib
XDIR	=	$(BDIR)/$(ARCH)

CC		=	icc
CFLOPTS	=	-O
CFLAGS	=	$(CFLOPTS) -I$(PVMIDIR) -L$(PVMLDIR) $(ARCHCFLAGS)
LIBS	=	$(PVMLIB) $(ARCHLIB)
XLIBS	=	-lXaw -lXmu -lXt -lXext -lX11
XCFLAGS	=	-I$(XIDIR) -I$(XLDIR)

default:	mtile xep

all:	mmain mtile xep

$(XDIR):	$(BDIR)
	- mkdir $(XDIR)

$(BDIR):
	- mkdir $(BDIR)

mmain:	$(SDIR)/mmain.c $(XDIR)
	$(CC) $(CFLAGS) -o mmain $(SDIR)/mmain.c $(LIBS)
	mv mmain $(XDIR)

mtile:	$(SDIR)/mtile.c $(XDIR)
	$(CC) $(CFLAGS) -o mtile $(SDIR)/mtile.c -lpvm3pe $(ARCHLIB) -lm
	mv mtile $(XDIR)

xep:	xep.o calc.o imp.o hostc.o $(XDIR)
	$(CC) $(XCFLAGS) $(CFLAGS) -o xep xep.o calc.o imp.o hostc.o \
		$(XLIBS) $(LIBS) -lm
	mv xep $(XDIR)

xep.o:	$(SDIR)/xep.c $(HEADERS)
	$(CC) $(XCFLAGS) $(CFLAGS) -c $(SDIR)/xep.c

imp.o:	$(SDIR)/imp.c $(HEADERS)
	$(CC) $(XCFLAGS) $(CFLAGS) -c $(SDIR)/imp.c

calc.o:	$(SDIR)/calc.c $(HEADERS)
	$(CC) $(XCFLAGS) $(CFLAGS) -c $(SDIR)/calc.c

hostc.o:	$(SDIR)/hostc.c $(HEADERS)
	$(CC) $(XCFLAGS) $(CFLAGS) -c $(SDIR)/hostc.c

clean:
	rm -f mmain mtile xep *.o

