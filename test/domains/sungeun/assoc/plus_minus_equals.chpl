import Sort;
config param parSafe = true;

var D1: domain(int, parSafe=parSafe);
D1 += max(int);
D1 += max(int)/2;
D1 += max(int)/4;
D1 += max(int)/8;
writeln(Sort.sorted(D1));

D1 += 1;
writeln(Sort.sorted(D1));

D1 -= max(int)/4;
writeln(Sort.sorted(D1));

D1 += 1;
writeln(Sort.sorted(D1));

D1 -= max(int);
writeln(Sort.sorted(D1));

D1 += 1;
writeln(Sort.sorted(D1));

D1 -= max(int)/8;
writeln(Sort.sorted(D1));

D1 += 1;
writeln(Sort.sorted(D1));

D1 -= max(int)/2;
writeln(Sort.sorted(D1));

D1 += 1;
writeln(Sort.sorted(D1));

