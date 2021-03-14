config const n = 10;

proc main() {
  var A: [1..10] int = 1;
  var B: [1..10] int = 2;

  var sum = test(A, B);
  writeln(sum);
}

proc test(const ref a, const ref b) {
  var sum = 0;
  for i in 1..n {
    sum += a[1] + b[1];
  }
  return sum;
}
