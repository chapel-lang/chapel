//Check whether we add parallel_loop_access metadata for loops at all
proc loop (A, B, n) {
  for i in vectorizeOnly(1..n) {
    // CHECK: !llvm.access.group ![[GROUP1:[0-9]+]]
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
