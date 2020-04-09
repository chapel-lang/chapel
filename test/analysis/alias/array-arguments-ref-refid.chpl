config const n = 10;

proc main() {
  var A: [1..10] int;
  var B: [1..10] int = 1;

  test(refIdentity(A), B);
  writeln(A);
}

proc refIdentity(ref arg) ref {
  return arg;
}

proc test(ref a, ref b) {
  for i in 1..n {
    a[1] += b[1];
  }
}
