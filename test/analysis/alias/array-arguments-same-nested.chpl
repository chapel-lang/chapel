proc main() {
  var A: [1..10] int;

  callTest(A, A);
}

proc callTest(a, b) {
  test(a, b);
}

proc test(a, b) {
  for i in 1..100 {
    a[1] += b[1];
  }
}
