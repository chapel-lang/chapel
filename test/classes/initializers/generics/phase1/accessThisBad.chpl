class ThisTooEarly {
  param r : real;

  proc init(param rVal : real) {
    foo(this); // Uh oh!

    r = rVal;

    super.init();

    foo(this); // OK!
  }
}

proc foo(x: borrowed ThisTooEarly) {
  writeln(x.r);
}

proc main() {
  var c: borrowed ThisTooEarly(5.0) = new owned ThisTooEarly(5.0);

  foo(c);
}
