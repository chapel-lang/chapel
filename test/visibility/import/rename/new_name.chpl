module Foo {
  var x: int;

  proc bar() {
    writeln("In Foo.bar()");
  }
}
module User {
  import Foo as F;

  proc main() {
    writeln(F.x);
    F.bar();
  }
}
