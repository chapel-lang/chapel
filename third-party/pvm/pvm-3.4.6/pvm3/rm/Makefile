#
# $Id: Makefile,v 1.2 1997/07/21 18:06:31 pvmsrc Exp $
#

PVMDIR		=	$(PVM_ROOT)

#CC			=	cc
#CFLOPTS	=	-g

PVMOPTS		=	CC=$(CC) CFLOPTS=$(CFLOPTS) PVMDIR=$(PVMDIR)

default: local

install:
	$(PVMDIR)/lib/aimk $(PVMOPTS) install
	@ ctags -w *.c *.h &

local:
	$(PVMDIR)/lib/aimk $(PVMOPTS) local
	@ ctags -w *.c *.h &

objs:
	$(PVMDIR)/lib/aimk $(PVMOPTS) objs

again:
	$(PVMDIR)/lib/aimk $(PVMOPTS) again

clean:
	$(PVMDIR)/lib/aimk clean

veryclean:
	$(PVMDIR)/lib/aimk veryclean

