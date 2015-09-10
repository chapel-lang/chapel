use Random;

use BlockDist;

config var n = 10000:int(32);

const D = {1..n} dmapped Block(idxType=int(32), boundingBox={1..n});

var A: [D] real;
var B: [D] real;

var randStr1 = new RandomStream(314159265);
var randStr2 = new RandomStream(314159265);

forall (i,r) in zip(A.domain, randStr1) do
  A(i) = r;

for b in B do
  b = randStr2.getNext();

for (i,a,b) in zip(D,A,B) {
  if (a != b) then
    writeln("mismatch at #", i, ": ", a, " != ", b);
  else
    writeln("#", i, " = ", a);
}

