//
// This tells Chapel to compile Foo.chpl, but does not import any
// of its symbols.  If this feature sticks around, we've talked
// about switching to the keyword 'require' to avoid this confusion.
//
require "Foo.chpl";
//
// This tells it to make Foo's symbols available in this scope
//
use Foo;

proc main() {
  foo();
}
