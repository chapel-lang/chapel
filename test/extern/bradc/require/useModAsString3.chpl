//
// This tells Chapel to compile Foo.chpl, but does not import any
// of its symbols.
//
require "Foo.chpl";

writeln("In useModAsString3's init");

proc foo() {
  writeln("In useModAsString.chpl's foo");
}


proc main() {
  use Foo only;
  foo();
  Foo.foo();  // use a fully-qualified reference to Foo's foo()
}
