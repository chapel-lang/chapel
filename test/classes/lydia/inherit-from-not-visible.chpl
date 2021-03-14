module A {
  class Foo {
    var x: int;
  }
}
module B {
  // should use A; but doesn't to lock in error message
  class Bar: Foo {
    var y: bool;
  }

  proc main() {
    var bar = new Bar();
    writeln(bar);
  }
}
