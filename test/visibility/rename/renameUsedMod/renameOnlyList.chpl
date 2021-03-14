module Foo {
  var x: bool;
  var y: int;
}

module User {
  use Foo as Bar only x;

  proc main() {
    writeln(Bar.x); // Should work, Foo should be renamed to Bar
    writeln(Bar.y); // Should work, Foo should be renamed to Bar
  }
}
