// Return a global tuple and store it in a var.

record r { var x: int = 0; }

var g1 = (0, new r());

proc inferReturnType() ref {
  return g1;
}

proc specifyReturnType() ref: (int, r) {
  return g1;
}

// Return type is inferred.
proc test1() {
  writeln(g1);
  ref (rg1, rg2) = inferReturnType();
  rg1 = 128;
  rg2 = new r(128);
  writeln(g1);
}
test1();

g1 = (0, new r());

// Return type is specified.
proc test2() {
  writeln(g1);
  ref (rg1, rg2) = specifyReturnType();
  rg1 = 256;
  rg2 = new r(256);
  writeln(g1);
}
test2();

