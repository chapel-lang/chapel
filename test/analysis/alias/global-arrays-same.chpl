var A: [1..10] int;
var B: [1..10] int;

test(A, A);

proc test(a, b) {
  for i in 1..100 {
    a[1] += b[1];
  }
}
