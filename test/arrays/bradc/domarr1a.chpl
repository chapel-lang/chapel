var m : integer = 4; 
var n : integer = 8;

var Mat : domain(2) = (1..m, 1..n);

var A : [Mat] float;

forall ij in Mat do
  A(ij) = 0.0;

writeln("A is:\n", A);
