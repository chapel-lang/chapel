// This loop shouldn't be vectorized, because
// LLVM backend cannot check whether A and B overlap
// runtime check for overlap was turned off and --no-vectorize was set
// to avoid adding parallel_loop_access metadata

// CHECK: @loop_chpl
proc loop (A, B, n) {
  for i in vectorizeOnly(1..n) {
    // CHECK-NOT: <4 x i32>
    A[i] = 3*B[i];
  }
}
// CHECK: ret void

config const n = 1000;

var A : [1..n] int(32);
var B : [1..n] int(32);

loop(A, B, n);
writeln("Sum of A is ", + reduce A);
