use BlockDist;

config var n: int = 8;

var A, B, C: [[1..n] distributed Block(rank=1,boundingBox=[1..n])] int;

for (a,b,i) in (A,B,1..n) do
  (a,b) = (i,n-i+1);

C = A + B;

writeln(A);
writeln(B);
writeln(C);
