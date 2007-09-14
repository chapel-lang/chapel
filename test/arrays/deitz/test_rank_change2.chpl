var A: [1..3, 1..3, 1..3] int = 1..;

writeln("writeln(A[1..3, 1..3, 1..3])");
writeln(A[1..3, 1..3, 1..3]);

writeln("writeln(A[1..3, 1..3, 1])");
writeln(A[1..3, 1..3, 1]);
writeln("writeln(A[1..3, 1, 1..3])");
writeln(A[1..3, 1, 1..3]);
writeln("writeln(A[1, 1..3, 1..3])");
writeln(A[1, 1..3, 1..3]);

writeln("writeln(A[1..3, 1, 1])");
writeln(A[1..3, 1, 1]);
writeln("writeln(A[1, 1..3, 1])");
writeln(A[1, 1..3, 1]);
writeln("writeln(A[1, 1, 1..3])");
writeln(A[1, 1, 1..3]);
