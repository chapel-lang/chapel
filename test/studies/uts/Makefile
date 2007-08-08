CC=chpl
CFLAGS=-O --no-bounds-checks
#CFLAGS=-g

TARGETS=uts-rec uts-rec-seq uts-deq uts-deq-seq

all: $(TARGETS)

uts-rec: uts_rec.chpl sha1_rng.chpl
	$(CC) $(CFLAGS) -o $@ --ldflags="brg_sha1.c" -sparallel=true $+

uts-rec-seq: uts_rec.chpl sha1_rng.chpl
	$(CC) $(CFLAGS) -o $@ --ldflags="brg_sha1.c" --serial -sparallel=false $+

uts-deq: uts_deq.chpl sha1_rng.chpl dequeue.chpl
	$(CC) $(CFLAGS) -o $@ --ldflags="brg_sha1.c" $+

uts-deq-seq: uts_deq.chpl sha1_rng.chpl dequeue.chpl
	$(CC) $(CFLAGS) -o $@ --ldflags="brg_sha1.c" -sparallel=false --serial $+

clean:
	rm -f $(TARGETS)
