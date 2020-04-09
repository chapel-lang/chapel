module X {
  proc f() {
    writeln("in X.f()");
  }
}

module Y {
  proc main() {
    X.f();
    Z.f();
  }
}
