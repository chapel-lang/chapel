use ProvidesTypes;
use ProvidesOps except align;

// Checks that `except align` means we can't call align on the type.
proc main() {
  var r1 = 0..6;
  var a = new Foo(3);
  var r1b = r1 by 4 align a;
  writeln(r1b);
  for i in r1b {
    writeln(i);
  }
}
