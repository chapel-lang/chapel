config const n = 100;

var A: [1..100] real = [i in 1..100] i/10.0;

writeln(A);
writeln(A:int);
writeln(+ scan A:int);
