config const n = 1_000;

proc multiSqrt(in n: real) {
  while n > 10.0 {
    n = sqrt(n);
  }
  return n;
}

proc main() {
  var nums: [0..<n] real = [i in 0..<n] i:real;
  for n in nums {
    n = multiSqrt(n);
  }
}
