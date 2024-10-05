import Sort;
config param parSafe = true;
config const n = 7;

var D1: domain(int, parSafe=parSafe);
D1 += n;
D1 += n-1;
D1 += n-2;
writeln(Sort.sorted(D1));

var D2 = D1 - n;
writeln(Sort.sorted(D2));

