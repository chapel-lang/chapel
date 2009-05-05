#
# $Id: Makefile,v 1.2 1997/08/28 20:53:32 pvmsrc Exp $
#

PVMDIR		=	../..
SDIR		=	$(PVMDIR)/gexamples

PVM_ARCH	=	OS2
CONFFILE	=	$(PVMDIR)/conf/$(PVM_ARCH).def
LIBDIR		=	$(PVMDIR)/lib/$(PVM_ARCH)


all:
	make FORT=g77 -f $(CONFFILE) -f $(SDIR)/Makefile.aimk all

clean:
	make -f $(CONFFILE) -f $(SDIR)/Makefile.aimk clean

