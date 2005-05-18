var x: integer = 10;

writeln("x is: ", x);
writeln("x is: ", x:"%3d");
writeln("x is: ", x:"%.3d");

var D: domain(1) = 1..4;
var A: [D] integer;

forall i in D {
  A(i) = 3;
}

writeln("A is: ", A);
writeln("A is: ", A:"%2d");
writeln("A is: ", A:"%.2d");
