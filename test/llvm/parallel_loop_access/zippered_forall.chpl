config const n = 10000;
proc main() {
  var A:[1..n] real = 0;
  loop(A);
  writeln(A[1], " ", A[n]);
}

proc loop(A) {
  forall (i,j) in zip(1..n, 2..) {
    A[i] = 17.5 * j;
  }
  // CHECK: @loop
  // CHECK: fmul
  // CHECK: llvm.mem.parallel_loop_access

  // CHECK: @coforall
  // CHECK: fmul
  // CHECK: llvm.mem.parallel_loop_access
}
