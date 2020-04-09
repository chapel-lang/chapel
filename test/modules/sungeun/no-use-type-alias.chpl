module X {
  type T = string;
}

module Y {
  proc main() {
    var t: X.T;
    writeln("[",t,"]");

    Z.f();
  }
}
