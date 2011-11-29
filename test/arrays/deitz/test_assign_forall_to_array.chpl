proc main {
  var A, B: [1..3] real;
  A = 1..3;
  B = [i in 1..3] A(i);
  writeln(B);
}
