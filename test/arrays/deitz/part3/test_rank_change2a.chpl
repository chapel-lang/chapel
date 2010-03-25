var A: [1..3, 1..3, 1..3] int = 1..;

writeln("writeln(A[1..3, 1..3, 1..3])");
writeln(A[1..3, 1..3, 1..3]);

writeln("writeln(A[1..3, 1..3, 2])");
writeln(A[1..3, 1..3, 2]);
writeln("writeln(A[1..3, 2, 1..3])");
writeln(A[1..3, 2, 1..3]);
writeln("writeln(A[2, 1..3, 1..3])");
writeln(A[2, 1..3, 1..3]);

writeln("writeln(A[1..3, 2, 2])");
writeln(A[1..3, 2, 2]);
writeln("writeln(A[2, 1..3, 2])");
writeln(A[2, 1..3, 2]);
writeln("writeln(A[2, 2, 1..3])");
writeln(A[2, 2, 1..3]);
