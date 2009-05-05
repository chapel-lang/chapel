#
# $Id: Makefile,v 1.5 1998/10/01 21:11:43 pvmsrc Exp $
#

PVMDIR		=	../..
SDIR		=	$(PVMDIR)/src

PVM_ARCH	=	PGON
CONFFILE	=	$(PVMDIR)/conf/$(PVM_ARCH).def
LIBDIR		=	$(PVMDIR)/lib/$(PVM_ARCH)


all:
	$(MAKE) -f $(CONFFILE) -f $(SDIR)/Makefile.aimk all-mpp 

install:	
	$(MAKE) -f $(CONFFILE) -f $(SDIR)/Makefile.aimk install-mpp

clean:
	$(MAKE) -f $(CONFFILE) -f $(SDIR)/Makefile.aimk clean

