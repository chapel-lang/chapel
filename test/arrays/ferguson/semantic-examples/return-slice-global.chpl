use samples;
// const one = new R(1);

var A:[1..4] R;

proc f() {
  return A[2..3];
}

proc g(x) {
  x[2] = one;
}

g(f());
writeln(A);
