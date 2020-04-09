// This test exercises the case where an initializer defines a function within
// itself (in a different phase than the one where it was called) that returns
// a value
class Foo {
  var field: int;

  proc init(val) {
    field = nested(val);
    this.complete();

    // Where the function definition was placed shouldn't impact its viability
    proc nested(arg) {
      return arg + 2;
    }
  }
}

proc main() {
  var f = new borrowed Foo(13);
  writeln(f);
}
