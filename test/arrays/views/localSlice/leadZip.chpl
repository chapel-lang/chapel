
proc main() {
  var A, B : [1..20] int;
  A = 1;
  B = 2;

  forall (a, b) in zip(A[local], B) {
    writeln(a + b);
  }
}
