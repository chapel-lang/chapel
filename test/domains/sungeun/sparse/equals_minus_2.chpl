config const n = 7;

const D = {1..n*n};

var D1: sparse subdomain(D);
D1 += n;
D1 += n-1;
D1 += n-2;
writeln(D1);

var D2 = D1 - n;
writeln(D2);

