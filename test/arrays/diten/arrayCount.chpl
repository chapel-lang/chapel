var A: [1..10] int = [i in 1..10] i;
writeln(+ reduce (A == 3));

A[7..9] = 3;
writeln(+ reduce (A == 3));
writeln(+ reduce (A == 11));
