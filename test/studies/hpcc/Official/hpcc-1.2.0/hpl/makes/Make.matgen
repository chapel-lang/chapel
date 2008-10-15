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
   $(INCdir)/hpl_misc.h   $(INCdir)/hpl_blas.h  $(INCdir)/hpl_auxil.h  \
   $(INCdir)/hpl_matgen.h
#
## Object files ########################################################
#
HPL_matobj       = \
   HPL_dmatgen.o          HPL_ladd.o             HPL_lmul.o             \
   HPL_xjumpm.o           HPL_jumpit.o           HPL_rand.o             \
   HPL_setran.o
#
## Targets #############################################################
#
all     : lib 
#
lib     : lib.grd
#
lib.grd : $(HPL_matobj)
	$(ARCHIVER) $(ARFLAGS) $(HPLlib) $(HPL_matobj)
	$(RANLIB) $(HPLlib)
	$(TOUCH) lib.grd
#
# ######################################################################
#
HPL_dmatgen.o          : ../HPL_dmatgen.c          $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_dmatgen.c
HPL_ladd.o             : ../HPL_ladd.c             $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_ladd.c
HPL_lmul.o             : ../HPL_lmul.c             $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_lmul.c
HPL_xjumpm.o           : ../HPL_xjumpm.c           $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_xjumpm.c
HPL_jumpit.o           : ../HPL_jumpit.c           $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_jumpit.c
HPL_rand.o             : ../HPL_rand.c             $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_rand.c
HPL_setran.o           : ../HPL_setran.c           $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_setran.c
#
# ######################################################################
#
clean            :
	$(RM) *.o *.grd
#
# ######################################################################
