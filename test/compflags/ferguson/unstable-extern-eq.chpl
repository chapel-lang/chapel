
extern type mytype;
extern proc return_a_mytype():mytype;

// This should give a warning with --warn-unstable
// because later on we might require an init=.
// When that happens (or something else replaces it),
// this test should be removed.
operator =(lhs: mytype, rhs: real) {
  writeln("in = ");
}
operator :(from: real, type toType: mytype) {
  var x: mytype = return_a_mytype();
  x = from;
  return x;
}

proc main() {
  var x: mytype = return_a_mytype();
  var y: mytype = return_a_mytype();
  x = y;
  y = 1.0;
}
