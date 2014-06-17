use Random;

use BlockDist;

config var n = 10000: int(64);

const ProblemDist = new Block1D(bbox={1..n}, targetLocales=Locales);
const ProblemSpace: domain(1, int(64)) dmapped(ProblemDist) = {1..n};

var A: [ProblemSpace] real;
var B: [ProblemSpace] real;

var randStr1 = new RandomStream(314159265);
var randStr2 = new RandomStream(314159265);

forall (i,r) in (A.domain, randStr1) {
  writeln("Doing iteration ", format("#####", i), " on locale ", here.id);
  A(i) = r;
}

for b in B do
  b = randStr2.getNext();

for (i,a,b) in (ProblemSpace,A,B) {
  if (a != b) then
    writeln("mismatch at #", format("#####", i), ": ", a, " != ", b);
  else
    writeln("#", format("#####", i), " = ", a);
}

