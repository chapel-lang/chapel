class ThisTooEarly {
  param r : real;

  proc init(param rVal : real) {
    foo(this); // Uh oh!

    r = rVal;

    super.init();

    foo(this); // OK!
  }
}

proc foo(x: ThisTooEarly) {
  writeln(x.r);
}

proc main() {
  var c: ThisTooEarly(5.0) = new ThisTooEarly(5.0);

  foo(c);

  delete c;
}
