use ProvidesTypes;
use ProvidesOps except +=;

// Checks that `except +=` means we can't call += on the type.
proc main() {
  var a = new Foo(2);
  var b = new Foo(3);
  a += b; // Expected to fall back on + definition
  writeln(a);
}
