module Foo {
  var a = 13.5;

  var b = false;

}

module M {
  private use Foo except b;

  proc main() {
    // Ensures that making a use private doesn't allow symbols that are in
    // the except list to be found.
    writeln(b);
  }
}
