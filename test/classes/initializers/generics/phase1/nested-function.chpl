// This test exercises the case where an initializer defines a function within
// itself
class Foo {
  param field: int;

  proc init(param val) {
    proc nested() {
      writeln("I'm in the nested function!");
    }
    field = val;
    nested();
  }
}

proc main() {
  var f = new shared Foo(13);
  writeln(f.type: string);
}
