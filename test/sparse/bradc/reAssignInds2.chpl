config var n = 9;

const D = [1..n, 1..n];

var DSps: sparse subdomain(D) = [i in 1..n] (i,i);

var A: [(i,j) in DSps] real = i + j/10.0;

writeln("DSps is: ", DSps);
writeln("A is:\n", A);

DSps = [i in 1..n] (i,1);

writeln("DSps is: ", DSps);
writeln("A is:\n", A);

DSps = [i in 1..n] (1,i);

writeln("DSps is: ", DSps);
writeln("A is:\n", A);
