config const n = 5;

const D = {1..n};

var A: [D] real = [i in D] i;

writeln("A is: ", A);
writeln("A[2..n-1] is: ", A[2..n-1]);
writeln("A[2..] is: ", A[2..]);
writeln("A[..n-1] is: ", A[..n-1]);
writeln("A[..] is: ", A[..]);
