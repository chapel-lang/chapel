import Sort;
config param parSafe = true;
config const n = 10;

var D1: domain(int, parSafe=parSafe);
for i in 0..n do
  D1 += max(int)-i;
writeln(Sort.sorted(D1));

var D2: domain(int, parSafe=parSafe);
for i in 0..n do
  D2 += min(int)+i;
writeln(Sort.sorted(D2));

serial !parSafe do D2 += D1;
writeln(Sort.sorted(D2));
