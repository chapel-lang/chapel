config const n = 7;

const D = {1..n*n};

var D1: sparse subdomain(D);
D1 += max(int);
D1 += max(int)/2;
D1 += max(int)/4;
D1 += max(int)/8;
writeln(D1);

var D2: sparse subdomain(D);
D2 += max(int)/8;
D2 += max(int)/4;
D2 += max(int)/2;
D2 += max(int);
D2 += max(int)/16;
D2 += max(int)/32;
D2 += max(int)/64;
writeln(D2);

D2 -= D1;
writeln(D2);
