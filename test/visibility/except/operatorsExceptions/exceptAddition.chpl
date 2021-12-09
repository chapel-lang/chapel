use ProvidesTypes;
use ProvidesOps except +;
// Note: the above also prevents the unary + operator

// Checks that `except +` means we can't call + on the type.
proc main() {
  var a = new Foo(3);
  var b = new Foo(7);
  writeln(a + b);
}
