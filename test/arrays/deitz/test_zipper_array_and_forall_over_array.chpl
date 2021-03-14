proc main {
  var A, B: [1..3] real;
  A = 1..3;
  forall (b,a) in zip(B, [i in 1..3] A(i)) do
    b = a;
  writeln(B);
}
