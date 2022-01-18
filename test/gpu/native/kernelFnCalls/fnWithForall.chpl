config const n = 10;

proc foo(n) {
  var A: [0..n] real;

  forall i in 0..n {
    A[i] = i * 1.5;
  }

  return A[n];
}


on here.getChild(1) {
  var A: [0..#n] real;

  forall i in 0..#n {
    A[i] = foo(i);
  }

  writeln(A);

  var x = foo(n);
  writeln(x);
}

var x2 = foo(n);
writeln(x2);
