module Foo {
  var x: int;
}

module User {
  use Foo as Bar only x as y;

  proc main() {
    // Verifies that renaming the module and renaming a symbol in it doesn't
    // allow the old name of the module to be used with the renamed symbol
    writeln(Foo.y);
  }
}
