module A {
  var x: int;
}
module B {
  public use A;
}
module C {
  import B.{x};

  proc main() {
    writeln(x);
  }
}
