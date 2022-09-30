var A: [1..3] int;

proc foo(x: int) ref {
  return A[x];
}

proc bar(fn) {
  ref x = fn(2);
  x = 42;
}

bar(foo);
writeln(A);

