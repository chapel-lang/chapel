// Return a global tuple and store it in a ref.

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
  ref tup = inferReturnType();
  tup[0] = 128;
  tup[1] = new r(128);
  writeln(g1);
}
test1();

// Return type is specified.
proc test2() {
  writeln(g1);
  ref tup = specifyReturnType();
  tup[0] = 256;
  tup[1] = new r(256);
  writeln(g1);
}
test2();

