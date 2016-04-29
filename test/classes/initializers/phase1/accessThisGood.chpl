class ThisEarly {
  var r: real;

  proc init(r: real) {
    this.r = r; // This is the only accepted use of this
    super.init();
  }
}

proc main() {
  var c: ThisTooEarly = new ThisTooEarly(5);

  writeln(c.r);

  delete c;
}

