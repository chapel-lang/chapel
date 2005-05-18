var x: integer = 10;

writeln("x is: ", x);
writeln("x is: ", x:"%3lld");
writeln("x is: ", x:"%.3lld");

var D: domain(1) = 1..4;
var A: [D] integer;

forall i in D {
  A(i) = 3;
}

writeln("A is: ", A);
writeln("A is: ", A:"%2lld");
writeln("A is: ", A:"%.2lld");
