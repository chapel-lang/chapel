config const n = 11;
// check for correct access.group and llvm.loop.parallel_accesses hinting

// CHECK: void @nestedLoops
proc nestedLoops (A, B) {
  for i in 0..n {
    for j in vectorizeOnly(0..n) {
      for k in 0..n {
        for z in vectorizeOnly(0..n) {
          // CHECK: load i32,
          // CHECK-SAME: !llvm.access.group ![[GROUPZ:[0-9]+]]
          // CHECK: mul nsw i32 %{{[^,]+}}, 11111
          // CHECK: store i32 %
          // CHECK-SAME: !llvm.access.group ![[GROUPZ]]
          A[i,j,k,z] = B[i,j,k,z]*11111;
          // CHECK: br i1 %{{[^,]+}}, label %[[LABELZ:[^,]+]],
          // CHECK-SAME: !llvm.loop ![[LOOPZ:[0-9]+]]
        }
        // CHECK: load i32,
        // CHECK-SAME: !llvm.access.group ![[GROUPJ:[0-9]+]]
        // CHECK: mul nsw i32 %{{[^,]+}}, 22222
        // CHECK: store i32 %
        // CHECK-SAME: !llvm.access.group ![[GROUPJ]]
        A[i,j,k,0] = B[i,j,k,0]*22222;
        // CHECK: br i1 %{{[^,]+}}, label %[[LABELK:[^,]+]],
      }
      // CHECK: load i32,
      // CHECK-SAME: !llvm.access.group ![[GROUPJ]]
      // CHECK: mul nsw i32 %{{[^,]+}}, 33333
      // CHECK: store i32 %
      // CHECK-SAME: !llvm.access.group ![[GROUPJ]]
      A[i,j,0,0] = B[i,j,0,0]*33333;
      // CHECK: br i1 %{{[^,]+}}, label %[[LABELJ:[^,]+]],
      // CHECK-SAME: !llvm.loop ![[LOOPJ:[0-9]+]]
    }
    // CHECK: load i32,
    // CHECK-NOT: !llvm.access.group
    // CHECK: mul nsw i32 %{{[^,]+}}, 44444
    // CHECK: store i32 %
    // CHECK-NOT: !llvm.access.group
    A[i,0,0,0] = B[i,0,0,0]*44444;
    // CHECK: br i1 %{{[^,]+}}, label %[[LABELI:[^,]+]],
  }
  // CHECK: ret void
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
