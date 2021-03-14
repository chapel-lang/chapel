module Foo {
  var bar: int;
}
module User {
  import Foo.{baz}; // Should fail, baz not defined in Foo

  proc main() {
    writeln(baz);
  }
}
