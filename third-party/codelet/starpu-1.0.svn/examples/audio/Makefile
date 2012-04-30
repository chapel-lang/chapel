# StarPU --- Runtime system for heterogeneous multicore architectures.
#
# Copyright (C) 2009, 2010-2011  Université de Bordeaux 1
# Copyright (C) 2010, 2011  Centre National de la Recherche Scientifique
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

CFLAGS += -Wall -g3 -gdwarf-2 -O3 

LIBS+=$$(pkg-config --libs libstarpu) -lcufft
CFLAGS+=$$(pkg-config --cflags libstarpu)

LIBS+=$$(pkg-config --libs fftw3f)
CFLAGS+=$$(pkg-config --cflags fftw3f)

all: starpu_audio_processing

starpu_audio_processing.o: starpu_audio_processing.c

starpu_audio_processing: starpu_audio_processing.o
	$(CC) $(LDFLAGS) starpu_audio_processing.o -o starpu_audio_processing $(LIBS)

clean:
	rm -f *.o
	rm -f starpu_audio_processing
	rm -f input.dat input.raw
	rm -f output.dat output.wav output.raw
