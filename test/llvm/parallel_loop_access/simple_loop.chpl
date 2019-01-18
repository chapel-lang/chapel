//Check whether we add parallel_loop_access metadata for loops at all
proc loop (A, B, n) {
  for i in vectorizeOnly(1..n) {
    // CHECK: llvm.mem.parallel_loop_access
    A[i] = 3*B[i];
  }
}

config const n = 1000;

var A : [1..n] int(32);
var B : [1..n] int(32);

loop(A, B, n);
writeln("Sum of A is ", + reduce A);
