config const n = 11;
// check for correct access.group and llvm.loop.parallel_accesses hinting

// CHECK: void @nestedLoops
proc nestedLoops (A, B) {
  for i in 0..n {
    // CHECK: [[LABELI:nestedLoops.*]]:
    for j in vectorizeOnly(0..n) {
      // CHECK: [[LABELJ:nestedLoops.*]]:
      for k in 0..n {
        // CHECK: [[LABELK:nestedLoops.*]]:
        for z in vectorizeOnly(0..n) {
          // CHECK: [[LABELZ:nestedLoops.*]]:

          // CHECK: load i32,
          // CHECK-SAME: !llvm.access.group ![[GROUPZ:[0-9]+]]
          // CHECK: store i32 %
          // CHECK-SAME: !llvm.access.group ![[GROUPZ]]

          // CHECK: br i1
          // CHECK-SAME: label %[[LABELZ]]
          // CHECK-SAME: !llvm.loop ![[LOOPZ:[0-9]+]]
          A[i,j,k,z] = B[i,j,k,z]*3;
        }

        // CHECK: load i32,
        // CHECK-SAME: !llvm.access.group ![[GROUPJ:[0-9]+]]
        // CHECK: store i32 %
        // CHECK-SAME: !llvm.access.group ![[GROUPJ]]
        A[i,j,k,0] = B[i,j,k,0]*66;

        // CHECK: br i1
        // CHECK-SAME: label %[[LABELK]]
      }

      // CHECK: load i32,
      // CHECK-SAME: !llvm.access.group ![[GROUPJ]]
      // CHECK: store i32 %
      // CHECK-SAME: !llvm.access.group ![[GROUPJ]]
      A[i,j,0,0] = B[i,j,0,0]*101;

      // CHECK: br i1
      // CHECK-SAME: label %[[LABELJ]]
      // CHECK-SAME: !llvm.loop ![[LOOPJ:[0-9]+]]
    }

    // CHECK: load i32,
    // CHECK-NOT: !llvm.access.group
    // CHECK: store i32 %
    // CHECK-NOT: !llvm.access.group
    A[i,0,0,0] = B[i,0,0,0]*730;

    // CHECK: br i1
    // CHECK-SAME: label %[[LABELI]]
  }
}

// CHECK: !llvm.ident

// CHECK: ![[LOOPZ]] = distinct !{![[LOOPZ]], ![[PAZ:[0-9]+]]
// CHECK: ![[PAZ]] = !{!"llvm.loop.parallel_accesses",
// CHECK-SAME: ![[GROUPZ]]
// CHECK-SAME: ![[GROUPJ]]

// CHECK: ![[LOOPJ]] = distinct !{![[LOOPJ]], ![[PAJ:[0-9]+]]
// CHECK: ![[PAJ]] = !{!"llvm.loop.parallel_accesses",
// CHECK-SAME: ![[GROUPJ]]

{
  var A : [0..n, 0..n, 0..n, 0..n] int(32);
  var B : [0..n, 0..n, 0..n, 0..n] int(32);

  nestedLoops(A, B);
  writeln("Sum of A is ", + reduce A);
  // CHECK: Sum of A is 0
}
