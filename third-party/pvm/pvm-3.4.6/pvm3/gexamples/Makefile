#
# $Id: Makefile,v 1.4 1998/10/01 21:09:05 pvmsrc Exp $
#
# Makefile for PVM group library example programs.
#
# Set PVM_ROOT to the path where PVM includes and libraries are installed.
# Set PVM_ARCH to your architecture type (SUN4, HP9K, RS6K, SGI, etc.)
# Set ARCHLIB to any special libs needed on PVM_ARCH (-lrpc, -lsocket, etc.)
# otherwise leave ARCHLIB blank
#
# PVM_ARCH and ARCHLIB, among other things,  are set for you if you
# use "$PVM_ROOT/lib/aimk" instead of "make".
#

#PVM_ROOT	=
#PVM_ARCH	=
#ARCHCFLAGS	=

SHELL	=	/bin/sh

PVMDIR	=	$(PVM_ROOT)
SDIR	=	$(PVMDIR)/gexamples
PVMIDIR	=	$(PVMDIR)/include
PVMLDIR	=	$(PVMDIR)/lib/$(PVM_ARCH)
PVMLIB	=	-lgpvm3 -lpvm3
CFLOPTS	=	-g
CFLAGS	=	$(CFLOPTS) -I$(PVMIDIR) -L$(PVMLDIR) $(ARCHCFLAGS)
#BDIR	=	$(HOME)/pvm3/bin
BDIR	=	$(PVMDIR)/bin
XDIR	=	$(BDIR)/$(PVM_ARCH)
F77		=	f77
FFLOPTS	=	-g
FFLAGS	=	$(FFLOPTS)
PVMFLIB	=	-lfpvm3 $(PVMLIB)

all: gexamp tst thb tnb ge tjl tjf gs

joinleave: $(SDIR)/joinleave.c
	$(CC) $(CFLAGS) -o joinleave joinleave.c $(PVMLIB)

tst: $(SDIR)/tst.c
	$(CC) $(CFLAGS) -o tst tst.c $(PVMLIB)
	cp tst $(XDIR)

gs: $(SDIR)/gs.c
	$(CC) $(CFLAGS) -o gs gs.c $(PVMLIB)
	cp gs $(XDIR)

ge: $(SDIR)/ge.c
	$(CC) $(CFLAGS) -o ge ge.c $(PVMLIB)
	cp ge $(XDIR)

gexamp: $(SDIR)/gexamp.c
	$(CC) $(CFLAGS) -o gexamp gexamp.c $(PVMLIB)
	cp gexamp $(XDIR)

thb: $(SDIR)/thb.c
	$(CC) $(CFLAGS) -o thb thb.c $(PVMLIB)
	cp thb $(XDIR)

tnb: $(SDIR)/tnb.c
	$(CC) $(CFLAGS) -o tnb tnb.c $(PVMLIB)
	cp tnb $(XDIR)

tjl: $(SDIR)/tjl.c
	$(CC) $(CFLAGS) -o tjl tjl.c $(PVMLIB)
	cp tjl $(XDIR)

tjf: $(SDIR)/tjf.c
	$(CC) $(CFLAGS) -o tjf tjf.c $(PVMLIB)
	cp tjf $(XDIR)

trsg: $(SDIR)/trsg.c
	$(CC) $(CFLAGS) -o trsg trsg.c $(PVMLIB)
	cp trsg $(XDIR)

frsg: $(SDIR)/frsg.f
	$(F77) $(FFLAGS) -o frsg frsg.f $(PVMFLIB)
	cp frsg $(XDIR)

clean:
	rm -f *.o tst thb tnb tjl tjf ge gexamp gs joinleave trsg frsg

