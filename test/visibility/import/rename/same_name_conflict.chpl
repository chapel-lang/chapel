module Foo {
  var x: int;
}
module F {
  var y: bool;
}
module User {
  import Foo as F;
  import F;

  proc main() {
    writeln(F.x);
  }
}
