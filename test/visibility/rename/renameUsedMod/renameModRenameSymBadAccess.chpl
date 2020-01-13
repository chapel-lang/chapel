module Foo {
  var x: int;
}

module User {
  use Foo as Bar only x as y;

  proc main() {
    writeln(x); // Verifies that renaming the module doesn't break renaming x
  }
}
