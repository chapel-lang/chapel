config const n = 10;

const D = {1..n, 1..n};
const D2 = D by (2,2);

var A: [D2] int = [(i,j) in D2] i*10 + j;

writeln("A is:\n", A);
writeln();

writeln("A[<slice>] is:\n", A[1..n by 2, 1..n by 2]);
writeln();

writeln("A[<slice>] is:\n", A[3..n-1 by 2, 3..n-1 by 2]);
writeln();

writeln("A[<slice>] is:\n", A[1..n, 2..n]);
writeln();

writeln("A[<slice>] is:\n", A[0..n-1, 2..n+1]);
writeln();
