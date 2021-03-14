proc foo(n) {
  return for i in 1..n do i;
}

config const n = 10;
var A: [1..n] int;
A = foo(n);
writeln(A);
