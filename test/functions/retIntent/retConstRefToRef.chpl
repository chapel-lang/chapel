var A: [1..3] int;

proc foo(x: int) const ref {
  return A[x];
}

ref x = foo(2);
x = 42;

