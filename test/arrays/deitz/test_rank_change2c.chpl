var A: [1..9, 1..9, 1..9] int = 1..;

writeln("writeln(A[1..9, 1..9, 1..9])");
writeln(A[1..9, 1..9, 1..9]);

writeln("writeln(A[3..9 by 2, 3..7 by 4, 4])");
writeln(A[3..9 by 2, 3..7 by 4, 4]);
writeln("writeln(A[3..9 by 2, 4, 3..7 by 4])");
writeln(A[3..9 by 2, 4, 3..7 by 4]);
writeln("writeln(A[4, 3..9 by 2, 3..7 by 4])");
writeln(A[4, 3..9 by 2, 3..7 by 4]);

writeln("writeln(A[3..9 by 2, 4, 4])");
writeln(A[3..9 by 2, 4, 4]);
writeln("writeln(A[4, 3..9 by 2, 4])");
writeln(A[4, 3..9 by 2, 4]);
writeln("writeln(A[4, 4, 3..9 by 2])");
writeln(A[4, 4, 3..9 by 2]);
