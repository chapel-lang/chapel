config const n = 6;

const D = {1..n*n};

var D1: sparse subdomain(D);
for i in 1..D.size by n do
  D1 += i;
writeln(D1);

var D2: sparse subdomain(D);
for i in 1..D.size by n/2 do
  D2 += i;
writeln(D2);

D2 -= D1;
writeln(D2);
