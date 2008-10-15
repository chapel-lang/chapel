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
   $(INCdir)/hpl_misc.h  $(INCdir)/hpl_blas.h   $(INCdir)/hpl_auxil.h \
   $(INCdir)/hpl_pmisc.h $(INCdir)/hpl_pauxil.h $(INCdir)/hpl_pfact.h
#
## Object files ########################################################
#
HPL_pfaobj       = \
   HPL_dlocmax.o          HPL_dlocswpN.o         HPL_dlocswpT.o         \
   HPL_pdmxswp.o          HPL_pdpancrN.o         HPL_pdpancrT.o         \
   HPL_pdpanllN.o         HPL_pdpanllT.o         HPL_pdpanrlN.o         \
   HPL_pdpanrlT.o         HPL_pdrpanllN.o        HPL_pdrpanllT.o        \
   HPL_pdrpancrN.o        HPL_pdrpancrT.o        HPL_pdrpanrlN.o        \
   HPL_pdrpanrlT.o        HPL_pdfact.o
#
## Targets #############################################################
#
all              : lib 
#
lib              : lib.grd
#
lib.grd          : $(HPL_pfaobj)
	$(ARCHIVER) $(ARFLAGS) $(HPLlib) $(HPL_pfaobj)
	$(RANLIB) $(HPLlib)
	$(TOUCH) lib.grd
#
# ######################################################################
#
HPL_dlocmax.o          : ../HPL_dlocmax.c          $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_dlocmax.c
HPL_dlocswpN.o         : ../HPL_dlocswpN.c         $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_dlocswpN.c
HPL_dlocswpT.o         : ../HPL_dlocswpT.c         $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_dlocswpT.c
HPL_pdmxswp.o          : ../HPL_pdmxswp.c          $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_pdmxswp.c
HPL_pdpancrN.o         : ../HPL_pdpancrN.c         $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_pdpancrN.c
HPL_pdpancrT.o         : ../HPL_pdpancrT.c         $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_pdpancrT.c
HPL_pdpanllN.o         : ../HPL_pdpanllN.c         $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_pdpanllN.c
HPL_pdpanllT.o         : ../HPL_pdpanllT.c         $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_pdpanllT.c
HPL_pdpanrlN.o         : ../HPL_pdpanrlN.c         $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_pdpanrlN.c
HPL_pdpanrlT.o         : ../HPL_pdpanrlT.c         $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_pdpanrlT.c
HPL_pdrpanllN.o        : ../HPL_pdrpanllN.c        $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_pdrpanllN.c
HPL_pdrpanllT.o        : ../HPL_pdrpanllT.c        $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_pdrpanllT.c
HPL_pdrpancrN.o        : ../HPL_pdrpancrN.c        $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_pdrpancrN.c
HPL_pdrpancrT.o        : ../HPL_pdrpancrT.c        $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_pdrpancrT.c
HPL_pdrpanrlN.o        : ../HPL_pdrpanrlN.c        $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_pdrpanrlN.c
HPL_pdrpanrlT.o        : ../HPL_pdrpanrlT.c        $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_pdrpanrlT.c
HPL_pdfact.o           : ../HPL_pdfact.c           $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_pdfact.c
#
# ######################################################################
#
clean            :
	$(RM) *.o *.grd
#
# ######################################################################
