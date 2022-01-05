use ProvidesTypes;
use ProvidesOps except by;

// Checks that `except by` means we can't call by on the type.
proc main() {
  var r1 = 0..6;
  var foo = new Foo(3);
  writeln(r1 by foo);
}
