config const n = 10;

proc main() {
  var A: [1..10, 1..10] int;
  const ref B = A[1, 1..10];

  A[1,1] = 1;
  test(A, B);
  writeln(A);
}

proc test(ref a, b) {
  for i in 1..n {
    a[1,1] += b[1];
  }
}
