#
# $Id: Makefile,v 1.4 1998/10/01 21:11:16 pvmsrc Exp $
#

PVMDIR		=	../..
SDIR		=	$(PVMDIR)/src

PVM_ARCH	=	CSPP
CONFFILE	=	$(PVMDIR)/conf/$(PVM_ARCH).def
LIBDIR		=	$(PVMDIR)/lib/$(PVM_ARCH)


all:
	$(MAKE) -f $(CONFFILE) -f $(SDIR)/Makefile.aimk all-shm

install:
	$(MAKE) -f $(CONFFILE) -f $(SDIR)/Makefile.aimk install-shm

clean:
	$(MAKE) -f $(CONFFILE) -f $(SDIR)/Makefile.aimk clean

