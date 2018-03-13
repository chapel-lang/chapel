class ThisTooEarly {
  var r : real;

  proc init(rVal : real) {
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
  var c: ThisTooEarly = new ThisTooEarly(5);

  foo(c);

  delete c;
}
