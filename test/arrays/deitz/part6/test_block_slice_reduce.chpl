use BlockDist;

config var n: int = 8;

var A: [[1..n] distributed Block(rank=1,boundingBox=[1..n])] int;

for (a,i) in (A,1..n) do
  a = i;

writeln(+ reduce A(2..n-1));
