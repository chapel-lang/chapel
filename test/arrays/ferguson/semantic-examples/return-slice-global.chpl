var A:[1..4] int;

proc f() {
  return A[2..3];
}

proc g(x) {
  x[2] = 1;
}

g(f());
writeln(A);
