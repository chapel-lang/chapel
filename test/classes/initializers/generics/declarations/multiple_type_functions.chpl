// Verifies the behavior when multiple initializers would want to use the same
// type function for a declaration, but our "one with more type arguments" rule
// wouldn't help us choose.
class Foo {
  type t;
  var x;
  var y;

  proc init(type tVal, xVal) {
    var yVal: tVal;
    t = tVal;
    x = xVal;
    y = yVal;
  }

  proc init(type tVal, xVal, yVal: bool) {
    t = tVal;
    x = xVal;
    y = yVal;
  }
}

var f: Foo(int, real);
writeln(f.type: string);
