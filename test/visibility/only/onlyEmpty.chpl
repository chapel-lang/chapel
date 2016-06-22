// Verifies that an empty only list breaks the compiler.
module A {
  var whatev: complex;
}

module M {
  use A only ; // Expect a syntax error here

  proc main() {
    writeln("Anything could happen!");
  }
}
