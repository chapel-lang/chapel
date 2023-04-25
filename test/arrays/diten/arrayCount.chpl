var A: [1..10] int = [i in 1..10] i;
writeln(A.count(3));

A[7..9] = 3;
writeln(A.count(3));
writeln(A.count(11));
