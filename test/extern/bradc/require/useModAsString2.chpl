//
// This tells Chapel to compile Foo.chpl, but does not import any
// of its symbols.
//
require "Foo.chpl";
//
// This tells it to make Foo's symbols available in this scope
//
use Foo;

proc main() {
  foo();
}
