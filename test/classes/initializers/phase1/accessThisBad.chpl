class ThisTooEarly {
  var r : real;

  proc init(rVal : real) {
    foo(this); // Uh oh!

    r = rVal;

    super.init();

    foo(this); // OK!
  }
}

proc foo(x: shared ThisTooEarly) {
  writeln(x.r);
}

proc main() {
  var c: shared ThisTooEarly = new shared ThisTooEarly(5);

  foo(c);
}
