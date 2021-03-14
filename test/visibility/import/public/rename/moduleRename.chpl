module A {
  var x = 2;
}
module B {
  var y = 4;
}

module C {
  public import A as X;
}

module Z {
  use C;
  proc main() {
    writeln(X.x);
  }
}
