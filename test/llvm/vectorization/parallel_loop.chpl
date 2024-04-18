// Check whether parallel_loop_access metadata impacts vectorization
// This is special case of loop taken from LLVM test suite
// with extra branch added inside, to avoid vectorizaton if
// metadata isn't added for operations inside branch
// LLVM is unable to see whether
//
// A[A[i]] = B[i];
// A[i] = B[i+1];
//
// has no loop carried dependency, and adding
// parallel_loop_access metadata is necessary

config const N = 512;
proc loop (ref A, B, n) {
  // CHECK: <4 x i32>
  foreach i in 0..<n {
    A[A[i]] = B[i];
    A[i] = B[i+1];
  }
}

var A : [0..<N] int(32);
var B : [0..<N] int(32);

loop(A, B, N);
writeln("Sum of A is ", + reduce A);
