// Allocate 1/2 of memory multiple times each time doubling the number of
// allocations but halving the size of each allocation. This is testing that
// larger memroy regions can be reused for later smaller ones (i.e. support
// splitting regions)

use MemUtil;

const totalMem = availMem();
config const memFraction = 2;
config const numTrials = memFraction * 2;
var memPerAlloc = totalMem / memFraction;

var cnt = 0;
var numArrays = 1;
for 1..numTrials {
  var A: [0..#numArrays] [0..#memPerAlloc] uint(8);
  A[0] = 1;
  cnt += A[0][0];
  memPerAlloc /= 2;
  numArrays *= 2;
}
assert(cnt == numTrials);
