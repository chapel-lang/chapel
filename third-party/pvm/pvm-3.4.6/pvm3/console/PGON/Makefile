#
# $Id: Makefile,v 1.3 1998/10/01 21:04:34 pvmsrc Exp $
#

PVMDIR		=	../..
SDIR		=	$(PVMDIR)/console

PVM_ARCH	=	PGON
CONFFILE	=	$(PVMDIR)/conf/$(PVM_ARCH).def
LIBDIR		=	$(PVMDIR)/lib/$(PVM_ARCH)

#
install:	default $(LIBDIR)

default:
	$(MAKE) CC=icc -f $(CONFFILE) -f $(SDIR)/Makefile.aimk install

clean:
	$(MAKE) -f $(CONFFILE) -f $(SDIR)/Makefile.aimk clean

$(LIBDIR):
	mkdir $(LIBDIR)

