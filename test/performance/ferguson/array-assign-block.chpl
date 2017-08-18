use BlockDist;
use CommUtil;

config const n = 100000;

const Space = {1..n};
const D = Space dmapped Block(boundingBox=Space);
var A: [D] int;
var B: [D] int;

for i in 1..n {
  A[i] = i;
}

start();

B = A;

stop();

writeln(B[1]);
writeln(B[n]);

report(maxGets=0, maxPuts=0, maxOns=2);
