config const n = 10;

on here.getChild(1) {
  var A: [0..n+1] real;

  A[0] = 1; A[n+1] = 1;
  forall i in 1..n { A[i] = i:real; }

  writeln(A);
}
