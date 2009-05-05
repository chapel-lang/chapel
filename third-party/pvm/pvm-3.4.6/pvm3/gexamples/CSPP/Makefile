#
# $Id: Makefile,v 1.4 1998/10/01 21:09:20 pvmsrc Exp $
#
# Makefile for PVM group library example programs.
#
# Set PVM_ROOT to the path where PVM includes and libraries are installed.
# Set PVM_ARCH to your architecture type (SUN4, HP9K, RS6K, SGI, etc.)
# Set ARCHLIB to any special libs needed on PVM_ARCH (-lrpc, -lsocket, etc.)
# otherwise leave ARCHLIB blank
#
# PVM_ARCH and ARCHLIB, among other things, are set for you if you
# use "$PVM_ROOT/lib/aimk" instead of "make".
#

PVM_ARCH	=	CSPP
ARCHLIB		=	-lcnx_syscall /lib/libail.sl

PVMDIR		=	../..
SDIR		=	$(PVMDIR)/gexamples
BDIR		=	$(PVMDIR)/bin
XDIR		=	$(BDIR)/$(PVM_ARCH)

PVMIDIR		=	$(PVMDIR)/include
PVMLDIR		=	$(PVMDIR)/lib/$(PVM_ARCH)
PVMLIB		=	-Wl,-aarchive -lgpvm3 -lpvm3 $(ARCHLIB)

CC			=	/usr/convex/bin/cc -Wl,+tmspp1
CFLAGS		=	$(CFLOPTS) -cxdb -no -I$(PVMIDIR) -L$(PVMLDIR)

FORT		=	/usr/convex/bin/fc -Wl,+tmspp1
FFLAGS		=	$(FFLOPTS) -cxdb -no -I$(PVMIDIR) -L$(PVMLDIR)
PVMFLIB		=	-Wl,-aarchive -lfpvm3 $(PVMLIB)

all: joinleave gexamp tst thb tnb ge tjl tjf trsg frsg gs

joinleave: $(SDIR)/joinleave.c
	$(CC) $(CFLAGS) -o joinleave $(SDIR)/joinleave.c $(PVMLIB)
	mv joinleave $(XDIR)

tst: $(SDIR)/tst.c
	$(CC) $(CFLAGS) -o tst $(SDIR)/tst.c $(PVMLIB)
	mv tst $(XDIR)

gs: $(SDIR)/gs.c
	$(CC) $(CFLAGS) -o gs $(SDIR)/gs.c $(PVMLIB)
	mv gs $(XDIR)

ge: $(SDIR)/ge.c
	$(CC) $(CFLAGS) -o ge $(SDIR)/ge.c $(PVMLIB)
	mv ge $(XDIR)

gexamp: $(SDIR)/gexamp.c
	$(CC) $(CFLAGS) -o gexamp $(SDIR)/gexamp.c $(PVMLIB)
	mv gexamp $(XDIR)

thb: $(SDIR)/thb.c
	$(CC) $(CFLAGS) -o thb $(SDIR)/thb.c $(PVMLIB)
	mv thb $(XDIR)

tnb: $(SDIR)/tnb.c
	$(CC) $(CFLAGS) -o tnb $(SDIR)/tnb.c $(PVMLIB)
	mv tnb $(XDIR)

tjl: $(SDIR)/tjl.c
	$(CC) $(CFLAGS) -o tjl $(SDIR)/tjl.c $(PVMLIB)
	mv tjl $(XDIR)

tjf: $(SDIR)/tjf.c
	$(CC) $(CFLAGS) -o tjf $(SDIR)/tjf.c $(PVMLIB)
	mv tjf $(XDIR)

trsg: $(SDIR)/trsg.c
	$(CC) $(CFLAGS) -o trsg $(SDIR)/trsg.c $(PVMLIB)
	mv trsg $(XDIR)

frsg: $(SDIR)/frsg.f
	$(FORT) $(FFLAGS) -o frsg $(SDIR)/frsg.f $(PVMFLIB)
	mv frsg $(XDIR)

clean:
	rm -f *.o tst thb tnb tjl tjf ge gexamp gs joinleave trsg frsg

