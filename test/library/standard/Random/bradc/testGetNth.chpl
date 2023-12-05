use Random;

config var n = 100;

var randStr1 = new randomStream(real, 314159265);
var randStr2 = new randomStream(real, 314159265);
var randStr3 = new randomStream(real, 314159265);

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
