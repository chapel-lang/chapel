// This test exercises the case where an initializer defines a function within
// itself (in a different phase than the one where it was called)
class Foo {
  var field: int;

  proc init(val) {
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
  var f = new unmanaged Foo(13);
  writeln(f);
  delete f;
}
