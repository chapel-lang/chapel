config var n = 8;

var D = {1..n, 1..n};

var A: [D] real;

[(i,j) in D] A(i,j) = i*10 + j;

writeln("A is:\n", A);

writeln("A(1) is: ", A(1));

writeln("A(1)(2) is: ", A(1)(2));
