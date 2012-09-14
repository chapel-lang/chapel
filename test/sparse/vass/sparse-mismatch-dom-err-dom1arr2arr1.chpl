config const n = 8;
var d: domain(1) = {1..n};
var sd1: sparse subdomain(d) = {1..n};
var sd2: sparse subdomain(d) = {1..n};
var a1: [sd1] real;
var a2: [sd2] real;

forall (i,j,k) in zip(sd1,a2,a1) do writeln(i,j,k);
