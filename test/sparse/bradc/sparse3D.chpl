config const n = 10;

const D = [1..n, 1..n, 1..n];

const DSps: sparse subdomain(D) = [i in 1..n] (i,i,i);

writeln("DSps is: ", DSps);

var A: [(i,j,k) in DSps] real = i*10 + j + k/10.0;

writeln("A is: ", A);
