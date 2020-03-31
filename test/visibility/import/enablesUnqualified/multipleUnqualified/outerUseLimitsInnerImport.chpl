module A {
  var x: int;
  var y: bool;
}
module B {
  public import A.{x, y};
}
module C {
  use B only x;

  proc main() {
    writeln(x);
  }
}
