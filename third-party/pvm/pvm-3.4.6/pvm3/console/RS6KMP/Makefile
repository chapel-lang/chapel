#
# $Id: Makefile,v 1.5 1998/10/01 21:04:54 pvmsrc Exp $
#

PVMDIR		=	../..
SDIR		=	$(PVMDIR)/console

PVM_ARCH	=	RS6KMP
CONFFILE	=	$(PVMDIR)/conf/$(PVM_ARCH).def
LIBDIR		=	$(PVMDIR)/lib/$(PVM_ARCH)

#
install:	default $(LIBDIR)
		
default:    
	$(MAKE) PVMLIB=pvm3s -f $(CONFFILE) -f $(SDIR)/Makefile.aimk install

clean:
	$(MAKE) -f $(CONFFILE) -f $(SDIR)/Makefile.aimk clean
	
$(LIBDIR):
	mkdir $(LIBDIR)

