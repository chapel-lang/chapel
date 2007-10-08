var A, B, C: [1..3] int;

C = (1, 3, 2);
B = (2, 3, 1);
A(B(C)) = 1..3;
writeln(C);
writeln(B);
writeln(A);
