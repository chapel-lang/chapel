// Verifies the behavior when multiple initializers would want to use the same
// type function for a declaration
class Foo {
  var x;

  proc init(type t) {
    var xVal: t;
    x = xVal;
    super.init();
  }

  proc init(xVal) {
    x = xVal;
    super.init();
  }
}

var f: Foo(int);
writeln(f.type: string);
