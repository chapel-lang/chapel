config const n = 10000;
proc main() {
  var A:[1..n] real;
  var B:[1..n] real;
  var C:[1..n] real;
  for i in 1..n do A[i] = 0;
  for i in 1..n do B[i] = 2;
  for i in 1..n do C[i] = i;

  kernel7(A, C);
  writeln(A[1], " ", A[n]);
  kernel8(A);
  writeln(A[1], " ", A[n]);
  kernel9(A);
  writeln(A[1], " ", A[n]);
  kernel10(A);
  writeln(A[1], " ", A[n]);
  kernel11(A);
  writeln(A[1], " ", A[n]);
}

proc kernel7(A, C) {
  var m = 0.0;
  // Fancy reductions that aren't likely
  // to be recognized by the back-end don't
  // get the vectorization hint.
  forall c in C with (max reduce m) {
    m reduce= c;
  }
  A[1] = m;
  A[n] = 0;
}

proc kernel8(A) {
  // regular for loops don't get the vectorization hint
  for i in 1..n {
    A[i] = i;
  }
}

proc kernel9(A) {
  // variables declared within a loop inhibit vectorization
  // if they require stack space - but loop still has parallel access data
  forall i in 1..n {
    var x = i;
    x *= 2;
    A[i] = x;
  }
}

proc kernel10(A) {
  // synchronizing within a forall loop inhibits vectorization
  // (critical sections, waiting for other tasks)
  var sum: atomic int;
  forall i in 1..n {
    sum.add(1);
  }
  A[1] = sum.read();
  A[n] = 0;
}

proc doIncrement(sum: atomic int) {
  sum.add(1);
}

proc kernel11(A) {
  // synchronizing within a forall loop inhibits vectorization
  // (critical sections, waiting for other tasks)
  // detected even if they are called indirectly
  var sum: atomic int;
  forall i in 1..n {
    doIncrement(sum);
  }
  A[1] = sum.read();
  A[n] = 0;
}
