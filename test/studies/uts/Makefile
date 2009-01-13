CHPL     = chpl
CHPLFLAGS= -O --no-checks

TARGETS  = uts-rec uts-rec-seq uts-deq uts-deq-seq

all: $(TARGETS)

uts-rec: uts_rec.chpl sha1_rng.chpl brg_sha1.c brg_sha1.h
	$(CHPL) $(CHPLFLAGS) -o $@ -sparallel=true $+

uts-rec-seq: uts_rec.chpl sha1_rng.chpl brg_sha1.c brg_sha1.h
	$(CHPL) $(CHPLFLAGS) -o $@ --serial -sparallel=false $+

uts-deq: uts_deq.chpl sha1_rng.chpl dequeue.chpl brg_sha1.c brg_sha1.h
	$(CHPL) $(CHPLFLAGS) -o $@ --main-module=uts_deq $+

uts-deq-seq: uts_deq.chpl sha1_rng.chpl dequeue.chpl brg_sha1.c brg_sha1.h
	$(CHPL) $(CHPLFLAGS) -o $@ --main-module=uts_deq -sparallel=false --serial $+

clean:
	rm -f $(TARGETS)
