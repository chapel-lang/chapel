proc main() {
  var A: [1..10] int;
  var B: [1..10] int;

  test(refIdentity(A), B);
}

proc refIdentity(const ref arg) const ref {
  return arg;
}

proc test(const ref a, const ref b) {
  var sum = 0;
  for i in 1..100 {
    sum += a[1] + b[1];
  }
  return sum;
}
