// Verifies the behavior when a sync statement surrounds a super.init() call.
// As in the field case, this should be acceptable, as sync statements do not
// themselves introduce parallelism or loops.
class Foo {
  var x: real;

  proc init(xVal) {
    x = xVal;
    sync {
      super.init();
    }
  }
}

var foo = new Foo(4.3);
writeln(foo);
delete foo;
