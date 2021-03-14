record R {
  var D = {1..3};
  var A: [D] real;
}

proc main {
  var r1: R;
  var r2   = r1;
  var r3:R = r1;

  r2.A(1) = 2.0;
  r3.A(1) = 1.0;
  writeln(r1.A(1));
}
