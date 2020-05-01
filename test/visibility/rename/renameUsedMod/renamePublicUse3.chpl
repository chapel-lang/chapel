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
    // Should fail, publicly renaming a module should not allow the use of the
    // old module name as a prefix for qualified access
    writeln(Foo.x);
  }
}
