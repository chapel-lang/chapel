#
# $Id: Makefile,v 1.2 1997/08/28 20:54:08 pvmsrc Exp $
#

PVMDIR		=	../..
SDIR		=	$(PVMDIR)/libfpvm

PVM_ARCH	=	OS2
CONFFILE	=	$(PVMDIR)/conf/$(PVM_ARCH).def
LIBDIR		=	$(PVMDIR)/lib/$(PVM_ARCH)

#
install:	default $(LIBDIR)
		
default:    
	make LIBPREFIX="" -f $(CONFFILE) -f $(SDIR)/Makefile.aimk install

clean:
	make LIBPREFIX="" -f $(CONFFILE) -f $(SDIR)/Makefile.aimk clean
	
$(LIBDIR):
	mkdir $(LIBDIR)

