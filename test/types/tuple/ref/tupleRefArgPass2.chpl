// Modify ref tuple argument. Actual is a global value tuple.

record r { var x: int = 0; }

var g1 = (0, new r());

proc modifyRefArg(ref tup) {
  tup[0] = 128;
  tup[1] = new r(128);
}

// Actual is a local value tuple.
proc test1() {
  var tup = (0, new r());

  writeln(tup);

  modifyRefArg(tup);

  writeln(tup);
}
test1();

// Actual is a global value tuple.
proc test2() {
  writeln(g1);

  modifyRefArg(g1);

  writeln(g1);
}
test2();

