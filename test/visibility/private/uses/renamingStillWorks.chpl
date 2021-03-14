module Foo {
  var a = 13.5;

  var b = false;

}

module M {
  private use Foo only b as c;

  proc main() {
    // Ensures that making a use private doesn't break our ability to rename
    // symbols.
    writeln(c);
  }
}
