CHPL     = chpl
CHPLFLAGS= -O --no-bounds-checks --ccflags -DBRG_C99_TYPES

TESTTREE = -sdistrib=Geometric -sgeoDist=GeoFixed -sSEED=19 -sB_0=4 -sMAX_DEPTH=5

TARGETS  = uts-rec uts-rec-seq uts-deq uts-deq-seq

all: $(TARGETS)

uts-rec: uts_rec.chpl sha1_rng.chpl brg_sha1.c brg_sha1.h
	$(CHPL) $(CHPLFLAGS) -o $@ -sparallel=true $+

uts-rec-seq: uts_rec.chpl sha1_rng.chpl brg_sha1.c brg_sha1.h
	$(CHPL) $(CHPLFLAGS) -o $@ --serial -sparallel=false $+

uts-deq: uts_deq.chpl sha1_rng.chpl dequeue.chpl brg_sha1.c brg_sha1.h
	$(CHPL) $(CHPLFLAGS) -o $@ $+

uts-deq-seq: uts_deq.chpl sha1_rng.chpl dequeue.chpl brg_sha1.c brg_sha1.h
	$(CHPL) $(CHPLFLAGS) -o $@ -sparallel=false --serial $+

test:
	@$(MAKE) CHPL=$(CHPL) all > /dev/null
	./uts-rec -nl 1 -stestMode=true $(TESTTREE)
	./uts-rec-seq -nl 1 -stestMode=true $(TESTTREE)
	./uts-deq -nl 1 -stestMode=true $(TESTTREE)
	./uts-deq-seq -nl 1 -stestMode=true $(TESTTREE)

clean:
	rm -f $(TARGETS)
