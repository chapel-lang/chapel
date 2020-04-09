config const n = 10;

proc main() {
  var A: [1..10] int = 1;

  callTest(A, A);
  writeln(A);
}

proc callTest(a, b) {
  test(a, b);
}

proc test(a, b) {
  for i in 1..n {
    a[1] += b[1];
  }
}
