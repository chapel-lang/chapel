config const n = 7;

var D1 = {1..n-1};
writeln(D1);

var D2 = {n..2*n};
writeln(D2);

D1 += D2;
writeln(D1);

