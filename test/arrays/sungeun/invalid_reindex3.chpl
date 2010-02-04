config param n = 10;

use BlockDist;

var Space4 = [1..n, 2..n, 3..n, 4..n];

const D4D: domain(4) = Space4;
const BlockD4D = D4D distributed new dist(new Block(rank=4,bbox=Space4));

def foo(D: domain, A:[D], i) {
  A += n*i;
}

var A: [D4D] int;
A = -1;
foo(D4D, A, -1);
A += -2;
foo(BlockD4D, A, -2);

writeln("FAIL: We should not get here!");
