# StarPU --- Runtime system for heterogeneous multicore architectures.
# 
# Copyright (C) 2009  Université de Bordeaux 1
# Copyright (C) 2010  Centre National de la Recherche Scientifique
# 
# StarPU is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2.1 of the License, or (at
# your option) any later version.
# 
# StarPU is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# 
# See the GNU Lesser General Public License in COPYING.LGPL for more details.

CUDASDKDIR=/home/gonnet/NVIDIA_CUDA_SDK/
CUDAINSTALLDIR=/usr/local/cuda/

CFLAGS += -I$(CUDAINSTALLDIR)/include -Wall
LDFLAGS += -lcuda -L/usr/local/cuda/lib
LDFLAGS += -lcublas -lpthread

CFLAGS+= -I$(ATLASDIR)/include/
LDFLAGS+= $(ATLASDIR)/lib/$(BLASARCH)/libcblas.a
LDFLAGS+= $(ATLASDIR)/lib/$(BLASARCH)/libatlas.a

CFLAGS+= -g #-pg
#LDFLAGS+= -pg

all:  cuda_bandwidth

cuda_bandwidth:  cuda_bandwidth.o
	$(CC)  cuda_bandwidth.o -o  cuda_bandwidth $(LDFLAGS)

clean:
	rm -f cuda_bandwidth
	rm -f *.o
