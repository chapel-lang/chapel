//
// See issue #14861. Once `map.this` has been adjusted we can fix map so
// that it works with non-nilable classes, and then remove this test.
//

use Map;

class SomeClass { var x: int = 0; }

type NilClass = owned SomeClass?;
type NonNilClass = owned SomeClass;

config type TestClass = NilClass;

proc main() {
  var m: map(int, TestClass, false);
  return;
}

