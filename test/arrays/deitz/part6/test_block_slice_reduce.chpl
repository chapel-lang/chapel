use BlockDist;

config var n: int = 8;

var A: [{1..n} dmapped Block(rank=1,boundingBox={1..n})] int;

for (a,i) in zip(A,1..n) do
  a = i;

writeln(+ reduce A(2..n-1));
