// Verifies the behavior when multiple initializers would want to use the same
// type function for a declaration
class Foo {
  var x;

  proc init(type t) {
    var xVal: t;
    x = xVal;
  }

  proc init(xVal) {
    x = xVal;
  }
}

var f: borrowed Foo(int)?;
writeln(f.type :class :string);
