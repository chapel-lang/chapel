CHPL     = chpl
CHPLFLAGS= -O --no-checks

TARGETS  = uts-rec uts-deq

all: $(TARGETS)

uts-rec: uts_rec.chpl sha1_rng.chpl brg_sha1.c brg_sha1.h
	$(CHPL) $(CHPLFLAGS) -o $@ -sparallel=true $+

uts-deq: uts_deq.chpl sha1_rng.chpl dequeue.chpl brg_sha1.c brg_sha1.h
	$(CHPL) $(CHPLFLAGS) -o $@ --main-module=uts_deq $+

clean:
	rm -f $(TARGETS)
