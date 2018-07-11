// This test exercises the case where an initializer defines a function within
// itself that returns a value
class Foo {
  var field: int;

  proc init(val) {
    proc nested(arg) {
      return arg+2;
    }
    field = nested(val);
  }
}

proc main() {
  var f = new owned Foo(13);
  writeln(f);
}
