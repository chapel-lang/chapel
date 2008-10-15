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
   $(INCdir)/hpl_pmisc.h  $(INCdir)/hpl_grid.h   $(INCdir)/hpl_comm.h  \
   $(INCdir)/hpl_pauxil.h $(INCdir)/hpl_panel.h  $(INCdir)/hpl_pfact.h \
   $(INCdir)/hpl_pgesv.h
#
## Object files ########################################################
#
HPL_pgeobj       = \
   HPL_pipid.o            HPL_plindx0.o          HPL_pdlaswp00N.o       \
   HPL_pdlaswp00T.o       HPL_perm.o             HPL_logsort.o          \
   HPL_plindx10.o         HPL_plindx1.o          HPL_spreadN.o          \
   HPL_spreadT.o          HPL_rollN.o            HPL_rollT.o            \
   HPL_equil.o            HPL_pdlaswp01N.o       HPL_pdlaswp01T.o       \
   HPL_pdupdateNN.o       HPL_pdupdateNT.o       HPL_pdupdateTN.o       \
   HPL_pdupdateTT.o       HPL_pdtrsv.o           HPL_pdgesv0.o          \
   HPL_pdgesvK1.o         HPL_pdgesvK2.o         HPL_pdgesv.o
#
## Targets #############################################################
#
all     : lib 
#
lib     : lib.grd
#
lib.grd : $(HPL_pgeobj)
	$(ARCHIVER) $(ARFLAGS) $(HPLlib) $(HPL_pgeobj)
	$(RANLIB) $(HPLlib)
	$(TOUCH) lib.grd
#
# ######################################################################
#
HPL_pipid.o            : ../HPL_pipid.c            $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_pipid.c
HPL_plindx0.o          : ../HPL_plindx0.c          $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_plindx0.c
HPL_pdlaswp00N.o       : ../HPL_pdlaswp00N.c       $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_pdlaswp00N.c
HPL_pdlaswp00T.o       : ../HPL_pdlaswp00T.c       $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_pdlaswp00T.c
HPL_perm.o             : ../HPL_perm.c             $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_perm.c
HPL_logsort.o          : ../HPL_logsort.c          $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_logsort.c
HPL_plindx10.o         : ../HPL_plindx10.c         $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_plindx10.c
HPL_plindx1.o          : ../HPL_plindx1.c          $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_plindx1.c
HPL_spreadN.o          : ../HPL_spreadN.c          $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_spreadN.c
HPL_spreadT.o          : ../HPL_spreadT.c          $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_spreadT.c
HPL_rollN.o            : ../HPL_rollN.c            $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_rollN.c
HPL_rollT.o            : ../HPL_rollT.c            $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_rollT.c
HPL_equil.o            : ../HPL_equil.c            $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_equil.c
HPL_pdlaswp01N.o       : ../HPL_pdlaswp01N.c       $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_pdlaswp01N.c
HPL_pdlaswp01T.o       : ../HPL_pdlaswp01T.c       $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_pdlaswp01T.c
HPL_pdupdateNN.o       : ../HPL_pdupdateNN.c       $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_pdupdateNN.c
HPL_pdupdateNT.o       : ../HPL_pdupdateNT.c       $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_pdupdateNT.c
HPL_pdupdateTN.o       : ../HPL_pdupdateTN.c       $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_pdupdateTN.c
HPL_pdupdateTT.o       : ../HPL_pdupdateTT.c       $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_pdupdateTT.c
HPL_pdtrsv.o           : ../HPL_pdtrsv.c           $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_pdtrsv.c
HPL_pdgesv0.o          : ../HPL_pdgesv0.c          $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_pdgesv0.c
HPL_pdgesvK1.o         : ../HPL_pdgesvK1.c         $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_pdgesvK1.c
HPL_pdgesvK2.o         : ../HPL_pdgesvK2.c         $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_pdgesvK2.c
HPL_pdgesv.o           : ../HPL_pdgesv.c           $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_pdgesv.c
#
# ######################################################################
#
clean            :
	$(RM) *.o *.grd
#
# ######################################################################
