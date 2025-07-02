var A = [1, 2; 3, 4];

// new overloads

ref B = reshape(A, 1.., copy=true);
B[1] = 5;

ref C = reshape(A, 1.., copy=false);
C[2] = 6;

ref D = reshape(A, 1.., checkDims=true, copy=true);
D[3] = 7;

ref E = reshape(A, 1.., checkDims=true, copy=false);
E[4] = 8;

writeln(A);
writeln(B);
writeln(C);
writeln(D);
writeln(E);


// old ones for completeness

ref F = reshape(A, 1..);
F[1] = 9;

ref G = reshape(A, 1.., checkDims=true);
G[3] = 10;

writeln(A);
writeln(F);
writeln(G);

