#
# $Id: Makefile,v 1.3 1997/05/08 16:08:11 pvmsrc Exp $
#
# Custom section
# Set ARCH to your architecture type (SUN4, HP9K, RS6K, SGI, etc)
# if ARCH = BSD386 then set ARCHLIB = -lrpc
# if ARCH = SGI    then set ARCHLIB = -lsun
# if ARCH = I860   then set ARCHLIB = -lrpc -lsocket
# if ARCH = IPSC2  then set ARCHLIB = -lrpc -lsocket
# otherwise leave ARCHLIB blank
#
# ARCH and ARCHLIB are set for you if you use 'aimk'.
#
#PVM_ARCH	=	PGON
ARCHLIB		=	-lrpc -lnx
# END of custom section - leave this line here
#
PVMDIR		=	../..
PVMLIB		=	$(PVMDIR)/lib/$(PVM_ARCH)/libpvm3.a
PVMPELIB	=	$(PVMDIR)/lib/$(PVM_ARCH)/libpvm3pe.a
SDIR		=	$(PVMDIR)/gexamples
BDIR		=	$(PVMDIR)/bin
XDIR		=	$(BDIR)/$(PVM_ARCH)

CC			=	icc
CFLOPTS		=	-O
CFLAGS		=	$(CFLOPTS) -I$(PVMDIR)/include
LIBS		=	$(PVMLIB) $(ARCHLIB)
NODELIBS	=	$(PVMPELIB) $(ARCHLIB)
GLIBS		=	$(PVMDIR)/lib/$(PVM_ARCH)/libgpvm3.a

F77     	=	if77
FFLOPTS		=	-O
FFLAGS		=	$(FFLOPTS)
FLIBS		=	$(PVMDIR)/lib/$(PVM_ARCH)/libfpvm3.a
FPELIBS		=	$(FLIBS)


default: gexamp

all: joinleave gexamp thb tnb trsg frsg

$(XDIR):	$(BDIR)
	- mkdir $(XDIR)

$(BDIR):
	- mkdir $(BDIR)


joinleave: $(SDIR)/joinleave.c
	$(CC) $(CFLAGS) -o joinleave $(SDIR)/joinleave.c $(GLIBS) $(NODELIBS)
	mv joinleave $(XDIR)

gexamp: $(SDIR)/gexamp.c
	$(CC) $(CFLAGS) -o gexamp $(SDIR)/gexamp.c $(GLIBS) $(NODELIBS)
	mv gexamp $(XDIR)

thb: $(SDIR)/thb.c
	$(CC) $(CFLAGS) -o thb $(SDIR)/thb.c $(GLIBS) $(NODELIBS)
	mv thb $(XDIR)

tnb: $(SDIR)/tnb.c
	$(CC) $(CFLAGS) -o tnb $(SDIR)/tnb.c $(GLIBS) $(NODELIBS)
	mv tnb $(XDIR)

trsg: $(SDIR)/trsg.c
	$(CC) $(CFLAGS) -o trsg $(SDIR)/trsg.c $(GLIBS) $(NODELIBS)
	mv trsg $(XDIR)

frsg: $(SDIR)/frsg.f
	$(F77) $(FFLAGS) -o frsg $(SDIR)/frsg.f $(FPELIBS) $(GLIBS) $(NODELIBS)
	mv frsg $(XDIR)

clean:
	rm -f *.o  thb tnb gexamp joinleave trsg frsg

