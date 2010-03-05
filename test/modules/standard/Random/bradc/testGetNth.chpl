use Random;

config var n = 100;

var randStr1 = new RandomStream(314159265);
var randStr2 = new RandomStream(314159265);
var randStr3 = new RandomStream(314159265);

for i in 1..n {
  const r1 = randStr1.getNext();
  const r2 = randStr2.getNth(i);
  const r3 = randStr3.getNth(i);
  if (r1 != r2 || r2 != r3) {
    writeln("mismatch at #", i, ": ", r1, " ?= ", r2, " ?= ", r3);
  } else {
    writeln("#", i, " = ", r2);
  }
}

delete randStr1;
delete randStr2;
delete randStr3;
