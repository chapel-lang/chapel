const n = max(int);
const m = min(int);
config const bbLow = -5;
config const bbSize = 1;

use BlockDist;

var bbox4 = [bbLow..bbLow+bbSize,
             bbLow..bbLow+2*bbSize,
             bbLow..bbLow+3*bbSize,
             bbLow..bbLow+4*bbSize];
var maxSpace4 = [n-1..n, n-2..n, n-3..n, n-4..n];
var minSpace4 = [m..m+1, m..m+2, m..m+3, m..m+4];

const maxD4D: domain(4) = maxSpace4;
const minD4D: domain(4) = minSpace4;
const BlockmaxD4D: domain(4) distributed new dist(new Block(rank=4,bbox=bbox4)) = minSpace4;

def foo(D: domain, A:[D], i) {
  A += n*i;
}

var A: [minD4D] int;
A = -1;
// This reindex works for the default distribution
foo(maxD4D, A, -2);

var BA: [BlockmaxD4D] int;
BA = -1;
// This reindex does not work for the Block distribution except as follows
foo(maxD4D, BA, -2);
writeln("We should not get here unless bbLow==max(int) [",
        bbLow==max(int), "] || bbLow==min(int) [",
        bbLow==min(int), "]");
