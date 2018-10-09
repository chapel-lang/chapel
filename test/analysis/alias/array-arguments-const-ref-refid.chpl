config const n = 10;

proc main() {
  var A: [1..10] int;
  var B: [1..10] int;
  A = 1;
  B = 2;

  var sum = test(refIdentity(A), B);
  writeln(sum);
}

proc refIdentity(const ref arg) const ref {
  return arg;
}

proc test(const ref a, const ref b) {
  var sum = 0;
  for i in 1..n {
    sum += a[1] + b[1];
  }
  return sum;
}
