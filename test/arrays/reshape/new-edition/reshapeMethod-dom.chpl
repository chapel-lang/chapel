var A = [111, 112;
         121, 122;
         ;
         211, 212;
         221, 222];

ref B = reshape(A, {1..4, 1..2});
writeln(B);
B[2, 2] = 1;

ref C = reshape(A, {1..2, 1..4});
writeln(C);
C[2, 2] = 2;

ref D = reshape(A, {0..<8});
writeln(D);
D[2] = 3;

ref E = reshape(A, {1..8});
writeln(E);
E[2] = 4;

writeln(A);

ref F = reshape(A, {1..1, 1..2, 1..1, 1..2, 1..1, 1..2});
writeln(F);
