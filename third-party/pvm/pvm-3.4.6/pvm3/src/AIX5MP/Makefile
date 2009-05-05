#
# $Id: Makefile,v 1.1 2004/01/14 17:32:09 pvmsrc Exp $
#

PVMDIR		=	../..
SDIR		=	$(PVMDIR)/src

PVM_ARCH	=	AIX5MP
CONFFILE	=	$(PVMDIR)/conf/$(PVM_ARCH).def
LIBDIR		=	$(PVMDIR)/lib/$(PVM_ARCH)


all:
	$(MAKE) -f $(CONFFILE) -f $(SDIR)/Makefile.aimk all-shm

install:
	$(MAKE) -f $(CONFFILE) -f $(SDIR)/Makefile.aimk install-shm

clean:
	$(MAKE) -f $(CONFFILE) -f $(SDIR)/Makefile.aimk clean

