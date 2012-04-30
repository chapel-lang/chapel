# StarPU --- Runtime system for heterogeneous multicore architectures.
#
# Copyright (C) 2009-2011  Université de Bordeaux 1
# Copyright (C) 2010, 2011, 2012  Centre National de la Recherche Scientifique
#
# Redistribution  and  use  in  source and binary forms, with or without
# modification,  are  permitted  provided  that the following conditions
# are met:
#
# * Redistributions  of  source  code  must  retain  the above copyright
#   notice,  this  list  of  conditions  and  the  following  disclaimer.
# * Redistributions  in  binary  form must reproduce the above copyright
#   notice,  this list of conditions and the following disclaimer in the
#   documentation  and/or other materials provided with the distribution.
# * The name of the author may not be used to endorse or promote products
#   derived from this software without specific prior written permission.
#
# THIS  SOFTWARE  IS  PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# ``AS IS''  AND  ANY  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED  TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A  PARTICULAR  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL
# SPECIAL,  EXEMPLARY,  OR  CONSEQUENTIAL  DAMAGES  (INCLUDING,  BUT NOT
# LIMITED  TO,  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE
# DATA,  OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY  OF  LIABILITY,  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF  THIS  SOFTWARE,  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


CFLAGS          +=      $$(pkg-config --cflags libstarpu-1.0)
LDFLAGS         +=      $$(pkg-config --libs libstarpu-1.0)

HAS_CUDA	=	$(shell pkg-config --libs libstarpu-1.0 |grep -i cuda)
NVCC		?=	nvcc
HAS_OPENCL	=	$(shell pkg-config --libs libstarpu-1.0 |grep -i opencl)

%.o: %.cu
	nvcc $(CFLAGS) $< -c

all: hello_world vector_scal

VECTOR_SCAL_PREREQUISITES	=	vector_scal.o vector_scal_cpu.o 
ifneq ($(strip $(HAS_CUDA)),)
VECTOR_SCAL_PREREQUISITES	+=	vector_scal_cuda.o
VECTOR_SCAL_COMPILER		=	$(NVCC)
else
VECTOR_SCAL_COMPILER		=	$(CC)
endif
ifneq ($(strip $(HAS_OPENCL)),)
VECTOR_SCAL_PREREQUISITES += vector_scal_opencl.o
endif

vector_scal: $(VECTOR_SCAL_PREREQUISITES)
	$(VECTOR_SCAL_COMPILER) $(LDFLAGS) $^ -o $@

clean:
	rm -f hello_world vector_scal *.o

