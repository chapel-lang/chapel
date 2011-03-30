var D1: domain(int);
D1 += max(int);
D1 += max(int)/2;
D1 += max(int)/4;
D1 += max(int)/8;
writeln(D1.sorted());

var D2: domain(int);
D2 += max(int)/8;
D2 += max(int)/4;
D2 += max(int)/2;
D2 += max(int);
D2 += max(int)/16;
D2 += max(int)/32;
D2 += max(int)/64;
writeln(D2.sorted());

D2 -= D1;
writeln(D2.sorted());
