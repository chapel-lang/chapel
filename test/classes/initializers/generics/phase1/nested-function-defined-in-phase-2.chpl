// This test exercises the case where an initializer defines a function within
// itself (in a different phase than the one where it was called)
class Foo {
  param field: int;

  proc init(param val) {
    field = val;
    nested();
    this.complete();

    // Where the function definition was placed shouldn't impact its viability
    proc nested() {
      writeln("I'm in the nested function!");
    }
  }
}

proc main() {
  var ownF = new owned Foo(13);
  var f = ownF.borrow();
  writeln(f.type: string);
}
