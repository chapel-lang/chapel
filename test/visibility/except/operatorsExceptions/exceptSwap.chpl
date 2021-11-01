use ProvidesTypes;
use ProvidesOps except <=>;

// Checks that `except <=>` means we can't call <=> on the type.
proc main() {
  var a: owned Foo? = new Foo(6);
  var b: owned Foo? = new Foo(2);
  // Note: must be explicit with type of a and b due to nilability
  a <=> b; // Expected to fall back on default <=>
  writeln(a);
  writeln(b);
}
