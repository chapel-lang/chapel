module Foo {
  var x: int;
}
module Foo2 {
  var Bar: int;
}
module User {
  use Foo as Bar;
  use Foo2;

  proc main() {
    writeln(Bar.x); // Should this fail or work?
  }
}
