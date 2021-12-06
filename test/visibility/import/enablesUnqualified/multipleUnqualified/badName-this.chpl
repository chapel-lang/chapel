module Foo {
  var bar: int;
}
module User {
  import Foo.{this}; // Should fail, this not defined in Foo

  proc main() {
    writeln(this);
  }
}
