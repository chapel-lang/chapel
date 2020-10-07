// Modify ref tuple arguments. Actual elements are references to globals.

record r { var x: int = 0; }

var g1 = 0;
var g2 = new r();

proc modifyRefArg(ref tup) {
  tup[0] = 128;
  tup[1] = new r(128);
}

// Actual is a tuple expression of locals.
proc test1() {
  var a = 0;
  var b = new r();

  writeln(a);
  writeln(b);

  modifyRefArg((a, b));

  writeln(a);
  writeln(b);
}
test1();

// Actual is a tuple expression of globals.
proc test2() {
  writeln(g1);
  writeln(g2);

  modifyRefArg((g1, g2));

  writeln(g1);
  writeln(g2);
}
test2();

