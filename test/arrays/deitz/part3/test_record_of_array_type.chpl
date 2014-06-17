record R {
  type t;
  var x: t;
}

var A: [1..3] real = (1.0, 2.0, 3.0);

writeln(A);

var rA = new R(A.type);

writeln(rA);

var rrA = new R(rA.type);

writeln(rrA);
