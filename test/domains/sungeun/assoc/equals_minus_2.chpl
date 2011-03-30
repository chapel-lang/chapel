config const n = 7;

var D1: domain(int);
D1 += n;
D1 += n-1;
D1 += n-2;
writeln(D1.sorted());

var D2 = D1 - n;
writeln(D2.sorted());

