var A: [1..10, 1..10] real = [(i,j) in {1..10, 1..10}] i + j/10.0;

writeln(A.localSlice(1..10 by 2, 1..10 by 2));
writeln(A.localSlice({1..10 by 2, 1..10 by 2}));

A.localSlice(1..10 by 2, 1..10 by 2) = -1;
A.localSlice(1..10 by 2, 2..10 by 2) = -2;
A.localSlice(2..10 by 2, 1..10 by 2) = -3;
A.localSlice(2..10 by 2, 2..10 by 2) = -4;

writeln("A is: ", A);
