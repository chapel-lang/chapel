
module DefinesOpIO {
  use ProvidesTypes;
  use IO;
  operator <~>(const ref ch: channel, const x: Foo) const ref throws
    where ch.writing {

    writeln("In ProvidesOps.<~>");
    try ch.readwrite(x.x);
    return ch;
  }
}

use ProvidesTypes;
use DefinesOpIO except <~>;
use IO only ioNewline, stdout;
use IO only <~>;

// Checks that `except <~>` means we can't call <~> on the type.
proc main() {
  var foo = new Foo(3);
  stdout <~> foo; // Expected to fall back on default <~>
  stdout <~> new ioNewline();
}
