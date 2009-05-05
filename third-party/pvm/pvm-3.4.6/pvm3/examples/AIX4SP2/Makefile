#
# $Id: Makefile,v 1.1 1998/02/23 21:33:57 pvmsrc Exp $
#

PVMDIR      =   ../..

SDIR        =   $(PVMDIR)/examples

CONFFILE    =   $(PVMDIR)/conf/$(PVM_ARCH).def

LIBDIR      =   $(PVMDIR)/lib/$(PVM_ARCH)

include ${CONFFILE}

#### Example programs that can be built by ../Makefile.aimk

EXAMPLES = 	hello hello_other \
			master1 slave1 fmaster1 fslave1 \
			spmd fspmd \
			dbwtest ibwtest pbwtest rbwtest \
			timing timingh timing_slave \
			hitc hitc_slave \
			gexample fgexample \
			gmbi \
			lmbi

HEXAMPLES = helloh \
			master1h \
			timingh \
			gexampleh fgexampleh \
			gmbih \
			hitch
#### Targets that are NOT example programs but are recognized by
#  ../Makefile.aimk

AIMKTARGETS = default all c-all f-all hostprogs clean 

#### How to build programs using ../Makefile.aimk. Notice that 
#  		PVMLIB and PVMHLIB are set for MPPs (lpvm3pe, lpvm3, respectively). 
#  		Hence the default examples are built for MPP nodes.

${EXAMPLES}:
	make -f $(CONFFILE) PVMLIB=-lpvm3pe PVMHLIB=-lpvm3 FORT=${NODEFORT}\
		CC=${NODECC} -f $(SDIR)/Makefile.aimk $@ 

${HEXAMPLES} ${AIMKTARGETS}:
	make -f $(CONFFILE) PVMLIB=-lpvm3pe PVMHLIB=-lpvm3 FORT=${FORTRAN}\
		CC=${CC} -f $(SDIR)/Makefile.aimk $@ 

