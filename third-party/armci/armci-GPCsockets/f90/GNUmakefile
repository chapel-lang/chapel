#$Id: GNUmakefile,v 1.2 2005-05-29 03:30:32 vinod Exp $
         FOPT = -g
         COPT = -O3

OBJ= armci_types.o arraywrap.o definekind.o armci_mem_f90.o armci_mov_f90.o armcif90.o armci_nbmov.o

#default message-passing library for ARMCI is MPI
ifndef MSG_COMMS
  MSG_COMMS=MPI
endif

#defines communication libs and symbols
include ../config/makemp.h
include ../config/makecoms.h


LIB_DEFINES += $(MP_DEFINES) $(COMM_DEFINES)
LIB_INCLUDES += -I. $(COMM_INCLUDES) $(MP_INCLUDES) -I../src -I$(CHASM_ROOT)/include
EXTRA_LIBS += -L$(LIB_DISTRIB)/$(TARGET) -larmci $(MP_LIBS) $(COMM_LIBS) -L$(CHASM_ROOT)/src/farray -lchasm

ifdef NOFORT
LIB_DEFINES += -DNOFORT
endif

ifdef REPORT_SHMMAX
LIB_DEFINES += -DREPORT_SHMMAX
endif

ifndef ARMCI_ROOT
ARMCI_ROOT = ../..
endif
#......................................................................
LIBRARY = libarmcif90.a
HARDCLEAN = yes

#add some extra Microsoft VC files to clean if any left 
NTSTUFF = *.ilk *.exe *.ncb
LIB_TARGETS = *.x *.mod ifc*  $(NTSTUFF)
LIB_DISTRIB = ../lib

COPT = -O3
include ../config/makefile.h

#OBJ += $(EXTRA_OBJ)
include ../config/makelib.h
LIBS += $(EXTRA_LIBS)
FOPT_REN += -Vaxlib
#FOPT = -O3
COPT = -O3

#......................................................................
.SUFFIXES: .c .f90 .o .x .p

%.x.p:%.x
	echo "`whoami` `hostname` 4 `pwd`/$(basename $@) /tmp" > $@

%.x:%.F90 testa_init.o checkput.o testfa_type.o testnbfa_type.o $(FULL_LIBRARY_PATH)
	$(FC) -g -I. $(FOPT_REN) $(MP_INCLUDES) $(MP_DEFINES) -o $@ $^ $(LIBS) -lm

%.o:%.f90
	$(FC) $(FOPT) $(MP_DEFINES) -c $*.f90

info:
	@echo "********************************************************"
	@echo compiling F90 Wrappers to ARMCI
ifndef CHASM_ROOT
	@echo "env CHASM_ROOT needs to be defined and it should point to your chasm installation"
	exit 1
endif
	@echo "using ARMCI_ROOT"  $(ARMCI_ROOT)
	@echo "using CHASM_ROOT"  $(CHASM_ROOT)
	@echo "********************************************************"
