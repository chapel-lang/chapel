CC     = chpl
CFLAGS = -O --no-bounds-checks

TARGETS= uts-rec uts-rec-seq uts-deq uts-deq-seq

all: $(TARGETS)

uts-rec: uts_rec.chpl sha1_rng.chpl brg_sha1.c brg_sha1.h
	$(CC) $(CFLAGS) -o $@ -sparallel=true $+

uts-rec-seq: uts_rec.chpl sha1_rng.chpl brg_sha1.c brg_sha1.h
	$(CC) $(CFLAGS) -o $@ --serial -sparallel=false $+

uts-deq: uts_deq.chpl sha1_rng.chpl dequeue.chpl brg_sha1.c brg_sha1.h
	$(CC) $(CFLAGS) -o $@ $+

uts-deq-seq: uts_deq.chpl sha1_rng.chpl dequeue.chpl brg_sha1.c brg_sha1.h
	$(CC) $(CFLAGS) -o $@ -sparallel=false --serial $+

clean:
	rm -f $(TARGETS)
