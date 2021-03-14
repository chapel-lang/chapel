module Foo {
  var x: int;
}

module User {
  use Foo as Bar only x as y;

  proc main() {
    writeln(y); // Foo.x
    writeln(Bar.x); // Should work, as Foo is treated as Bar, and has an x
  }
}
