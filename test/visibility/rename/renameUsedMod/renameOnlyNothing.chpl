module Foo {
  var x: bool;
}

module User {
  use Foo as Bar only;

  proc main() {
    writeln(Bar.x); // Should work, Foo should be renamed to Bar
  }
}
