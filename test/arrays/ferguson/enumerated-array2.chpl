enum classVals {S, W, A, B, C, D};

proc doit() {
  const Class: domain(classVals);
  var probSizes:   [Class] int;

  probSizes(classVals.S) = 1400;
  probSizes(classVals.W) = 7000;

  ref x = probSizes(classVals.S);
  x += 1;
  writeln("x ", x);
  assert(x == 1401);
  var y = probSizes(classVals.W);
  writeln("y ", y);
  assert(y == 7000);
}

doit();
