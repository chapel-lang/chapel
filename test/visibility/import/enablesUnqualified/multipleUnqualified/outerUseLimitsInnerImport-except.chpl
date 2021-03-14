module A {
  var x: int;
  var y: bool;
}
module B {
  public import A.{x, y};
}
module C {
  use B except y;

  proc main() {
    writeln(x);
  }
}
