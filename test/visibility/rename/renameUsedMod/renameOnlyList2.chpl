module Foo {
  var x: bool;
  var y: int;
}

module User {
  use Foo as Bar only x;

  proc main() {
    writeln(x);
    writeln(y); // Should fail, renaming shouldn't break only lists
  }
}
