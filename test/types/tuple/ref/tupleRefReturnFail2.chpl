// Return a tuple of references to unsafe lvalues and store it in a var.
record r { var x: int = 0; }

var g1 = 0;
var g2 = new r();

proc inferReturnType() ref {
  var local1: int = 0;
  var local2: r = new r();
  return (local1, local2);
}

proc specifyReturnType() ref: (int, r) {
  var local1: int = 0;
  var local2: r = new r();
  return (local1, local2);
}

// Return type is inferred.
proc test1() {
  writeln(g1);
  writeln(g2);
  var tup = inferReturnType();
  tup[0] = 128;
  tup[1] = new r(128);
  writeln(g1);
  writeln(g2);
}
test1();

// Return type is specified.
proc test2() {
  writeln(g1);
  writeln(g2);
  var tup = specifyReturnType();
  tup[0] = 256;
  tup[1] = new r(256);
  writeln(g1);
  writeln(g2);
}
test2();

