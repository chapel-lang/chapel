config const n = 10;

var D1: domain(int);
for i in 0..n do
  D1 += max(int)-i;
writeln(D1.sorted());

var D2: domain(int);
for i in 0..n do
  D2 += min(int)+i;
writeln(D2.sorted());

D2 += D1;
writeln(D2.sorted());
