config const n = 10;

const D = {1..n*n};

var D1: sparse subdomain(D);
for i in 0..n do
  D1 += max(int)-i;
writeln(D1);

var D2: sparse subdomain(D);
for i in 0..n do
  D2 += min(int)+i;
writeln(D2);

D2 += D1;
writeln(D2);
