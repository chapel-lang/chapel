const n = max(int);
const m = min(int);

use BlockDist;

var maxSpace4 = [n-1..n, n-2..n, n-3..n, n-4..n];
var minSpace4 = [m..m+1, m..m+2, m..m+3, m..m+4];

const maxD4D: domain(4) = maxSpace4;
const minD4D: domain(4) = minSpace4;
const BlockmaxD4D: domain(4) distributed new dist(new Block(rank=4,bbox=maxSpace4)) = maxSpace4;

def foo(D: domain, A:[D], i) {
  A += n*i;
}

var A: [maxD4D] int;
A = -1;
// This reindex works for the default distribution
foo(minD4D, A, -2);

var BA: [BlockmaxD4D] int;
BA = -1;
// This reindex does not work for the Block distribution
foo(minD4D, BA, -2);
writeln("We should not get here.");
