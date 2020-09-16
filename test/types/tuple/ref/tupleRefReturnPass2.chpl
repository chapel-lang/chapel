// Return a tuple of references to globals and store it in a ref.

record r { var x: int = 0; }

var g1 = 0;
var g2 = new r();

proc inferReturnType() ref {
  return (g1, g2);
}

proc specifyReturnType() ref: (int, r) {
  return (g1, g2);
}

// Return type is inferred.
proc test1() {
  writeln(g1, g2);
  ref tup = inferReturnType();
  tup[0] = 128;
  tup[1] = new r(128);
  writeln(g1, g2);
}
test1();

// Return type is specified.
proc test2() {
  writeln(g1, g2);
  ref tup = specifyReturnType();
  tup[0] = 256;
  tup[1] = new r(256);
  writeln(g1, g2);
}
test2();

