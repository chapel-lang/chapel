config const n = 10;

proc main() {
  var A: [1..10] int;
  var B: [1..10] int;
  A = 0;
  B = 1;

  callTest(A, B);
  writeln(A);
}

proc callTest(ref a, ref b) {
  test(a, b);
}

proc test(ref a, ref b) {
  for i in 1..n {
    a[1] += b[1];
  }
}
