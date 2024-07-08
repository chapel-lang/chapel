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
  // there is a load of a ptr, a load of an i32, and a store of an i32
  // all of them should get the same parallel access
  // CHECK: @loop_chpl
  // CHECK: !llvm.access.group ![[GROUP:[0-9]+]]
  // CHECK: !llvm.access.group ![[GROUP]]
  // CHECK: !llvm.access.group ![[GROUP]]
  foreach i in 0..<n {
    A[A[i]] = B[i];
    A[i] = B[i+1];
  }
  // CHECK: !llvm.loop ![[LOOP_ID:[0-9]+]]
  // CHECK: ![[LOOP_ID]] = distinct !{![[LOOP_ID]], ![[ACCESS:[0-9]+]]
  // CHECK: ![[ACCESS]] = !{!"llvm.loop.parallel_accesses", ![[GROUP]]}
}

var A : [0..<N] int(32);
var B : [0..<N] int(32);

loop(A, B, N);
writeln("Sum of A is ", + reduce A);
