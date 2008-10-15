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
   $(INCdir)/hpl_misc.h   $(INCdir)/hpl_blas.h
#
## Object files ########################################################
#
HPL_blaobj       = \
   HPL_dcopy.o            HPL_daxpy.o            HPL_dscal.o            \
   HPL_idamax.o           HPL_dgemv.o            HPL_dtrsv.o            \
   HPL_dger.o             HPL_dgemm.o            HPL_dtrsm.o
#
## Targets #############################################################
#
all     : lib 
#
lib     : lib.grd
#
lib.grd : $(HPL_blaobj)
	$(ARCHIVER) $(ARFLAGS) $(HPLlib) $(HPL_blaobj)
	$(RANLIB) $(HPLlib)
	$(TOUCH) lib.grd
#
# ######################################################################
#
HPL_dcopy.o            : ../HPL_dcopy.c            $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_dcopy.c
HPL_daxpy.o            : ../HPL_daxpy.c            $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_daxpy.c
HPL_dscal.o            : ../HPL_dscal.c            $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_dscal.c
HPL_idamax.o           : ../HPL_idamax.c           $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_idamax.c
HPL_dgemv.o            : ../HPL_dgemv.c            $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_dgemv.c
HPL_dtrsv.o            : ../HPL_dtrsv.c            $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_dtrsv.c
HPL_dger.o             : ../HPL_dger.c             $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_dger.c
HPL_dgemm.o            : ../HPL_dgemm.c            $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_dgemm.c
HPL_dtrsm.o            : ../HPL_dtrsm.c            $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_dtrsm.c
#
# ######################################################################
#
clean            :
	$(RM) *.o *.grd
#
# ######################################################################
