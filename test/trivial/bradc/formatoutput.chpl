var x: int = 10;

writeln("x is: ", x);

var D: domain(1) = {1..4};
var A: [D] int;

forall i in D {
  A(i) = 3;
}

writeln("A is: ", A);
writeln("A+1 is: ", A+1);
