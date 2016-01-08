/*
 * There's a problem with the placement of definitions when nest modules are
 * involved
 *
 * Doesn't work :proc:`gob`
 * Also broken :proc:`Foo.gob`
 * But this works :proc:`Foo.Bar.gob`
 */
module Foo {

  module Bar {
    proc baz() {
      return 1;
    }
  }

  proc gob() {
    return 2;
  }
}
