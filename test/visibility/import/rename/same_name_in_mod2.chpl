module Foo {
  var x: int = 5;
}
module User {
  import Foo as F;

  var F: int;

  proc main() {
    writeln(F.x);
  }
}
