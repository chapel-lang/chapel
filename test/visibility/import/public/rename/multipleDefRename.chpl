module A {
  var x = 2;
}
module B {
  var x = 4;
}
module C {
  public import A.{x as y};
}

module Z {
  use C, B;
  proc main() {
    writeln(x);
    writeln(y);
  }
}
