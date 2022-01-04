// This test was added to track a bug with ignoring where clauses when one of
// the actuals would accidentally stay generic in the call.
class C {
  var x;
}

// This function is bad because the y argument ends up generic, but it should
// be eliminated as a candidate in this particular instance because of the where
// clause
proc foo(param x: bool, y: borrowed C? = nil) where (x == true) {
  writeln(y);
}

// This is the function that should be called
proc foo(param x: bool, y: borrowed C(int)? = nil) where (x == false) {
  writeln(y);
}

foo(false);
