use ProvidesTypes;
use ProvidesOps except &=;

// Checks that `except &=` means we can't call &= on the type.
proc main() {
  var a = new Foo2(true);
  var b = new Foo2(false);
  a &= b; // Without explicit &=, falls back on &, so expected to compile
  writeln(a);
}
