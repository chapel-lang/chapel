// Modify ref tuple arguments. Actual elements are iterator index vars.
record r { var x: int = 0; }
class C { var x: int = 0; }
record foo { var x: int = 0; var y: r; var z: shared C?; }

var g1 = 0;
var g2 = new r();
var g3 = new shared C();
var g4 = new foo();
var g5 = (0, new r(), new shared C());

proc modifyRefArg(ref tup) {
  tup[0] = 128;
  tup[1] = new r(128);
  tup[2] = new shared C(128);
}

iter yieldTupleExprGlobals() ref {
  yield (g1, g2, g3);
}

iter yieldTupleExprObjectFields() ref {
  yield (g4.x, g4.y, g4.z);
}

iter yieldTupleGlobal() ref {
  yield g5;
}

// Yield tuple expression of globals.
proc test1() {
  writeln(g1);
  writeln(g2);
  writeln(g3);
  for tup in yieldTupleExprGlobals() {
    modifyRefArg(tup);
  }
  writeln(g1);
  writeln(g2);
  writeln(g3);
}
test1();

// Yield tuple expression of object fields.
proc test2() {
  writeln(g4);
  for tup in yieldTupleExprObjectFields() {
    modifyRefArg(tup);
  }
  writeln(g4);
}
test2();

// Yield tuple global.
proc test3() {
  writeln(g5);
  for tup in yieldTupleGlobal() {
    modifyRefArg(tup);
  }
  writeln(g5);
}
test3();

