use ProvidesTypes;
use ProvidesOps except #;

// Checks that `except #` means we can't call # on the type.
proc main() {
  var r1 = 0..6;
  var foo = new Foo(3);
  writeln(r1 # foo);
}
