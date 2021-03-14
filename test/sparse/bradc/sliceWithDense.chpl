config const n = 10;
const D = {1..n, 1..n};
const DSps: sparse subdomain(D) = [i in 1..n] (i,i);
var A: [(i,j) in DSps] int = i*10 + j;

writeln("A[D] is:\n", A[D]);
writeln();
writeln("A[2..n-1, 2..n-1] is:\n", A[2..n-1, 2..n-1]);
writeln();
