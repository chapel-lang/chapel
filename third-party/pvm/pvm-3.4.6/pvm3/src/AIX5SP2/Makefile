#
# $Id: Makefile,v 1.1 2004/01/14 17:34:27 pvmsrc Exp $
#

PVMDIR		=	../..
SDIR		=	$(PVMDIR)/src

PVM_ARCH	=	AIX5SP2
CONFFILE	=	$(PVMDIR)/conf/$(PVM_ARCH).def
LIBDIR		=	$(PVMDIR)/lib/$(PVM_ARCH)


all:
	make -f $(CONFFILE) -f $(SDIR)/Makefile.aimk all-mpp 

install:	
	make -f $(CONFFILE) -f $(SDIR)/Makefile.aimk install-mpp

clean:
	make -f $(CONFFILE) -f $(SDIR)/Makefile.aimk clean

