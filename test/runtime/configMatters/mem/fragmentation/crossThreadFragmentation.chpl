// Try to check that large allocations don't suffer from fragmentation. We try
// to check that 8 different threads can serially allocate 1/4 of memory. If
// there is fragmentation threads might retain memory and prevent a different
// thread from then using it. This isn't exact because how threads might retain
// memory is implementation specific, but if we have fragmentation this would
// fail under a comm that uses a fixed heap along with qthreads+jemalloc.

use MemUtil;

const totalMem = availMem();
config const memFraction = 4;
config const numTrials = memFraction * 2;
const memPerAlloc = totalMem / memFraction;

var cnt = 0;
for 1..numTrials {
  sync {
    begin with (ref cnt) {
      var A: [0..#memPerAlloc] uint(8) = 1;
      cnt += A[0];
    }
  }
}
assert(cnt == numTrials);
