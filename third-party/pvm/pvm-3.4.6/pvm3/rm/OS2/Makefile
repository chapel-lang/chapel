#
# $Id: Makefile,v 1.1 1997/08/28 21:19:42 pvmsrc Exp $
#

PVMDIR		=	../..
SDIR		=	$(PVMDIR)/rm

PVM_ARCH	=	OS2
CONFFILE	=	$(PVMDIR)/conf/$(PVM_ARCH).def
LIBDIR		=	$(PVMDIR)/lib/$(PVM_ARCH)


all:
	make LIBPREFIX="" -f $(CONFFILE) -f $(SDIR)/Makefile.aimk all

install:
	make LIBPREFIX="" -f $(CONFFILE) -f $(SDIR)/Makefile.aimk install

clean:
	make -f $(CONFFILE) -f $(SDIR)/Makefile.aimk clean

