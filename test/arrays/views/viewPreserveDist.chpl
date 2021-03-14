use BlockDist;

const D1 = {1..8} dmapped Block({1..8});
var A1: [D1] real;

const D2 = {1..8, 1..8} dmapped Block({1..8, 1..8});
var A2: [D2] real;

foo(A1);
foo(A1[2..8]);
foo(A2[.., 3]);
foo(A2[7, ..]);
foo(A1.reindex(0..15 by 2));

proc foo(X: [?D] real) {
  writeln("D is: ", D);
  forall i in D do
    X[i] = here.id / 10.0;
  writeln("X is: ", X);
  var Y: [D] real;
  forall y in Y do
    y = here.id / 10.0;
  writeln("Y is: ", Y);
  writeln();
}
