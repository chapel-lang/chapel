#
# $Id: Makefile,v 1.1 1997/08/29 14:04:40 pvmsrc Exp $
#

PVMDIR		=	../..
SDIR		=	$(PVMDIR)/tracer

PVM_ARCH	=	OS2
CONFFILE	=	$(PVMDIR)/conf/$(PVM_ARCH).def
LIBDIR		=	$(PVMDIR)/lib/$(PVM_ARCH)


install:
	make LIBPREFIX="" -f $(CONFFILE) -f $(SDIR)/Makefile.aimk install

clean:
	make LIBPREFIX="" -f $(CONFFILE) -f $(SDIR)/Makefile.aimk clean

