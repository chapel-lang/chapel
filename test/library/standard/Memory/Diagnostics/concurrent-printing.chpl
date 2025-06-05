// The purpose of this test is to check that
// memoryUsed, printMemAllocsByType, and printMemAllocs
// do not core dump when called while other tasks
// are doing allocations.

import MemDiagnostics;
import IO.FormattedIO.format;

config const k = 10;
config const m = 100;
config const n = 10000;
config const p = 1009;

var tot = 0;
forall i in 0..<n with (+ reduce tot) {
  for j in 1..k {
    var A: [0..<m] int;
    A = 0..<m;
    A *= 2;
    var s = try! "%?".format(A);
    tot += s.size;
  }
  if i % p == 0 {
    writeln("memory usage: ", MemDiagnostics.memoryUsed());
    MemDiagnostics.printMemAllocsByType();
    MemDiagnostics.printMemAllocs();
  }
}

writeln("OK, tot=", tot);
