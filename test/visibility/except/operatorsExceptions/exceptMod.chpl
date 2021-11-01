use ProvidesTypes;
use ProvidesOps except %;

// Checks that `except %` means we can't call % on the type.
proc main() {
  var a = new Foo(6);
  var b = new Foo(5);
  writeln(a % b);
}
