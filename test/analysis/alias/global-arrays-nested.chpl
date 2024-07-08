config const n = 10;

var A: [1..10] int;
var B: [1..10] int;

A = 1;
callTest(A);
writeln(A);

proc callTest(ref a) {
  test(a, A);
}

proc test(ref a, b) {
  for i in 1..n {
    a[1] += b[1];
  }
}
