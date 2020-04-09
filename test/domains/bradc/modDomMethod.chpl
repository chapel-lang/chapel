config var n = 9;

var D: domain(1) = {1..n};

var A: [D] real = [i in D] i;

writeln("A is: ", A);

A.domain = {1..2*n};

writeln("A is: ", A);
