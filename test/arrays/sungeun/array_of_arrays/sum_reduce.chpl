config param useReduce = true;
config const n = 8;

const D = {1..n};
var AxA: [D] [D] uint;
var asum: [D] uint;

for (i,j) in {D.dim(0),D.dim(0)} do
  AxA[i][j] = (i*j+j):uint;

asum = + reduce AxA;
writeln(asum);

asum = 0;
for i in D do
  asum += AxA[i];
writeln(asum);
