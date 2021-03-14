config const n = 10;

proc main() {
  var A: [1..10] int;
  var B: [1..10] int;
  A = 0;
  B = 1;

  test(A, B);
  writeln(A);
}

proc test(ref a, ref b) {
  for i in 1..n {
    a[1] += b[1];
  }
}
