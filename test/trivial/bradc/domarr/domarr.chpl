var m : integer = 4; 
var n : integer = 8;

var Mat : domain(2) = [1..m, 1..n];
--var Col : domain(2) = [*, 1..n];
--var Row : domain(2) = [1..m, *];

var A : [Mat] float;
--var y : [Col] float;
--var x : [Row] float;

forall i,j in Mat
  A(i,j) = 0.0;

writeln("A is:\n", A);

/*
[i,j:Mat] A(i,j) = 1.0;

writeln("A is:\n", A);

[Mat] A = 1.5;

writeln("A is:\n", A);

A = [Mat] 2.0;

writeln("A is:\n", A);

A = 3.0;

writeln("A is:\n", A);
*/

-- y = [i:Col] i;
