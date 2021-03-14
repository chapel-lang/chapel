var A: [1..10] real = [i in 1..10] i;

ref B = A[2..9];
writeln("B is: ", B);

ref C = A[1..10 by 2];
writeln("C is: ", C);

ref D = A[1..10 by -2];
writeln("D is: ", D);
