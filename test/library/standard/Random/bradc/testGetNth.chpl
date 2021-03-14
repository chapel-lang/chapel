use Random;

config var n = 100;

config param useNPB = true;
config param rtype = if useNPB then RNG.NPB else RNG.PCG;

var randStr1 = createRandomStream(real, 314159265, algorithm=rtype);
var randStr2 = createRandomStream(real, 314159265, algorithm=rtype);
var randStr3 = createRandomStream(real, 314159265, algorithm=rtype);

for i in 0..n-1 {
  const r1 = randStr1.getNext();
  const r2 = randStr2.getNth(i);
  const r3 = randStr3.getNth(i);
  if (r1 != r2 || r2 != r3) {
    writeln("mismatch at #", i, ": ", r1, " ?= ", r2, " ?= ", r3);
  } else {
    // Don't print the #s in this test; just check for a match
    //writeln("#", i, " = ", r2);
  }
}
