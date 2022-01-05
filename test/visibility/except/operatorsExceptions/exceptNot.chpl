use ProvidesTypes;
use ProvidesOps except !;

// Checks that `except !` means we can't call ! on the type.
proc main() {
  var a = new Foo2(true);
  writeln(!a);
}
