module Foo {
  var a = 11;

  private const b = -3.5;
}

module M {
  public use Foo;

  proc main() {
    // Verify that using a module publicly in an explicit manner doesn't break
    // our implementation of private.
    writeln(b);
  }
}
