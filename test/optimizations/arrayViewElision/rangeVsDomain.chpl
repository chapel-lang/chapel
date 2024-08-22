var A, B: [1..10] int;
B = 1;

A[1..9] = B[{1..9}];

writeln(A);
