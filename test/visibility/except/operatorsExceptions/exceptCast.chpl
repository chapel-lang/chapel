use ProvidesTypes;
use ProvidesOps except :;

// Checks that `except :` means we can't call : on the type.
proc main() {
  var foo = new Foo(3);
  var bar = foo: Bar;
  writeln(bar.type: string);
  writeln(bar);
}
