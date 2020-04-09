module Foo {
  var a = 14.2;

  const b: bool;

  proc c() {
    writeln("wheeeee");
  }

  proc d(x: int) {
    return x * 2 + 4;
  }
}

module M {
  use Foo except a, c;
  use Foo only a, c;
  // Verifies that you can cover an entire module with mutually exclusive uses
  // at the same scope.

  proc main() {
    writeln(a);
    writeln(b);
    c();
    writeln(d(3)); // Should be 10
  }
}
