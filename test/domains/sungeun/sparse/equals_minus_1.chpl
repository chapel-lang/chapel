config const n = 7;

const D = {1..n*n};

var D1: sparse subdomain(D);
D1 += n+2;
D1 += n+1;
D1 += n;
D1 += n-1;
D1 += n-2;
writeln(D1);

var D2: sparse subdomain(D);
D2 += n;
D2 += n+1;
D2 += n+2;
writeln(D2);

var D3 = D1 - D2;
writeln(D3);

