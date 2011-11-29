CC = gcc
FLAGS ?= -O3 -std=c99 -lm

FFTLIB = fft2.c fft4.c fft8.c fft16.c fft32.c fft64.c fft128.c fft256.c fft512.c fft1024.c fft2048.c 

.PHONY: run
run: fft
	./fft

fft: libfft.c $(FFTLIB) main.c
	$(CC) $(FLAGS) -o fft $(FFTLIB) libfft.c main.c

clean:
	rm -f *.o fft

