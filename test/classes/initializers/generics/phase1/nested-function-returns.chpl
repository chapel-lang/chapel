// This test exercises the case where an initializer defines a function within
// itself that returns a value
class Foo {
  param field: int;

  proc init(param val) {
    proc nested(param arg) param {
      return arg+2;
    }
    field = nested(val);
  }
}

proc main() {
  var f = new owned Foo(13);
  writeln(f.type: string);
}
