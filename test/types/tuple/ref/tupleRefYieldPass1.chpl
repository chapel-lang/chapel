// Function yielding tuple of references to globals. Captured in a
// variable, which means updates are not visible.

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
  for tup in inferYieldType() {
    writeln(g1);
    writeln(g2);
    tup[0] = 128;
    tup[1] = new r(128);
    writeln(g1);
    writeln(g2);
  }
}
test1();

g1 = 0;
g2 = new r();

// Return type is specified.
proc test2() {
  for tup in specifyYieldType() {
    writeln(g1);
    writeln(g2);
    tup[0] = 256;
    tup[1] = new r(256);
    writeln(g1);
    writeln(g2);
  }
}
test2();

