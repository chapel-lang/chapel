var A: [1..10] int;
var B: [1..10] int;

callTest(A);

proc callTest(a) {
  test(a, A);
}

proc test(a, b) {
  for i in 1..100 {
    a[1] += b[1];
  }
}
