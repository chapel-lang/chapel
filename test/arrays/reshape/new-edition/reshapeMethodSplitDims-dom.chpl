var G: [1..2, 1..3, 1..5, 1..7] real;
forall (i,j,k,l) in G.domain do
  G[i,j,k,l] = i*1000 + j*100 + k*10 + l;
var H = reshape(G, {1..10, 1..21});
writeln(H);

var I: [1..10, 1..21] real;
forall (i,j) in I.domain do
  I[i,j] = i*100 + j;
var J = reshape(I, {1..2, 1..3, 1..5, 1..7});
writeln(J);

var K: [1..2, 1..4, 1..2] real;
forall (i,j,k) in K.domain do
  K[i,j,k] = i*100 + j*10 + k;
var L = reshape(K, {1..4, 1..4});
writeln(L);

var M = [11, 12, 13, 14;
         21, 22, 23, 24;
         31, 32, 33, 34;
         41, 42, 43, 44];
var N = reshape(M, {1..2, 1..4, 1..2});
writeln(N);

var O = [11, 12, 13;
         21, 22, 23];

var P = reshape(O, {1..3, 1..2});
writeln(P);

