use ProvidesTypes;
use ProvidesOps except -;
// Note: the above also excludes the binary - operator

// Checks that `except -` means we can't call - on the type.
proc main() {
  var a = new Foo(3);
  writeln(-a);
}
