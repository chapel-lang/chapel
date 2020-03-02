module Foo {
  var x: int;

  proc bar() {
    writeln("In Foo.bar()");
  }
}
module User {
  import Foo as F;

  proc main() {
    Foo.bar(); // verifies can't use old name to access functions
  }
}
