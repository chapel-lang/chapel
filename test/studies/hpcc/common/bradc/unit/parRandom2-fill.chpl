use Random;

use BlockDist;

config var n = 10000: int(64);

const ProblemDist = new dmap(new Block(rank=1, idxType=int(64), boundingBox={1..n}, targetLocales=Locales));
const ProblemSpace: domain(1, int(64)) dmapped(ProblemDist) = {1..n};

var A: [ProblemSpace] real;
var B: [ProblemSpace] real;

var randStr1 = new RandomStream(314159265);
var randStr2 = new RandomStream(314159265);

randStr1.fillRandom(A);

for b in B do
  b = randStr2.getNext();

for (i,a,b) in zip(ProblemSpace,A,B) {
  if (a != b) then
    writef("mismatch at #%{#####}: %r != %r", i, a, b);
  else
    writef("#%{#####} = %r\n", i, a);
}

delete randStr1;
delete randStr2;
