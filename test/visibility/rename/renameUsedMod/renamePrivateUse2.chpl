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
    // renamed module name
    writeln(F.x);
  }
}
