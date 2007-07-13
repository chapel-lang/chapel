CC=chpl
CFLAGS=-O --no-bounds-checks

TARGETS=uts

all: uts

uts: uts.chpl sha1_rng.chpl
	$(CC) $(CFLAGS) -o $@ --ccflags="brg_sha1.c" $+

clean:
	rm -f *.o *.gch $(TARGETS)
