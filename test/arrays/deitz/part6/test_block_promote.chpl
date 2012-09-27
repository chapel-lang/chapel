use BlockDist;

config var n: int = 8;

var A, B, C: [{1..n} dmapped Block(rank=1,boundingBox={1..n})] int;

for (a,b,i) in zip(A,B,1..n) do
  (a,b) = (i,n-i+1);

C = A + B;

writeln(A);
writeln(B);
writeln(C);
