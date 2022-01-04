use ProvidesTypes;
use ProvidesOps except +;
// Note: also excludes the binary addition operator

// Checks that `except +` means we can't call + on the type.
proc main() {
  var a = new Foo(-2);
  writeln(+a);
}
