config var n = 9;

var D: domain(1) = [1..n];

var A: [i in D] real = i;

writeln("A is: ", A);

A.domain = [1..2*n];

writeln("A is: ", A);
