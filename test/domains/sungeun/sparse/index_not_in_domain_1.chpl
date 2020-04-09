config const n = 7;

const D = {1..max(int)};

var D1: sparse subdomain(D);
D1 += max(int);
D1 += max(int)/2;
D1 += max(int)/4;
D1 += max(int)/8;
writeln(D1);

var D2: sparse subdomain(D);
D2 += max(int);
D2 += max(int)/2;
D2 += max(int)/4;
D2 += max(int)/8;
D2 += max(int)/16;
writeln(D2);

D1 -= D2;
writeln(D2);

