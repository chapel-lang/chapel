// Function yielding tuple of references to globals. Captured in a
// variable, which means updates are not visible, and the iterator
// contents are converted to an array.

record r { var x: int = 0; }

var g1 = 0;
var g2 = new r();

iter inferYieldType() ref {
  yield (g1, g2);
}

iter specifyYieldType() ref: (int, r) {
  yield (g1, g2);
}

// Return type is inferred.
proc test1() {
  writeln(g1);
  writeln(g2);
  var tup = inferYieldType();
  writeln(tup.type:string);
  writeln(tup);
  tup[0][0] = 128;
  tup[0][1] = new r(128);
  writeln(g1);
  writeln(g2);
}
test1();

// Return type is specified.
proc test2() {
  var tup = specifyYieldType();
  writeln(tup.type:string);
  writeln(tup);
  tup[0][0] = 256;
  tup[0][1] = new r(256);
  writeln(tup);
  writeln(g1);
  writeln(g2);
}
test2();

