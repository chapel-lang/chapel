#
# $Id: Makefile,v 1.6 1998/10/01 21:12:55 pvmsrc Exp $
#

PVMDIR		=	../..
SDIR		=	$(PVMDIR)/src

PVM_ARCH	=	SP2MPI
CONFFILE	=	$(PVMDIR)/conf/$(PVM_ARCH).def
LIBDIR		=	$(PVMDIR)/lib/$(PVM_ARCH)


all:
	$(MAKE) -f $(CONFFILE) -f $(SDIR)/Makefile.aimk all-mpp 

install:	
	$(MAKE) -f $(CONFFILE) -f $(SDIR)/Makefile.aimk install-mpp

clean:
	$(MAKE) -f $(CONFFILE) -f $(SDIR)/Makefile.aimk clean

