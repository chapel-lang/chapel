use SysCTypes;
use CPtr;
use CommDiagnostics;
use Random;

config const verbose = false;
param testSize = 4096*1024;
param lineSize = 64; // should match cache line size in bytes

var buf = c_aligned_alloc(uint(8), testSize, testSize);
// store buf as a _ddata so we can GET from it (otherwise always narrow)
const A = buf:_ddata(uint(8));

var rng = new RandomStream(int);

for i in 0..#testSize {
  A[i] = i:uint(8);
}

{ // sanity checking the allocation
  for i in 0..#testSize {
    assert(A[i] == i:uint(8));
  }
}

proc doRead(r: range(?)) {
  resetCommDiagnosticsHere();
  startCommDiagnosticsHere();

  for i in r {
    var tmp = A[i];
    assert(tmp == i:uint(8));
  }

  stopCommDiagnosticsHere();
  var counts = getCommDiagnosticsHere();

  return counts;
}

proc test_readahead(size:int, const reverse:bool, const misses:int) {
  assert(size <= testSize);

  if verbose {
    if size <= 1024 then
      writeln("test_readahead(", size, ", ", reverse, ")");
    else
      writeln("test_readahead(", size/1024, "KiB, ", reverse, ")");
  }

  on Locales[1] {
    var r = 0..#size by -1;
    if reverse == false then
      r = 0..#size;

    var counts = doRead(r);
    if verbose then
      writeln(counts);

    assert(counts.cache_get_misses <= misses);
  }
}

var inPageSizes = [256, 512, 1024];

writeln("Checking forward readahead within page");
for size in inPageSizes {
  test_readahead(size, reverse=false, misses=5);
}

writeln("Checking reverse readahead within page");
for size in inPageSizes {
  test_readahead(size, reverse=true, misses=5);
}


var acrossPageSizes = [4*1024, 8*1024,
                       16*1024, 32*1024, 64*1024,
                       128*1024, 256*1024, 512*1024,
                       1024*1024, 2048*1024, 4096*1024];

writeln("Checking forward readahead across pages");
for size in acrossPageSizes {
  test_readahead(size, reverse=false, misses=5+size/1024);
}
writeln("Checking reverse readahead across pages");
for size in acrossPageSizes {
  test_readahead(size, reverse=true, misses=5+size/1024);
}

// TODO: add separate cache performance counters
//   - include late prefetches / prefetches requiring a wait on a GET
//   - check for late prefetches here

writeln("done");

c_free(buf);
