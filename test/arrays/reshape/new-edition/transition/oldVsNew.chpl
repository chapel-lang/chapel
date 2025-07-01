var A = [1, 2, 3, 4];
ref B = reshape(A, {1..2, 1..2});
writeln(B);
writeln();

B[1,1] = 5;
writeln(A);

