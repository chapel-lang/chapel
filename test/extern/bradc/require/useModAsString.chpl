//
// This tells Chapel to compile Foo.chpl, but does not import any
// of its symbols.
//
require "Foo.chpl";

proc foo() {
  writeln("In useModAsString.chpl's foo");
}


proc main() {
  foo();
}
