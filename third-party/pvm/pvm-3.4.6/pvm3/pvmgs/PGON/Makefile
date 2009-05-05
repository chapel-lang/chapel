#
# $Id: Makefile,v 1.5 1998/10/01 21:10:05 pvmsrc Exp $
#

PVMDIR		=	../..
SDIR		=	$(PVMDIR)/pvmgs

PVM_ARCH	=	PGON
CONFFILE	=	$(PVMDIR)/conf/$(PVM_ARCH).def
LIBDIR		=	$(PVMDIR)/lib/$(PVM_ARCH)

#
install:	default $(LIBDIR)
		
default:    
	$(MAKE) -f $(CONFFILE) -f $(SDIR)/Makefile.aimk install-mpp

clean:
	$(MAKE) -f $(CONFFILE) -f $(SDIR)/Makefile.aimk clean
	
$(LIBDIR):
	mkdir $(LIBDIR)

