CC=chpl
CFLAGS=-O --no-bounds-checks

TARGETS=uts btdl bttl

all: uts btdl bttl

uts: uts.chpl sha1_rng.chpl
	$(CC) $(CFLAGS) -o $@ --ccflags="brg_sha1.c" $+

btdl: BinaryTree_DepthLimit.chpl
	$(CC) $(CFLAGS) -o $@ $+

bttl: BinaryTree_ThreadLimit.chpl
	$(CC) $(CFLAGS) -o $@ $+

clean:
	rm -f *.o *.gch $(TARGETS)
