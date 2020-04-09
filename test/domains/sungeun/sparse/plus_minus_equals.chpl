config const n = 7;

const D = {1..max(int)};

var D1: sparse subdomain(D);
D1 += max(int);
D1 += max(int)/2;
D1 += max(int)/4;
D1 += max(int)/8;
writeln(D1);

D1 += 1;
writeln(D1);

D1 -= max(int)/4;
writeln(D1);

D1 += 1;
writeln(D1);

D1 -= max(int);
writeln(D1);

D1 += 1;
writeln(D1);

D1 -= max(int)/8;
writeln(D1);

D1 += 1;
writeln(D1);

D1 -= max(int)/2;
writeln(D1);

D1 += 1;
writeln(D1);

