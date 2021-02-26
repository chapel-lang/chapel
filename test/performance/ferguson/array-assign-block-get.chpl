use BlockDist;
use CommUtil;

config const n = 100000;

const Space = {1..n};
const D = Space dmapped Block(boundingBox=Space);
var A: [D] int;

var saveB1: int;
var saveBn: int;

for i in 1..n {
  A[i] = i;
}

start();

on Locales[1] {
  var B: [D] int;
  B; // avoiding split init as this is a test of =
  B = A;
  saveB1 = B[1];
  saveBn = B[n];
}

stop();

writeln(saveB1);
writeln(saveBn);

report(maxGets=37, maxPuts=4, maxOns=12);
