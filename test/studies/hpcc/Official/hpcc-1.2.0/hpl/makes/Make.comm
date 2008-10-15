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
   $(INCdir)/hpl_misc.h   $(INCdir)/hpl_pmisc.h   $(INCdir)/hpl_grid.h \
   $(INCdir)/hpl_panel.h  $(INCdir)/hpl_pgesv.h
#
## Object files ########################################################
#
HPL_comobj       = \
   HPL_1ring.o            HPL_1rinM.o            HPL_2ring.o            \
   HPL_2rinM.o            HPL_blong.o            HPL_blonM.o            \
   HPL_packL.o            HPL_copyL.o            HPL_binit.o            \
   HPL_bcast.o            HPL_bwait.o            HPL_send.o             \
   HPL_recv.o             HPL_sdrv.o
#
## Targets #############################################################
#
all     : lib 
#
lib     : lib.grd
#
lib.grd : $(HPL_comobj)
	$(ARCHIVER) $(ARFLAGS) $(HPLlib) $(HPL_comobj)
	$(RANLIB) $(HPLlib)
	$(TOUCH) lib.grd
#
# ######################################################################
#
HPL_1ring.o            : ../HPL_1ring.c            $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_1ring.c
HPL_1rinM.o            : ../HPL_1rinM.c            $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_1rinM.c
HPL_2ring.o            : ../HPL_2ring.c            $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_2ring.c
HPL_2rinM.o            : ../HPL_2rinM.c            $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_2rinM.c
HPL_blong.o            : ../HPL_blong.c            $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_blong.c
HPL_blonM.o            : ../HPL_blonM.c            $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_blonM.c
HPL_packL.o            : ../HPL_packL.c            $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_packL.c
HPL_copyL.o            : ../HPL_copyL.c            $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_copyL.c
HPL_binit.o            : ../HPL_binit.c            $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_binit.c
HPL_bcast.o            : ../HPL_bcast.c            $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_bcast.c
HPL_bwait.o            : ../HPL_bwait.c            $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_bwait.c
HPL_send.o             : ../HPL_send.c             $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_send.c
HPL_recv.o             : ../HPL_recv.c             $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_recv.c
HPL_sdrv.o             : ../HPL_sdrv.c             $(INCdep)
	$(CC) -o $@ -c $(CCFLAGS)  ../HPL_sdrv.c
#
# ######################################################################
# 
clean            :
	$(RM) *.o *.grd
#
# ######################################################################
