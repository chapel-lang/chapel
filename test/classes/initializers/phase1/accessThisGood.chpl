class ThisEarly {
  var r: real;

  proc init(r: real) {
    this.r = r; // This is the only accepted use of this
  }
}

proc main() {
  var c: unmanaged ThisEarly = new unmanaged ThisEarly(5);

  writeln(c.r);

  delete c;
}

