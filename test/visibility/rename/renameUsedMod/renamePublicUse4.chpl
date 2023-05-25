module Foo {
  var x: int;
}
module Bar {
  public use Foo as Foo;

  var y: bool;
}
module User {
  use Bar;

  proc main() {
    // Should work, because the user has opted in to having Bar
    // bring in the name Foo.
    writeln(Foo.x);
  }
}
