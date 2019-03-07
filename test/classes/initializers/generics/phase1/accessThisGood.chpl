class ThisEarly {
  param r: real;

  proc init(param r: real) {
    this.r = r; // This is the only accepted use of this
  }
}

proc main() {
  var c: unmanaged ThisEarly(5.0) = new unmanaged ThisEarly(5.0);

  writeln(c.r);

  delete c;
}

