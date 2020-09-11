proc start_block() {}
proc if_branch() {}
proc while_loop() {}
proc while_body() {}
proc end_block() {}

//Check whether we generate parallel_loop_access metadata
//inside basic blocks in the loop like
// - non-parallel-loops inside loop
// - if branches
// - basic block at start of loop
// - basic block at the end of the loop

proc loop (A, B, n) {
  //CHECK-LABEL: void @loop
  for i in vectorizeOnly(1..n) {
    // CHECK-LABEL: start_block
    start_block();
    // Make sure metadata is generated at start of loop
    // CHECK: !llvm.access.group ![[GROUP1:[0-9]+]]
    A[i] = 3*B[i];
    if(A[i] < B[i]) {
      // CHECK-LABEL: if_branch
      if_branch();
      // Make sure metadata is generated inside branches in loop
      // CHECK: !llvm.access.group ![[GROUP1]]
      A[i] = 3*B[i];
    }

    // CHECK: while_loop
    while_loop();
    while(A[i] < 100) // not a CForLoop so no metadata/loop annotation
    {
      // CHECK: while_body
      while_body();
      // Make sure metadata is generated in loops inside
      // CHECK: !llvm.access.group ![[GROUP1]]
      A[i] = A[i]+3;
    }
    // Make sure metadata is generated at the end of loop
    // CHECK-LABEL: end_block
    end_block();
    // CHECK: !llvm.access.group ![[GROUP1]]
    A[i] = 3*B[i];

    // CHECK: br i1
    // CHECK-SAME: !llvm.loop ![[LOOP1:[0-9]+]]
  }
}
// CHECK: ![[LOOP1]] = distinct !{![[LOOP1]], ![[PA1:[0-9]+]]
// CHECK: ![[PA1]] = !{!"llvm.loop.parallel_accesses",
// CHECK-SAME: ![[GROUP1]]
// CHECK-SAME: }

config const n = 1000;

var A : [1..n] int(32);
var B : [1..n] int(32);

loop(A, B, n);
writeln("Sum of A is ", + reduce A);
