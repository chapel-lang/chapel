var A: [1..3, 1..3, 1..3] int = [(i,j,k) in {1..3, 1..3, 1..3}]
                                  (i-1)*9 + (j-1)*3 + k;

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
