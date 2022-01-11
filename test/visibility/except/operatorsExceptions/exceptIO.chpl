use ProvidesTypes;
use ProvidesOps except <~>;
use IO only ioNewline, stdout;
use IO only <~>;

// Checks that `except <~>` means we can't call <~> on the type.
proc main() {
  var foo = new Foo(3);
  stdout <~> foo; // Expected to fall back on default <~>
  stdout <~> new ioNewline();
}
