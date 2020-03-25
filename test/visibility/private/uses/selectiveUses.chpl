module Foo {
  var a = 13.5;

  var b = false;

}

module M {
  private use Foo only b as c;
  public use Foo except b;

  var d = c;

  proc main() {
    // Verifies that marking multiple uses of the same module with different
    // privacy doesn't harm our ability to access the symbols made available
    // through those uses.
    writeln(a);
    writeln(c);
  }
}
