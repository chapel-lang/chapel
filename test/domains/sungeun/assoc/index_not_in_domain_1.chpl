config param parSafe = true;

var D1: domain(int, parSafe=parSafe);
D1 += max(int);
D1 += max(int)/2;
D1 += max(int)/4;
D1 += max(int)/8;
writeln(D1.sorted());

var D2: domain(int, parSafe=parSafe);
D2 += max(int);
D2 += max(int)/2;
D2 += max(int)/4;
D2 += max(int)/8;
D2 += max(int)/16;
writeln(D2.sorted());

D1 -= D2;
writeln(D2.sorted());

