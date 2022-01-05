use ProvidesTypes;
use ProvidesOps except -;
// Note: also prevents the unary negation operator

// Checks that `except -` means we can't call - on the type.
proc main() {
  var a = new Foo(6);
  var b = new Foo(2);
  writeln(a - b);
}
