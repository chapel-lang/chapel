use Random;

use BlockDist;

config var n = 10000;

const ProblemSpace = {1..n} dmapped Block(boundingBox={1..n});

var A: [ProblemSpace] real;
var B: [ProblemSpace] real;

var randStr1 = new RandomStream(314159265);
var randStr2 = new RandomStream(314159265);

forall (i,r) in zip(A.domain, randStr1) {
  writeln("Doing iteration %##### on locale %i\n", i, here.id);
  A(i) = r;
}

for b in B do
  b = randStr2.getNext();

for (i,a,b) in zip(ProblemSpace,A,B) {
  if (a != b) then
    writef("mismatch at %######: %i != %i", i, a, b);
  else
    writef("%###### = %i\n", a);
}

