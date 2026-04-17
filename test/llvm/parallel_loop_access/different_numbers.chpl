proc start_loop1() { return 1; }
proc end_loop1() { return 2; }

proc start_loop2() { return 3; }
proc end_loop2() { return 4; }

proc start_loop3() { return 5; }
proc end_loop3() { return 6; }

//Check whether we generate different metadata number for loops
proc loop (A, B, n) {
  //CHECK-LABEL: void @loop
  foreach i in 1..n {
    //CHECK-LABEL: start_loop1
    start_loop1();

    //CHECK: %[[LOAD_DEST1:[0-9]+]] = load i32,
    //CHECK-SAME: !llvm.access.group ![[GROUP1:[0-9]+]]

    //CHECK: %[[MUL_DEST1:[0-9]+]] = mul
    //CHECK-SAME: %[[LOAD_DEST1]]
    A[i] = 3*B[i];
    //CHECK: store i32 %[[MUL_DEST1]]
    //CHECK-SAME: !llvm.access.group ![[GROUP1]]

    //CHECK-LABEL: end_loop1
    end_loop1();

    // CHECK: br i1
    // CHECK-SAME: !llvm.loop ![[LOOP1:[0-9]+]]
  }

  foreach i in 1..n {
    //CHECK-LABEL: start_loop2
    start_loop2();

    //CHECK: %[[LOAD_DEST2:[0-9]+]] = load i32,
    //CHECK-SAME: !llvm.access.group ![[GROUP2:[0-9]+]]
    //CHECK-NOT: !llvm.access.group ![[GROUP1]]

    //CHECK: %[[MUL_DEST2:[0-9]+]] = mul
    //CHECK-SAME: %[[LOAD_DEST2]]
    A[i] = 5*B[i];
    //CHECK: store i32 %[[MUL_DEST2]]
    //CHECK-SAME: !llvm.access.group ![[GROUP2]]


    foreach j in 1..n {
      //CHECK-LABEL: start_loop3
      start_loop3();

      //CHECK: %[[LOAD_DEST3:[0-9]+]] = load i32,
      //CHECK-SAME: !llvm.access.group ![[GROUP3:[0-9]+]]
      //CHECK-NOT: !llvm.access.group ![[GROUP2]]
      //CHECK-NOT: !llvm.access.group ![[GROUP1]]

      //CHECK: %[[MUL_DEST3:[0-9]+]] = mul
      //CHECK-SAME: %[[LOAD_DEST3]]
      A[j] = 7*B[j];
      //CHECK: store i32 %[[MUL_DEST3]]
      //CHECK-SAME: !llvm.access.group ![[GROUP3]]

      //CHECK-LABEL: end_loop3
      end_loop3();

      // CHECK: br i1
      // CHECK-SAME: !llvm.loop ![[LOOP3:[0-9]+]]
    }

    //CHECK-LABEL: end_loop2
    end_loop2();

    // CHECK: br i1
    // CHECK-SAME: !llvm.loop ![[LOOP2:[0-9]+]]
  }
}
// CHECK: ![[LOOP1]] = distinct !{![[LOOP1]], ![[PA1:[0-9]+]]
// CHECK: ![[PA1]] = !{!"llvm.loop.parallel_accesses",
// CHECK-SAME: ![[GROUP1]]
// CHECK-NOT: ![[GROUP2]]
// CHECK-NOT: ![[GROUP3]]
// CHECK-SAME: }

// CHECK: ![[LOOP3]] = distinct !{![[LOOP3]], ![[PA3:[0-9]+]]
// CHECK: ![[PA3]] = !{!"llvm.loop.parallel_accesses",
// CHECK-SAME: ![[GROUP3]]
// CHECK-SAME: ![[GROUP2]]
// CHECK-NOT: ![[GROUP1]]
// CHECK-SAME: }

// CHECK: ![[LOOP2]] = distinct !{![[LOOP2]], ![[PA2:[0-9]+]]
// CHECK: ![[PA2]] = !{!"llvm.loop.parallel_accesses",
// CHECK-SAME: ![[GROUP2]]
// CHECK-NOT: ![[GROUP3]]
// CHECK-NOT: ![[GROUP1]]
// CHECK-SAME: }

config const n = 1000;

var A : [1..n] int(32);
var B : [1..n] int(32);

loop(A, B, n);
writeln("Sum of A is ", + reduce A);
