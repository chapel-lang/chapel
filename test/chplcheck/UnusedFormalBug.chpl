module UnusedFormalBug {
  var A: [1..10] int;
  proc foo(A) {
    A[1] = 2;
  }
}
