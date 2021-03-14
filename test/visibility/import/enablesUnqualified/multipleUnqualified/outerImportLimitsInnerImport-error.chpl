module A {
  var x: int;
  var y: bool;
}
module B {
  public import A.{x, y};
}

module C {
  import B.{x};

  proc main() {
    writeln(y);
  }
}
