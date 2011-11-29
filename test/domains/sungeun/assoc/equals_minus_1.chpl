config param parSafe = true;
config const n = 7;

var D1: domain(int, parSafe=parSafe);
D1 += n+2;
D1 += n+1;
D1 += n;
D1 += n-1;
D1 += n-2;
writeln(D1.sorted());

var D2: domain(int, parSafe=parSafe);
D2 += n;
D2 += n+1;
D2 += n+2;
writeln(D2.sorted());

var D3 = D1 - D2;
writeln(D3.sorted());

