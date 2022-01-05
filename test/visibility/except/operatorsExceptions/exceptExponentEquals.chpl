use ProvidesTypes;
use ProvidesOps except **=;

// Checks that `except **=` means we can't call **= on the type.
proc main() {
  var a = new Foo(6);
  var b = new Foo(3);
  a **= b; // When **= is not available, should fall back on **
  writeln(a);
}
