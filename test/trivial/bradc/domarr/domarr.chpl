var m : integer = 4; 
var n : integer = 8;

var Mat : domain(2) = [1..m, 1..n];

var A : [Mat] float;

forall i,j in Mat
  A(i,j) = 0.0;

writeln("A is:\n", A);
