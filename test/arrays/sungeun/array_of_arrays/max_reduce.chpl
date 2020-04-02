config param useReduce = true;
config const n = 8;

const D = {1..n};
var AxA: [D] [D] uint;
var amax: [D] uint = min(uint);

for (i,j) in {D.dim(0),D.dim(0)} do
  AxA[i][j] = (i*j+j):uint;

amax = max reduce AxA;
writeln(amax);

amax = min(uint);
for i in D do
  for j in D do
    amax[i] = max(amax[i], AxA[i][j]);
writeln(amax);
