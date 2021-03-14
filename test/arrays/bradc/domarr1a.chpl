var m : int = 4; 
var n : int = 8;

var Mat : domain(2) = {1..m, 1..n};

var A : [Mat] real;

forall ij in Mat do
  A(ij) = 0.0;

writeln("A is:\n", A);
