CC=chpl
CFLAGS=-O --no-bounds-checks

TARGETS=uts-par uts-seq

all: uts-par uts-seq

uts-seq: uts.chpl sha1_rng.chpl
	$(CC) $(CFLAGS) -o $@ --ccflags="brg_sha1.c" --serial -sparallel=false $+

uts-par: uts.chpl sha1_rng.chpl
	$(CC) $(CFLAGS) -o $@ --ccflags="brg_sha1.c" -sparallel=true $+

uts-deq: uts_deq.chpl sha1_rng.chpl dequeue.chpl
	$(CC) $(CFLAGS) -o $@ --ccflags="brg_sha1.c" $+

clean:
	rm -f $(TARGETS)
