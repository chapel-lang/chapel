// Nest the modules a bunch to make sure that the fix works for non-toplevel
// modules as well as toplevel ones.
module SomeLib {
  module A {
    module B {
      module C {
        var x = 0;

        record data {
          var a: int;

          proc init(a) { this.a = a; x += 1; }
        }
      }
    }
  }
}
