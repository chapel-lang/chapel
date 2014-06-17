record R {
  var A: [1..3] int;
}

var A: [1..3] int;

var r = new R(A);
r.A(2) = 2;
writeln(r);
writeln(A);

