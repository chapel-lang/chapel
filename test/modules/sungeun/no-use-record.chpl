module X {
  record R { var x: int; }
}

module Y {
  proc main() {
    var r: X.R;
    writeln(r);

    Z.f();
  }
}
