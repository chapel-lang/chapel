var m : int = 4; 
var n : int = 8;

var Mat : domain(2) = {1..m, 1..n};

var A : [Mat] real;

forall (i,j) in Mat do
  A(i,j) = 0.0;

writeln("A is:\n", A);

[(i,j) in Mat] A(i,j) = 1.0;

//Commented out as this for-expression syntax sugar is in
//direct conflict with the array literal production. 
//writeln("A is:\n", A);
//
//A = [Mat] 2.0;

writeln("A is:\n", A);

A = 3.0;

writeln("A is:\n", A);
