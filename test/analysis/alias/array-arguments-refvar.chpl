proc main() {
  var A: [1..10] int;
  var B: [1..10] int;
  ref R = B;
  test(A, R);
}

proc test(a, b) {
  for i in 1..100 {
    a[1] += b[1];
  }
}
