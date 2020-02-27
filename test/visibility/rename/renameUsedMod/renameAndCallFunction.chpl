module Foo {
  proc callMe() {
    writeln("I was called!");
  }
}

module User {
  use Foo as Bar;

  proc main() {
    callMe(); // Ensures renaming doesn't prevent us from finding the function
  }
}
