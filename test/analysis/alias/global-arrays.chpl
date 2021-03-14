config const n = 10;

var A: [1..10] int = 1;
var B: [1..10] int;
B = 0;

test(A, B);
writeln(A);

proc test(a, b) {
  for i in 1..n {
    a[1] += b[1];
  }
}
