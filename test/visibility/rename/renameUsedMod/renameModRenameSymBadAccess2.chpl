module Foo {
  var x: int;
}

module User {
  use Foo as Bar only x as y;

  proc main() {
    // Verifies that renaming the module doesn't allow qualified access of
    // symbols that don't exist in that context, even using the rename
    // This is in keeping with normal renaming
    writeln(Bar.y);
  }
}
