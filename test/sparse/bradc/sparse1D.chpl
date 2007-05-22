config const n = 10;

const D = [1..n];

const DSps: sparse subdomain(D) = (1, 3, 5, 7, 9);

writeln("DSps is: ", DSps);

var A: [i in DSps] real = i;

writeln("A is: ", A);
