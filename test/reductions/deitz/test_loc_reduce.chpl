var A: [1..5, 1..5] real = 0.0;
A[2, 4] = 2.0;
A[4, 2] = -2.0;

writeln(minloc reduce zip(A, A.domain));
writeln(maxloc reduce zip(A, A.domain));
