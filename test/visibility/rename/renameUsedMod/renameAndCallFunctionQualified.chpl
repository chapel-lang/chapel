module Foo {
  proc callMe() {
    writeln("I was called!");
  }
}

module User {
  use Foo as Bar;

  proc main() {
     // Ensures renaming doesn't prevent us from finding the function when
     // called in a qualified manner.
    Bar.callMe();
  }
}
