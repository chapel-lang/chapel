use Math;

inline proc myfma(x: real(64), y: real(64), z: real(64)) {
  return x*y+z;
}

proc main() {
  var n1 = fma(2, 2, 2);
  var n2 = myfma(2, 2, 2);
  assert(n1 == n2);
  writeln(n1);
}
