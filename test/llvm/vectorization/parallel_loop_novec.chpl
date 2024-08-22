//This loop shouldn't be vectorized because no parallel_loop_access
//metadata was added. For more details look into parallel_loop.chpl
config const N = 512;
proc loop (ref A, B, n) {
  // CHECK-NOT: !llvm.access.group
  foreach i in 0..<n {
    A[A[i]] = B[i];
    A[i] = B[i+1];
  }
  // CHECK-NOT: llvm.loop.parallel_accesses
}

var A : [0..<N] int(32);
var B : [0..<N] int(32);

loop(A, B, N);
writeln("Sum of A is ", + reduce A);
