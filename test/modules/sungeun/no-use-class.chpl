module X {
  class C { var x: int; }
}

module Y {
  proc main() {
    var c: borrowed X.C?;
    writeln(c);

    Z.f();
  }
}
