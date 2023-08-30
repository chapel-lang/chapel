use LinearAlgebra;
var A = Matrix([0,1.0],[0,0]); // gives error about expecting int but got real
var B = Matrix([0.0,1],[0,0]); // gives error about expecting float but got int
var C = Matrix([0.0,1.0],[0,0]); // gives error about dealing with matrix made from two arrays of different type
var D = Matrix([0,1],[0,0],real); // compiles as matrix full of reals
var E = Matrix([0.0,1],[0,0],real); // compilation error like B above
writeln(A);
writeln();
writeln(B);
writeln();
writeln(C);
writeln();
writeln(D);
writeln();
writeln(E);
