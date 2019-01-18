var D = {1..5};
var A: [D] int;

proc foo(A: []) {
  ref AA = A;
  AA[3] = 99;
  writeln(A);
  writeln(AA);
}

foo(A);
