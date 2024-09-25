var A, B: [1..10] int;

B = 1;

A[1..10 by 2] = B[1..10 by 2];

writeln(A);

A = 0;

A[1..5] = B[1..10 by 2];

writeln(A);

A = 0;

A[1..10 by 2] = B[1..5];

writeln(A);
