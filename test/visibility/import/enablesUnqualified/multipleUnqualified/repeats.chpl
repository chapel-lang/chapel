module Foo {
  var bar: int;
  var baz = "blah";
}
module User {
  import Foo.{bar, bar}; // Should work but warn, can't repeat

  proc main() {
    writeln(bar);
  }
}
