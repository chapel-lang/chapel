config var n = 10;

const D = {1..n, 1..n};

var A: [D] int = [(i,j) in D] i*10 + j;

writeln("A is:\n", A);
writeln();

writeln("A[<slice>] is:\n", A[1..n by 2, 2..n by 2]);
writeln();

writeln("A[<slice>] is:\n", A[1..n-1, 2..n]);
writeln();

writeln("A[<slice>] is:\n", A[0..n-1, 2..n+1]);
writeln();
