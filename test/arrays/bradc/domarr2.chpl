var m : integer = 4; 
var n : integer = 8;

var Mat : domain(2) = (1..m, 1..n);

var A : [Mat] float;

forall i,j in Mat do
  A(i,j) = 0.0;

writeln("A is:\n", A);

[i,j in Mat] A(i,j) = 1.0;

writeln("A is:\n", A);

/* This will only work if we have an elided indices concept
[Mat] A = 1.5;

writeln("A is:\n", A);
*/

A = [Mat] 2.0;

writeln("A is:\n", A);

A = 3.0;

writeln("A is:\n", A);
