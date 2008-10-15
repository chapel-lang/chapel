#  
#  -- High Performance Computing Linpack Benchmark (HPL)                
#     HPL - 1.0a - January 20, 2004                          
#     Antoine P. Petitet                                                
#     University of Tennessee, Knoxville                                
#     Innovative Computing Laboratories                                 
#     (C) Copyright 2000-2004 All Rights Reserved                       
#                                                                       
#  -- Copyright notice and Licensing terms:                             
#                                                                       
#  Redistribution  and  use in  source and binary forms, with or without
#  modification, are  permitted provided  that the following  conditions
#  are met:                                                             
#                                                                       
#  1. Redistributions  of  source  code  must retain the above copyright
#  notice, this list of conditions and the following disclaimer.        
#                                                                       
#  2. Redistributions in binary form must reproduce  the above copyright
#  notice, this list of conditions,  and the following disclaimer in the
#  documentation and/or other materials provided with the distribution. 
#                                                                       
#  3. All  advertising  materials  mentioning  features  or  use of this
#  software must display the following acknowledgement:                 
#  This  product  includes  software  developed  at  the  University  of
#  Tennessee, Knoxville, Innovative Computing Laboratories.             
#                                                                       
#  4. The name of the  University,  the name of the  Laboratory,  or the
#  names  of  its  contributors  may  not  be used to endorse or promote
#  products  derived   from   this  software  without  specific  written
#  permission.                                                          
#                                                                       
#  -- Disclaimer:                                                       
#                                                                       
#  THIS  SOFTWARE  IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#  ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,  INCLUDING,  BUT NOT
#  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE UNIVERSITY
#  OR  CONTRIBUTORS  BE  LIABLE FOR ANY  DIRECT,  INDIRECT,  INCIDENTAL,
#  SPECIAL,  EXEMPLARY,  OR  CONSEQUENTIAL DAMAGES  (INCLUDING,  BUT NOT
#  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#  DATA OR PROFITS; OR BUSINESS INTERRUPTION)  HOWEVER CAUSED AND ON ANY
#  THEORY OF LIABILITY, WHETHER IN CONTRACT,  STRICT LIABILITY,  OR TORT
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
# ######################################################################
#  
include Make.inc
#
# ######################################################################
#
INCdep           = \
   $(INCdir)/hpl_misc.h   $(INCdir)/hpl_blas.h   $(INCdir)/hpl_auxil.h \
   $(INCdir)/hpl_pmisc.h  $(INCdir)/hpl_grid.h   $(INCdir)/hpl_pauxil.h
#
## Object files ########################################################
#
HPL_pauobj       = \
   HPL_indxg2l.o          HPL_indxg2lp.o         HPL_indxg2p.o          \
   HPL_indxl2g.o          HPL_infog2l.o          HPL_numroc.o           \
   HPL_numrocI.o          HPL_dlaswp00N.o        HPL_dlaswp10N.o        \
   HPL_dlaswp01N.o        HPL_dlaswp01T.o        HPL_dlaswp02N.o        \
   HPL_dlaswp03N.o        HPL_dlaswp03T.o        HPL_dlaswp04N.o        \
   HPL_dlaswp04T.o        HPL_dlaswp05N.o        HPL_dlaswp05T.o        \
   HPL_dlaswp06N.o        HPL_dlaswp06T.o        HPL_pwarn.o            \
   HPL_pabort.o           HPL_pdlaprnt.o         HPL_pdlamch.o          \
   HPL_pdlange.o
#
## Targets #############################################################
#
all     : lib 
#
lib     : lib.grd
#
lib.grd : $(HPL_pauobj)
	$(ARCHIVER) $(ARFLAGS) $(HPLlib) $(HPL_pauobj)
	$(RANLIB) $(HPLlib)
	$(TOUCH) lib.grd
#
# ######################################################################
#
HPL_indxg2l.o          : ../HPL_indxg2l.c          $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_indxg2l.c
HPL_indxg2lp.o         : ../HPL_indxg2lp.c         $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_indxg2lp.c
HPL_indxg2p.o          : ../HPL_indxg2p.c          $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_indxg2p.c
HPL_indxl2g.o          : ../HPL_indxl2g.c          $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_indxl2g.c
HPL_infog2l.o          : ../HPL_infog2l.c          $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_infog2l.c
HPL_numroc.o           : ../HPL_numroc.c           $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_numroc.c
HPL_numrocI.o          : ../HPL_numrocI.c          $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_numrocI.c
HPL_dlaswp00N.o        : ../HPL_dlaswp00N.c        $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_dlaswp00N.c
HPL_dlaswp10N.o        : ../HPL_dlaswp10N.c        $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_dlaswp10N.c
HPL_dlaswp01N.o        : ../HPL_dlaswp01N.c        $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_dlaswp01N.c
HPL_dlaswp01T.o        : ../HPL_dlaswp01T.c        $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_dlaswp01T.c
HPL_dlaswp02N.o        : ../HPL_dlaswp02N.c        $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_dlaswp02N.c
HPL_dlaswp03N.o        : ../HPL_dlaswp03N.c        $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_dlaswp03N.c
HPL_dlaswp03T.o        : ../HPL_dlaswp03T.c        $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_dlaswp03T.c
HPL_dlaswp04N.o        : ../HPL_dlaswp04N.c        $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_dlaswp04N.c
HPL_dlaswp04T.o        : ../HPL_dlaswp04T.c        $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_dlaswp04T.c
HPL_dlaswp05N.o        : ../HPL_dlaswp05N.c        $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_dlaswp05N.c
HPL_dlaswp05T.o        : ../HPL_dlaswp05T.c        $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_dlaswp05T.c
HPL_dlaswp06N.o        : ../HPL_dlaswp06N.c        $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_dlaswp06N.c
HPL_dlaswp06T.o        : ../HPL_dlaswp06T.c        $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_dlaswp06T.c
HPL_pwarn.o            : ../HPL_pwarn.c            $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_pwarn.c
HPL_pabort.o           : ../HPL_pabort.c           $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_pabort.c
HPL_pdlaprnt.o         : ../HPL_pdlaprnt.c         $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_pdlaprnt.c
HPL_pdlamch.o          : ../HPL_pdlamch.c          $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_pdlamch.c
HPL_pdlange.o          : ../HPL_pdlange.c          $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_pdlange.c
#
# ######################################################################
#
clean            :
	$(RM) *.o *.grd
#
# ######################################################################
