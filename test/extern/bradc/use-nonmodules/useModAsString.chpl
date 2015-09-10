//
// This tells Chapel to compile Foo.chpl, but does not import any
// of its symbols.  If this feature sticks around, we've talked
// about switching to the keyword 'require' to avoid this confusion.
//
require "Foo.chpl";

proc foo() {
  writeln("In useModAsString.chpl's foo");
}


proc main() {

  foo();
}
