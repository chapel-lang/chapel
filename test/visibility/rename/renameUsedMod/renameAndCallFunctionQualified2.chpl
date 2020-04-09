module Foo {
  proc callMe() {
    writeln("I was called!");
  }
}

module User {
  use Foo as Bar;

  proc main() {
     // Ensures renaming doesn't allow us to find the function when called in a
     // qualified manner using the old name.
    Foo.callMe();
  }
}
