proc start_loop1() {}
proc end_loop1() {}

proc start_loop2() {}
proc end_loop2() {}

proc start_loop3() {}
proc end_loop3() {}

//Check whether we generate different metadata number for loops
proc loop (A, B, n) {
  //CHECK-LABEL: void @loop
  for i in vectorizeOnly(1..n) {
    //CHECK-LABEL: start_loop1
    start_loop1();

    //CHECK: llvm.mem.parallel_loop_access ![[LOOP1:[0-9]+]]
    A[i] = 3*B[i];

    //CHECK-LABEL: end_loop1
    end_loop1();
  }

  for i in vectorizeOnly(1..n) {
    //CHECK-LABEL: loop2
    start_loop2();

    //CHECK: llvm.mem.parallel_loop_access ![[LOOP2:[0-9]+]]
    //CHECK-NOT: llvm.mem.parallel_loop_access ![[LOOP1]]
    A[i] = 5*B[i];
    for j in vectorizeOnly(1..n) {
      //CHECK-LABEL: start_loop3
      start_loop3();

      //CHECK: llvm.mem.parallel_loop_access ![[LOOP3:[0-9]+]]
      //CHECK-NOT: llvm.mem.parallel_loop_access ![[LOOP2]]
      A[j] = 7*B[j];

      //CHECK-LABEL: end_loop3
      end_loop3();
    }

    //CHECK-LABEL: end_loop2
    end_loop2();
  }
}

config const n = 1000;

var A : [1..n] int(32);
var B : [1..n] int(32);

loop(A, B, n);
writeln("Sum of A is ", + reduce A);
