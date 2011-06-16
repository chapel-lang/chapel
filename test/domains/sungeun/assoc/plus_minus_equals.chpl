config param parSafe = true;

var D1: domain(int, parSafe=parSafe);
D1 += max(int);
D1 += max(int)/2;
D1 += max(int)/4;
D1 += max(int)/8;
writeln(D1.sorted());

D1 += 1;
writeln(D1.sorted());

D1 -= max(int)/4;
writeln(D1.sorted());

D1 += 1;
writeln(D1.sorted());

D1 -= max(int);
writeln(D1.sorted());

D1 += 1;
writeln(D1.sorted());

D1 -= max(int)/8;
writeln(D1.sorted());

D1 += 1;
writeln(D1.sorted());

D1 -= max(int)/2;
writeln(D1.sorted());

D1 += 1;
writeln(D1.sorted());

