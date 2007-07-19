CC=chpl
#CFLAGS=-O --no-bounds-checks
CFLAGS=-g

TARGETS=uts-par uts-seq uts-deq uts-deq-seq

all: uts-par uts-seq uts-deq uts-deq-seq

uts-seq: uts.chpl sha1_rng.chpl
	$(CC) $(CFLAGS) -o $@ --ccflags="brg_sha1.c" --serial -sparallel=false $+

uts-par: uts.chpl sha1_rng.chpl
	$(CC) $(CFLAGS) -o $@ --ccflags="brg_sha1.c" -sparallel=true $+

uts-deq: uts_deq.chpl sha1_rng.chpl dequeue.chpl
	$(CC) $(CFLAGS) -o $@ --ccflags="brg_sha1.c" $+

uts-deq-seq: uts_deq.chpl sha1_rng.chpl dequeue.chpl
	$(CC) $(CFLAGS) -o $@ --ccflags="brg_sha1.c" -sparallel=false --serial $+

clean:
	rm -f $(TARGETS)
