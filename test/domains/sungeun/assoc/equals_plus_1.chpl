import Sort;
config param parSafe = true;
config const n = 7;

var D1: domain(int, parSafe=parSafe);
D1 += n;
D1 += n-1;
D1 += n-2;
writeln(Sort.sorted(D1));

var D2: domain(int, parSafe=parSafe);
D2 += n;
D2 += n+1;
D2 += n+2;
writeln(Sort.sorted(D2));

var D3 = D1 + D2;
writeln(Sort.sorted(D3));

