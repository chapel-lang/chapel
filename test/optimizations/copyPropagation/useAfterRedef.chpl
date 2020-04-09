
config const n = 5;

proc main() {
  var A = 0, B = 0;
  for i in 1..n {
    B = A;
    A += 1;
    writeln("A = ", A);
    writeln("B = ", B);
  }
}
