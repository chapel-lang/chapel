module Foo {
  var x: int;
}
module Bar {
  public use Foo as F;

  var y: bool;
}
module User {
  use Bar;

  proc main() {
    // Should work, publicly renaming a module should not prevent its symbols
    // from being used in an unqualified manner by clients.
    writeln(x);
  }
}
