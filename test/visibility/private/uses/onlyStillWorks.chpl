module Foo {
  var a = 13.5;

  var b = false;

}

module M {
  private use Foo only b;

  proc main() {
    // Ensures that making a use private doesn't allow symbols that aren't in
    // the only list to be found.
    writeln(a);
  }
}
