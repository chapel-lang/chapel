var A = [111, 112;
         121, 122;
         ;
         211, 212;
         221, 222];

ref B = A.reshape(1..4, 1..2);
writeln(B);
B[2, 2] = 1;

ref C = A.reshape(1..2, 1..4);
writeln(C);
C[2, 2] = 2;

ref D = A.reshape(0..<8);
writeln(D);
D[2] = 3;

ref E = A.reshape(1..8);
writeln(E);
E[2] = 4;

writeln(A);

ref F = A.reshape(1..1, 1..2, 1..1, 1..2, 1..1, 1..2);
writeln(F);

var G: [1..2, 1..3, 1..5, 1..7] real;
forall (i,j,k,l) in G.domain do
  G[i,j,k,l] = i*1000 + j*100 + k*10 + l;
var H = G.reshape(1..10, 1..21);
writeln(H);

var I: [1..10, 1..21] real;
forall (i,j) in I.domain do
  I[i,j] = i*100 + j;
var J = I.reshape(1..2, 1..3, 1..5, 1..7);
writeln(J);

var K: [1..2, 1..4, 1..2] real;
forall (i,j,k) in K.domain do
  K[i,j,k] = i*100 + j*10 + k;
var L = K.reshape(1..4, 1..4);
writeln(L);

var M = [11, 12, 13, 14;
         21, 22, 23, 24;
         31, 32, 33, 34;
         41, 42, 43, 44];
var N = M.reshape(1..2, 1..4, 1..2);
writeln(N);

