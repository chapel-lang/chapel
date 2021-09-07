// Allocate ~1/4 of memory 8 times using slightly larger allocations each time.
// This is testing that there isn't fragmentation where smaller memory regions
// can't be reused for large ones (i.e. can we merge memory regions)

use MemUtil;

const totalMem = availMem();
config const memFraction = 4;
config const numTrials = memFraction * 2;
config const multiplier = 1.1;
var memPerAlloc = totalMem / memFraction;

var cnt = 0;
for 1..numTrials {
  var A: [0..#memPerAlloc] uint(8) = 1;
  cnt += A[0];
  memPerAlloc = increaseMem(memPerAlloc);
}
assert(cnt == numTrials);

// Increase mem size by multiplier up to 85% of available memory
proc increaseMem(curMem: int) {
  const maxMem = (totalMem:real * 0.85):int;
  const desiredMem = (curMem:real * multiplier):int;
  return min(maxMem, desiredMem);
}
