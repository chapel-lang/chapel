config param useReduce = true;
config const n = 8;

const D = {1..n};
var AxA: [D] [D] uint;
var bxor: [D] uint;

for (i,j) in {D.dim(0), D.dim(0)} do
  AxA[i][j] = max(uint)-(i*j+j):uint;

bxor = ^ reduce AxA;
writeln(bxor);

bxor = 0;
for i in D do
  bxor ^= AxA[i];
writeln(bxor);

