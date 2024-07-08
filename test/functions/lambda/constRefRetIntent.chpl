var A: [1..3] int;

proc foo(x: int) const ref {
  return A[x];
}

proc bar(fn) {
  const ref x = fn(2);
  writeln(x);
  A = 42;
  writeln(x);
}

bar(foo);
writeln(A);

