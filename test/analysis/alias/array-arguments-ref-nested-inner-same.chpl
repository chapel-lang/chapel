config const n = 10;

proc main() {
  var A: [1..10] int;
  var B: [1..10] int;
  A = 1;
  B = 0;

  callTest(A, B);
  writeln(A);
}

proc callTest(ref a, ref b) {
  test(a, a);
}

proc test(ref a, ref b) {
  for i in 1..n {
    a[1] += b[1];
  }
}
