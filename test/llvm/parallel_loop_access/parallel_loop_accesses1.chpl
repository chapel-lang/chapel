config const n = 11;
// check for correct access.group and llvm.loop.parallel_accesses hinting

// CHECK: void @loop1
proc loop1 (A, B) {
  for i in vectorizeOnly(0..n) {
    // CHECK: load i32,
    // CHECK-SAME: !llvm.access.group ![[GROUP:[0-9]+]]
    // CHECK: store i32 %
    // CHECK-SAME: !llvm.access.group ![[GROUP]]
    A[i] = B[i]*3;
    // CHECK: br i1
    // CHECK-SAME: !llvm.loop ![[LOOP:[0-9]+]]
  }
}
// CHECK: ![[LOOP]] = distinct !{![[LOOP]], ![[PA:[0-9]+]]
// CHECK: ![[PA]] = !{!"llvm.loop.parallel_accesses", ![[GROUP]]

{
  var A : [0..n] int(32);
  var B : [0..n] int(32);

  loop1(A, B);
  writeln("Sum of A is ", + reduce A);
  // CHECK: Sum of A is 0
}
