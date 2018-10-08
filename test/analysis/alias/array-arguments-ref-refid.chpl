proc main() {
  var A: [1..10] int;
  var B: [1..10] int;

  test(refIdentity(A), B);
}

proc refIdentity(ref arg) ref {
  return arg;
}

proc test(ref a, ref b) {
  for i in 1..100 {
    a[1] += b[1];
  }
}
