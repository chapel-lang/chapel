module Foo {
  var x: int;
}
module Bar {
  private use Foo as F;

  var y: bool;
}
module User {
  use Bar;

  proc main() {
    // Should fail, renaming a module shouldn't break the use privacy and allow
    // symbols in the private module to be used in a qualified manner with the
    // old module name (we still need to use Foo in a visible manner)
    writeln(Foo.x);
  }
}
