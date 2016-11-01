class ThisTooEarly {
  var r: real;

  proc init(rVal: real) {
    foo(this); // Uh oh!
    r = rVal;
    super.init();
  }
}

// The function exists so that we can pass a "this" instance to it before we've
// finished initializing in Phase 1, to verify that this particular behavior is
// not allowed
proc foo(x: ThisTooEarly) {
  writeln(x.r);
}

proc main() {
  var c: ThisTooEarly = new ThisTooEarly(5);

  foo(c);
  delete c;
}
