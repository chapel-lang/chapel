use Random;

use BlockDist;

config var n = 10000;

const ProblemDist = new Block1D(idxType=int(32), bbox={1..n}, targetLocales=Locales);
const D: domain(1) dmapped(ProblemDist) = {1..n};

var A: [D] real;
var B: [D] real;

var randStr1 = new RandomStream(314159265);
var randStr2 = new RandomStream(314159265);

forall (i,r) in (A.domain, randStr1) do
  A(i) = r;

for b in B do
  b = randStr2.getNext();

for (i,a,b) in (D,A,B) {
  if (a != b) then
    writeln("mismatch at #", i, ": ", a, " != ", b);
  else
    writeln("#", i, " = ", a);
}

