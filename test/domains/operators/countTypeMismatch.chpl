const n = 10: uint(8);

var D1 = {1..n};
var D2 = {1..n, 1..n};

var A1: [D1] int = [i in D1] i;
var A2: [D2] int = [(i,j) in D2] i*10 + j;

config const c = 3, c2 = 4;

writeln(D1#c);
//writeln(D2#c);
writeln(D2#(c,c2));

writeln(A1#c);
//writeln(A2#c);
writeln(A2#(c,c2));
