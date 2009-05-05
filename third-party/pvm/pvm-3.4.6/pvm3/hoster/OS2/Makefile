#
# $Id: Makefile,v 1.2 1997/08/28 20:53:49 pvmsrc Exp $
#

PVMDIR		=	../..
SDIR		=	$(PVMDIR)/hoster

PVM_ARCH	=	OS2
CONFFILE	=	$(PVMDIR)/conf/$(PVM_ARCH).def
LIBDIR		=	$(PVMDIR)/lib/$(PVM_ARCH)


all:
	make LIBPREFIX="" REXECOBJ="rexec.o ruserpas.o" \
		-f $(CONFFILE) -f $(SDIR)/Makefile.aimk all

clean:
	make -f $(CONFFILE) -f $(SDIR)/Makefile.aimk clean

