module X {
  enum E { one, two, three };
}

module Y {
  proc main() {
    var e: X.E;
    writeln(e);

    Z.f();
  }
}
