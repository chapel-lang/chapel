// Verifies that symbols we won't match against don't shadow those with the
// same name in outer scopes
module Foo {
  var a: int = 25;
  var b = false;
}

module M {
  var a = 94;

  proc main() {
    use Foo only b;

    writeln(a);
  }
}
