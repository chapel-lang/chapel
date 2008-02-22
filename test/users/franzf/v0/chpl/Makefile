NUMRUNS ?= 1000
CHPL = chpl
FLAGS = -O --no-checks --savec output

FFTLIB = fft_2.chpl fft_4.chpl fft_8.chpl fft_16.chpl fft_32.chpl fft_64.chpl fft_128.chpl fft_256.chpl fft_512.chpl fft_1024.chpl fft_2048.chpl 

.PHONY: run
run: fft
	./fft --NUMRUNS=$(NUMRUNS)

fft: fft.chpl $(FFTLIB) main.chpl
	$(CHPL) $(FLAGS) -o fft omega.chpl $(FFTLIB) fft.chpl main.chpl

clean:
	rm -f fft

FORCE:
