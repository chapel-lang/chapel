config const n = 5;

const D = {1..n, 1..n};

var A: [D] real = [(i,j) in D] (i-1)*n + j;

writeln("A is:\n", A);
writeln();

writeln("A[2..n-1, 2..n-1] is:\n", A[2..n-1, 2..n-1]);
writeln();

writeln("A[2.., 2..] is:\n", A[2.., 2..]);
writeln();

writeln("A[..n-1, n-1] is:\n", A[..n-1, ..n-1]);
writeln();

writeln("A[.., ..] is:\n", A[.., ..]);
writeln();

writeln("A[.., 2..] is:\n", A[.., 2..]);
writeln();

writeln("A[1..1, ..] is:\n", A[1..1, ..]);
writeln();

