module Foo {
  var x: bool;
}

module User {
  use Foo as Bar only;

  proc main() {
    writeln(x); // Should fail, renaming shouldn't break `only;`
  }
}
