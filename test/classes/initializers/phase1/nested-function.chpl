// This test exercises the case where an initializer defines a function within
// itself
class Foo {
  var field: int;

  proc init(val) {
    proc nested() {
      writeln("I'm in the nested function!");
    }
    field = val;
    nested();
  }
}

proc main() {
  var f = new unmanaged Foo(13);
  writeln(f);
  delete f;
}
